#include "Client.h"
#include "ServerConfig.h"

Client::Client(ServerConnection::ClientAddress address,
    uint64_t sessionId,
            std::string name,
    uint32_t nextExpectedEventNo) :
    address(address),
    sessionId(sessionId),
    name(name),
    nextExpectedEventNo(nextExpectedEventNo),
    lastActiveTime(Clock::now()),
    ready(false),
    active(true),
    playing(false),
    alive(false) {
}

bool Client::operator==(const Client &other) const {
    return sessionId == other.sessionId
           && address == other.address;
}

std::string Client::getName() {
    return name;
}

ServerConnection::ClientAddress Client::getAddress() {
    return address;
}

void Client::setSessionId(uint64_t value) {
    sessionId = value;
}

uint64_t Client::getSessionId() {
    return sessionId;
}

void Client::setIsPlaying(bool value) {
    playing = value;
}

bool Client::isPlaying() {
    return playing;
}

void Client::setIsReady(bool value) {
    ready = value;
}

bool Client::isReady() {
    return ready;
}

bool Client::isActive() {
    return active;
}

void Client::setIsActive(bool value) {
    active = value;
}

void Client::setName(std::string value) {
    name = value;
}

void Client::updateActiveTime() {
    lastActiveTime = Clock::now();
}

bool Client::isTimedOut() {
    return Clock::now() - lastActiveTime > ServerConfig::clientTimeOutTime;
}

bool Client::isAlive() {
    return alive;
}

void Client::setIsAlive(bool value) {
    alive = value;
}

