#include "GameOverEvent.h"

GameOverEvent::GameOverEvent(uint32_t event_no) :
    Event(event_no, EventType::GameOver) {}

RawData GameOverEvent::serialize(bool noCRC) {
    Serializer s;
    uint32_t len = 0;
    len += sizeof(event_no);
    len += sizeof(event_type);
    s.add(len);
    s.add(event_no);
    s.add(event_type);
    if (!noCRC) {
        s.add(getCRC32());
    }
    return s.getData();
}

std::shared_ptr<GameOverEvent> GameOverEvent::deserialize(RawData data) {
    Serializer s(data);
    uint32_t len = s.popUInt32();
    uint32_t event_no = s.popUInt32();
    char event_type = s.popChar();
    uint32_t crc32 = s.popUInt32();
    GameOverEvent ret(event_no);
    if (ret.getCRC32() != crc32) {
        throw BadCRC32Exception();
    }
    return std::make_shared<GameOverEvent>(ret);
}

bool GameOverEvent::operator == (const GameOverEvent &other) const {
    return Event::operator==(other);
}

std::string GameOverEvent::toString(const std::vector<std::string> &names) const {
    return "";
}
