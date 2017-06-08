#ifndef NETACKA_SIK_CLIENTCONFIG_H
#define NETACKA_SIK_CLIENTCONFIG_H

#include <cstdint>
#include <string>

namespace ClientConfig {
    extern std::string serverPortNumber;
    extern std::string serverHost;
    extern std::string guiPortNumber;
    extern std::string guiHost;
    extern std::string playerName;

    void loadFromArgs(int argc, char **argv);

};


#endif //NETACKA_SIK_CLIENTCONFIG_H
