#include <sstream>
#include "ClientMessage.h"
#include "Utility.h"

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
    s.add(playerName, false);
    return s.getData();
}

ClientMessage ClientMessage::deserialize(RawData data) {
    Serializer s(std::move(data));
    uint64_t session_id = s.popUInt64();
    char turn_direction = s.popChar();
    uint32_t next_expected_event_no = s.popUInt32();
    std::string playerName = s.popString();
    if (turn_direction > 1 || turn_direction < -1) {
        throw BadMessageDataException();
    }
    if (!checkName(playerName)) {
        throw BadMessageDataException();
    }
    return ClientMessage(session_id, turn_direction, next_expected_event_no,
                         playerName);
}

std::string ClientMessage::getPlayerName() {
    return playerName;
}

uint32_t ClientMessage::getNextExpectedEventNo() {
    return next_expected_event_no;
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

std::string ClientMessage::toString() {
    std::stringstream ss;
    ss << "ClientMessage (session_id = " << session_id;
    ss << ", turn_direction = " << (int)turn_direction;
    ss << ", next_expected_event_no = " << next_expected_event_no;
    ss << ", playerName = " << playerName;
    ss << ")";
    return ss.str();
}
