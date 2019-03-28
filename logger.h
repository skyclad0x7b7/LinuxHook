#ifndef __LOGGER_H__
#define __LOGGER_H__

#include <stdio.h>
#include <fstream>
#include <string>
#include <string.h>
#include "libc_func.h"

const char *const LOG_PATH = "./logs";

namespace Mini
{

class Logger
{
public:
    static Logger& instance()
    {
        static Logger* ins = new Logger();
        return *ins;
    }

    void reopen_logfile();
    void log(const std::string&);
private:
    Logger();
    ~Logger();

    pid_t _pid;
    char _filename[256];
    std::ofstream _ofs;
};

} // namespace Mini

#endif // __LOGGER_H__