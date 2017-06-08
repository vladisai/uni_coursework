//
// Created by vlad on 03.06.17.
//

#ifndef NETACKA_SIK_CONFIG_H
#define NETACKA_SIK_CONFIG_H

#include <cstdint>

namespace ServerConfig {
    extern uint32_t planeWidth;
    extern uint32_t planeHeight;
    extern uint32_t portNumber;
    extern uint32_t gameSpeed;
    extern uint32_t turningSpeed;
    extern uint32_t randomGeneratorSeed;
    extern uint32_t bufferSize;

    void loadFromArgs(int argc, char **argv);
}

#endif //NETACKA_SIK_CONFIG_H
