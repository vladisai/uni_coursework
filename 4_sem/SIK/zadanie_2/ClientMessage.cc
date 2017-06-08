#include "ClientMessage.h"

ClientMessage::ClientMessage(uint64_t session_id, char turn_direction,
                             unsigned int next_expected_event_no,
                             std::string playerName)
    : session_id(session_id), turn_direction(turn_direction),
      next_expected_event_no(next_expected_event_no), playerName(playerName) {}

RawData ClientMessage::serialize()const {
    Serializer s;
    s.add(session_id);
    s.add(turn_direction);
    s.add(next_expected_event_no);
    s.add(playerName);
    return s.getData();
}

ClientMessage ClientMessage::deserialize(RawData data) {
    Serializer s(std::move(data));
    uint64_t session_id = s.popUInt64();
    char turn_direction = s.popChar();
    unsigned int next_expected_event_no = s.popUInt32();
    std::string playerName = s.popString();
    return ClientMessage(session_id, turn_direction, next_expected_event_no,
                         playerName);
}

std::string ClientMessage::getPlayerName() {
    return playerName;
}

uint32_t ClientMessage::getNextExpectedEventNo() {
    return 0;
}

char ClientMessage::getTurnDirection() {
    return turn_direction;
}

uint64_t ClientMessage::getSessionId() {
    return session_id;
}

bool ClientMessage::operator==(const ClientMessage &other) const {
    return (session_id == other.session_id
            && turn_direction == other.turn_direction
            && next_expected_event_no == other.next_expected_event_no
            && playerName == other.playerName);
}
