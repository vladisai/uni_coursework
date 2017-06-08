#ifndef NETACKA_SIK_MESSENGERTHREAD_H
#define NETACKA_SIK_MESSENGERTHREAD_H

#include "GameManager.h"

#include <memory>

void startMessageReceiverThread(std::weak_ptr<GameManager> manager);

void messageReceiverLoop(std::weak_ptr<GameManager> manager);

#endif //NETACKA_SIK_MESSENGERTHREAD_H
