#include "ServerConfig.h"

uint32_t ServerConfig::planeHeight = 800;
uint32_t ServerConfig::planeWidth = 800;
uint32_t ServerConfig::portNumber = 12345;
uint32_t ServerConfig::gameSpeed = 50;
uint32_t ServerConfig::turningSpeed = 6;
uint32_t ServerConfig::bufferSize = 512;

void ServerConfig::loadFromArgs(int argc, char **argv) {
    // todo
}

