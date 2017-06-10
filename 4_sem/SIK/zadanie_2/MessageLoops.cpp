#include "MessageLoops.h"

#include <thread>

std::thread MessageLoops::runReceiverLoop(GameManager::SharedPtr manager,
                            ServerConnection::SharedPtr connection) {
    return std::thread([=]() {
        while (!manager->shouldTerminate()) {
            try {
                auto e = connection->receive();
                manager->addMessage(e);
            } catch (...) {}
        }
    });
}
