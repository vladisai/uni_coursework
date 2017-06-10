//
// Created by vlad on 03.06.17.
//

#ifndef NETACKA_SIK_CONFIG_H
#define NETACKA_SIK_CONFIG_H

#include <cstdint>
#include <chrono>

namespace ServerConfig {
    extern uint32_t planeWidth;
    extern uint32_t planeHeight;
    extern uint32_t portNumber;
    extern std::chrono::microseconds gameSpeed;
    extern uint32_t turningSpeed;
    extern uint32_t randomGeneratorSeed;
    extern uint32_t bufferSize;
    extern std::chrono::milliseconds clientTimeOutTime;
    extern uint32_t maxMessageLen;
    extern uint32_t maxClientsNumber;

    void loadFromArgs(int argc, char **argv);

}

#endif //NETACKA_SIK_CONFIG_H
