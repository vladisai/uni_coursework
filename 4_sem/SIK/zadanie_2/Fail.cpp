#include "Fail.h"

const std::string usageClient = "Usage: ./siktacka-client player_name game_server_host[:port] [ui_server_host[:port]]";
const std::string usageServer = "Usage: ./siktacka-server [-W n] [-H n] [-p n] [-s n] [-t n] [-r n]";

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
    std::cerr << std::endl;
    std::cerr << "-------------------------------" << std::endl;
    std::cerr << error << std::endl;
    std::cerr << errno << std::endl;
    std::cerr << strerror(errno) << std::endl;
    std::cerr << "-------------------------------" << std::endl;
    std::cerr << std::endl;
}

void failSysErrorExit(const std::string& error)
{
    failSysError(error);
    exit(EXIT_FAILURE);
}

void failErrorExit(const std::string &error) {
    std::cerr << error << std::endl;
    exit(EXIT_FAILURE);
}

