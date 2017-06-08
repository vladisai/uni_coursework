#include "Client.h"
#include "Utility.h"

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
