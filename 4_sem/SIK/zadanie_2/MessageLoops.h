#ifndef NETACKA_SIK_MESSAGELOOPS_H
#define NETACKA_SIK_MESSAGELOOPS_H

#include <thread>
#include "ServerConnection.h"
#include "GameManager.h"

namespace MessageLoops {
    std::thread runReceiverLoop(GameManager::SharedPtr, ServerConnection::SharedPtr);
}


#endif //NETACKA_SIK_MESSAGELOOPS_H
