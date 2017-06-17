#include <json/json.h>
#include <fstream>


#include <Log.h>
#include <FasInfo.h>


#include <boost/lexical_cast.hpp>


FasInfo::FasInfo() :
  fasInfo_(),
  pollerInfo_(),
  infos_() {
  infos_["poller"] = &pollerInfo_;
  infos_["fas"] = &fasInfo_;
}


int FasInfo::load(const std::string& filename) {
  std::ifstream in;

  in.open(filename, std::ios_base::in);

  if (!in.is_open()) {
    LOGGER_ERROR(" Open info file error!");
    return -1;
  }

  Json::Reader jsonReader;
  Json::Value root;

  if (!jsonReader.parse(in, root)) {
    LOGGER_ERROR(" Parse info file error!");
    return -1;
  }

  if (root.isMember("fas")) {
    fasInfo_["thread_num"] = root["fas"]["thread_num"].asString();
    fasInfo_["server_ip"] = root["fas"]["server_ip"].asString();
    fasInfo_["server_port"] = root["fas"]["server_port"].asString();
    fasInfo_["log_conf"] = root["fas"]["log_conf"].asString();
  } else {
    LOGGER_ERROR(" can not find fas json root!");
  }

  if (root.isMember("poller")) {
    pollerInfo_["event_num"] = root["poller"]["event_num"].asString();
    pollerInfo_["time_out"] = root["poller"]["time_out"].asString();
    pollerInfo_["type"] = root["poller"]["type"].asString();
  } else {
    LOGGER_ERROR(" can not find poller json root!");
  }

  return 0;
}

std::string FasInfo::getLogConf() const {
  return getValue("fas", "log_conf");
}

std::string FasInfo::getServerIp() const {
  return getValue("fas", "server_ip");
}

short FasInfo::getServerPort() const {
  std::string port = getValue("fas", "server_port");
  if (port.empty()) {
    return -1;
  }
  return boost::lexical_cast<short>(port);
}

int FasInfo::getThreadNum() const {
  std::string num = getValue("fas", "thread_num");
  if (num.empty()) {
    return -1;
  }
  return boost::lexical_cast<int>(num);
}

std::string FasInfo::getPollerType() const {
  return getValue("poller", "type");
}

int FasInfo::getPollerTimeout() const {
  std::string time_out = getValue("poller", "time_out");
  if (time_out.empty()) {
    return -1;
  }
  return boost::lexical_cast<int>(time_out);
}

int FasInfo::getPollerNum() const {
  std::string event_num = getValue("poller", "event_num");
  if (event_num.empty()) {
    return -1;
  }
  return boost::lexical_cast<int>(event_num);
}

std::string FasInfo::getValue(const std::string& root, const std::string& key) const {
  std::string value = "";

  auto info = infos_.find(root);
  if (info != infos_.end()) {
    auto viter = info->second->find(key);
    if (viter != info->second->end()) {
      value = viter->second;
    }
  }

  return value;
}