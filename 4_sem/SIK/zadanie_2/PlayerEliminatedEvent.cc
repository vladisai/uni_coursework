#include <sstream>
#include "PlayerEliminatedEvent.h"
#include "NewGameEvent.h"

PlayerEliminatedEvent::PlayerEliminatedEvent(uint32_t event_no,
                                             char playerNumber)
    : Event(event_no, EventType::PlayerEliminated), playerNumber(playerNumber) {

}

RawData PlayerEliminatedEvent::serialize(bool noCRC) {
    Serializer s;
    uint32_t len = 0;
    len += sizeof(event_no);
    len += sizeof(event_type);
    len += sizeof(playerNumber);
    s.add(len);
    s.add(event_no);
    s.add(event_type);
    s.add(playerNumber);
    if (!noCRC) {
        s.add(getCRC32());
    }
    return s.getData();
}

std::shared_ptr<PlayerEliminatedEvent>
PlayerEliminatedEvent::deserialize(RawData data) {
    Serializer s(data);
    uint32_t len = s.popUInt32();
    uint32_t event_no = s.popUInt32();
    char event_type = s.popChar();
    char playerNumber = s.popChar();
    uint32_t crc32 = s.popUInt32();
    PlayerEliminatedEvent ret(event_no, playerNumber);
    if (ret.getCRC32() != crc32) {
        throw BadCRC32Exception();
    }
    return std::make_shared<PlayerEliminatedEvent>(ret);
}

char PlayerEliminatedEvent::getPlayerNumber() {
    return playerNumber;
}

bool PlayerEliminatedEvent::operator == (const PlayerEliminatedEvent &other) const {
    return (Event::operator==(other)
            && playerNumber == other.playerNumber);
}

std::string PlayerEliminatedEvent::toString(const std::vector<std::string> &names) const {
    std::stringstream ss;
    ss << "PLAYER_ELIMINATED " << names[playerNumber] << std::endl;
    return ss.str();
}

bool PlayerEliminatedEvent::isConsistent(Event::SharedPtr event) {
    if (event->getEventType() != EventType::NewGame) {
        return false;
    }
    auto e = std::static_pointer_cast<NewGameEvent>(event);
    return e->getPlayerNames().size() > playerNumber;
}
