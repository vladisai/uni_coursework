#ifndef CLIENTMESSAGE_H
#define CLIENTMESSAGE_H

#include <cstdint>

#include "Serializer.h"

class ClientMessage {
public:
    ClientMessage(uint64_t session_id, char turn_direction, uint32_t next_expected_event_no,
            std::string playerName);

    RawData serialize()const;

    static ClientMessage deserialize(RawData data);

    uint64_t getSessionId();

    char getTurnDirection();

    uint32_t getNextExpectedEventNo();

    std::string getPlayerName();

    std::string toString();

    bool operator==(const ClientMessage &other) const;

private:
    uint64_t session_id;
    char turn_direction;
    uint32_t next_expected_event_no;
    std::string playerName;

};

#endif // CLIENTMESSAGE_H
