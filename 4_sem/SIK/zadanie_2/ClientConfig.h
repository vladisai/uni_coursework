#ifndef NETACKA_SIK_CLIENTCONFIG_H
#define NETACKA_SIK_CLIENTCONFIG_H

#include <cstdint>
#include <string>
#include <chrono>

namespace ClientConfig {
    extern std::string serverPortNumber;
    extern std::string serverHost;
    extern std::string guiPortNumber;
    extern std::string guiHost;
    extern std::string playerName;
    extern std::chrono::milliseconds messageFrequency;

    void loadFromArgs(int argc, const char **argv);
};


#endif //NETACKA_SIK_CLIENTCONFIG_H
