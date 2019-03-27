#include "logger.h"

namespace Mini
{

Logger::Logger() 
{
    _pid = lib_getpid();
    memset(_filename, 0, sizeof(_filename));
    snprintf(_filename, sizeof(_filename), "%s/log_%d.txt", LOG_PATH, _pid);
    _ofs.open(_filename, std::ofstream::out | std::ofstream::app );
}

Logger::~Logger()
{
    _ofs.close();
}

void Logger::reopen_logfile()
{
    if(_ofs)
        _ofs.close();
    _pid = lib_getpid();
    memset(_filename, 0, sizeof(_filename));
    snprintf(_filename, sizeof(_filename), "%s/log_%d.txt", LOG_PATH, _pid);
    _ofs.open(_filename, std::ofstream::out | std::ofstream::app );
}

void Logger::log(const std::string& log_string)
{
    if(_ofs.good())
    {
        _ofs << log_string;
        _ofs.flush();
    }
    else
        fprintf(stderr, "[-] %d | Logger::log error : _ofs.good failed with file '%s'", _pid, _filename);
}

} // namespace Mini