#include <Log.h>
#include <string.h>
#include <errno.h>

const char *Log::log_endl = "\n\r";

std::ostringstream& Log::getBuffer() {
  return buffer_;
}

bool Log::fflush() {
  if (!output_) {
    assert(false);
  }
  return output_(buffer_.str().c_str(), buffer_.str().size());
}

Log::LogLevel Log::logLevel() {
  return Log::LogLevel::DEBUG;
}

void Log::setOutput(default_output_t output) {
  output_ = output;
}

bool defaultLogOutput(const char* data, int len) {
  int ret = ::write(STDOUT_FILENO, data, len);
  if (ret == -1) {
    LOG_SYSERR(strerror(errno));
    return false;
  }
  return true;
}


void Logger(std::string file, int line, string msg) {
  Log log;
  log << file << " " << line << " " << msg << Log::log_endl;
  log.fflush();
}

void Logger(std::string file, int line, Log::LogLevel level, string msg) {
  Log log;
  log << file << " " << line << " " <<  level << " " << msg << Log::log_endl;
  log.fflush();
}

void Logger(std::string file, int line, Log::LogLevel level, std::string func, string msg) {
  Log log;
  log << file << " " << line << " " <<  level << " " << func << " " << msg << Log::log_endl;
  log.fflush();
}

void Logger(std::string file, int line, bool toAbort, string msg) {
  Log log;
  log << file << " " << line << " " << msg << Log::log_endl;
  log.fflush();

  if (toAbort) {
    ::abort();
  }
}

