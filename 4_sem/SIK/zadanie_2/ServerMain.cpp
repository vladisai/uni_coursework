#include <iostream>

#include "ServerConnection.h"
#include "ServerConfig.h"
#include "NewGameEvent.h"
#include "GameManager.h"

using namespace std;

int main(int argc, char **argv) {
    ServerConfig::loadFromArgs(argc, argv);
    RandomGenerator::init();

    ServerConnection::SharedPtr
            connection(std::make_shared<ServerConnection>(ServerConfig::portNumber));

    auto manager = std::make_shared<GameManager>(connection);
    if (!connection->connect()) {
        std::cerr << "Server: error connecting\n";
        return 1;
    }

    manager->run();
    return 0;
}
