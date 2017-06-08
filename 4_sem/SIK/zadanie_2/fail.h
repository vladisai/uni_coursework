#ifndef FAIL_H
#define FAIL_H

#include <cstring>
#include <iostream>
#include <string>
#include <cstdlib>
#include <errno.h>

const std::string usageClient = "Usage: ./client timestamp character host port.";
const std::string usageServer = "Usage: ./server port file.";

void failWrongUsageClient()
{
    std::cerr << usageClient << std::endl;
    exit(EXIT_FAILURE);
}

void failWrongUsageServer()
{
    std::cerr << usageServer << std::endl;
    exit(EXIT_FAILURE);
}

void failSysError(const std::string& error)
{
    std::cerr << error << std::endl;
    std::cerr << errno << std::endl;
    std::cerr << strerror(errno) << std::endl;
    exit(EXIT_FAILURE);
}

void failSysErrorExit(const std::string& error)
{
    failSysError(error);
    exit(EXIT_FAILURE);
}

#endif // FAIL_H
