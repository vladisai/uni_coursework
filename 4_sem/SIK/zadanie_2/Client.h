#ifndef NETACKA_SIK_CLIENT_H
#define NETACKA_SIK_CLIENT_H

#include <memory>
#include <chrono>

#include "Utility.h"
#include "ServerConnection.h"

class Client {
public:
    using SharedPtr = std::shared_ptr<Client>;

    Client(ServerConnection::ClientAddress address,
           uint64_t sessionId,
           std::string name,
           uint32_t nextExpectedEventNo);

    bool operator==(const Client &other) const;

    ServerConnection::ClientAddress getAddress();

    std::string getName();

    void setName(std::string value);

    void setSessionId(uint64_t value);

    uint64_t getSessionId();

    void setIsPlaying(bool value);

    bool isPlaying();

    void setIsReady(bool value);

    bool isReady();

    bool isActive();

    void setIsActive(bool value);

    void updateActiveTime();

    bool isTimedOut();

    bool isAlive();

    void setIsAlive(bool value);


private:
    Clock::time_point lastActiveTime;
    ServerConnection::ClientAddress address;
    uint64_t sessionId;
    std::string name;
    uint32_t nextExpectedEventNo;
    bool active;
    bool playing;
    bool ready;
    bool alive;
};


#endif //NETACKA_SIK_CLIENT_H
