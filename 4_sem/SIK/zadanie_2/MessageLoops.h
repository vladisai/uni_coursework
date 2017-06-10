#ifndef NETACKA_SIK_MESSAGELOOPS_H
#define NETACKA_SIK_MESSAGELOOPS_H

#include "ServerConnection.h"
#include "GameManager.h"

namespace MessageLoops {
    void runReceiverLoop(GameManager::SharedPtr, ServerConnection::SharedPtr);
}


#endif //NETACKA_SIK_MESSAGELOOPS_H
