#include "ClientConfig.h"

std::string ClientConfig::serverPortNumber = "12345";
std::string ClientConfig::serverHost = "localhost";
std::string ClientConfig::guiPortNumber = "12346";
std::string ClientConfig::guiHost = "localhost";
std::string ClientConfig::playerName = "player";

void ClientConfig::loadFromArgs(int argc, char **argv) {
    // todo
}

