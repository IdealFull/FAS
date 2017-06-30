#ifndef FAS_LOG_H
#define FAS_LOG_H
#include <log4cplus/layout.h>
#include <log4cplus/configurator.h>
#include <log4cplus/logger.h>
#include <log4cplus/fileappender.h>
#include <log4cplus/loggingmacros.h>
#include <log4cplus/helpers/property.h>
#include <log4cplus/configurator.h>
#include <log4cplus/helpers/loglog.h>
#include <log4cplus/loggingmacros.h>

namespace fas {

static const std::string CRLF = "\r\n";

class CommonLog {
public:
    log4cplus::Logger* trace_;
    log4cplus::Logger* debug_;
    log4cplus::Logger* info_;
    log4cplus::Logger* warn_;
    log4cplus::Logger* error_;
    log4cplus::Logger* sys_error_;
    log4cplus::Logger* fetal_;

public:
    int init(const std::string& name, std::vector<std::string>& log_name);

    static CommonLog* instance();

private:
    CommonLog() {}

    static CommonLog* logger_;
};

bool LoggerInit();

}
#define LOGGER_TRACE(MSG) LOG4CPLUS_TRACE(*fas::CommonLog::instance()->trace_, MSG)
#define LOGGER_DEBUG(MSG) LOG4CPLUS_DEBUG(*fas::CommonLog::instance()->debug_, MSG)
#define LOGGER_INFO(MSG) LOG4CPLUS_INFO(*fas::CommonLog::instance()->info_, MSG)
#define LOGGER_WARN(MSG) LOG4CPLUS_WARN(*fas::CommonLog::instance()->warn_, MSG)
#define LOGGER_ERROR(MSG) LOG4CPLUS_ERROR(*fas::CommonLog::instance()->error_, MSG)
#define LOGGER_FATAL(MSG) LOG4CPLUS_FATAL(*fas::CommonLog::instance()->fetal_, MSG)
#define LOGGER_SYSERR(MSG) LOG4CPLUS_ERROR(*fas::CommonLog::instance()->error_, MSG)

#endif // FAS_LOG_H

