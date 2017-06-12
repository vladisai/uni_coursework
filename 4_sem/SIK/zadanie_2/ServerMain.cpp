#include <iostream>
#include <csignal>

#include "ServerConnection.h"
#include "ServerConfig.h"
#include "NewGameEvent.h"
#include "GameManager.h"

using namespace std;


GameManager::SharedPtr manager;

void interrupt(int)
{
    std::cerr << "stopping" << std::endl;
    manager->stop();
}

int main(int argc, char **argv) {
    try {
        ServerConfig::loadFromArgs(argc, argv);
    } catch (...) {
        failWrongUsageServer();
    }
    RandomGenerator::init();

    ServerConnection::SharedPtr
            connection(std::make_shared<ServerConnection>(ServerConfig::portNumber));

    manager = std::make_shared<GameManager>(connection);
    connection->connect();

    if (signal(SIGINT, interrupt) == SIG_ERR) {
        failSysErrorExit("Unable to change signal handler");
    }
    manager->run();

    std::cerr << "closing connections" << std::endl;
    connection->disconnect();
    std::cerr << "exiting" << std::endl;

    return 0;
}
