#include <getopt.h>
#include "ServerConfig.h"
#include "Fail.h"
#include "Utility.h"

using namespace std::chrono;

uint32_t ServerConfig::planeHeight = 800;
uint32_t ServerConfig::planeWidth = 800;
uint32_t ServerConfig::portNumber = 12345;
microseconds ServerConfig::gameSpeed = microseconds(20 * 1000); // 50 ticks per second
uint32_t ServerConfig::turningSpeed = 6;
uint32_t ServerConfig::bufferSize = 512;
milliseconds ServerConfig::clientTimeOutTime = milliseconds(2000);
uint32_t ServerConfig::maxMessageLen = 512;
uint32_t ServerConfig::randomGeneratorSeed = time(0);
uint32_t ServerConfig::maxClientsNumber = 42;

void ServerConfig::loadFromArgs(int argc, char **argv) {
    int arg;
    optind = 1;
    while ((arg = getopt(argc, argv, "W:H:p:s:t:r:")) != -1) {
        switch (arg) {
            case 'W':
                planeWidth = parseUint32(optarg);
                break;
            case 'H':
                planeHeight = parseUint32(optarg);
                break;
            case 'p':
                portNumber = parseUint32(optarg);
                break;
            case 's':
                gameSpeed = microseconds(1000000 / parseUint32(optarg));
                break;
            case 't':
                turningSpeed = parseUint32(optarg);
                break;
            case 'r':
                randomGeneratorSeed = parseUint32(optarg);
                break;
            default:
                // that means that some option wasn't passed an argument,
                // or that we've encountered an unknown option
                throw IllegalCommandLineArguments();
        }
    }
}

