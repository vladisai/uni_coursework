#include <sstream>
#include "PixelEvent.h"
#include "NewGameEvent.h"

PixelEvent::PixelEvent(unsigned event_no, char playerNumber, unsigned x,
                       unsigned y)
    : Event(event_no, EventType::Pixel), playerNumber(playerNumber), x(x),
      y(y) {}

RawData PixelEvent::serialize(bool noCRC) {
    Serializer s;
    unsigned len = 0;
    len += sizeof(event_no);
    len += sizeof(event_type);
    len += sizeof(playerNumber);
    len += sizeof(x);
    len += sizeof(y);
    s.add(len);
    s.add(event_no);
    s.add(event_type);
    s.add(playerNumber);
    s.add(x);
    s.add(y);
    if (!noCRC) {
        s.add(getCRC32());
    }
    return s.getData();
}

std::shared_ptr<PixelEvent> PixelEvent::deserialize(RawData data) {
    Serializer s(data);
    unsigned len = s.popUInt32();
    unsigned event_no = s.popUInt32();
    char event_type = s.popChar();
    char playerNumber = s.popChar();
    unsigned x = s.popUInt32();
    unsigned y = s.popUInt32();
    unsigned crc32 = s.popUInt32();
    PixelEvent ret(event_no, playerNumber, x, y);
    if (ret.getCRC32() != crc32) {
        throw BadCRC32Exception();
    }
    return std::make_shared<PixelEvent>(ret);
}

unsigned PixelEvent::getX() { return x; }

unsigned PixelEvent::getY() { return y; }

char PixelEvent::getPlayerNumber() { return playerNumber; }

bool PixelEvent::operator == (const PixelEvent &other) const {
    return (Event::operator==(other)
            && x == other.x
            && y == other.y
            && playerNumber == other.playerNumber);
}

std::string PixelEvent::toString(const std::vector<std::string> &names) const {
    std::stringstream ss;
    ss << "PIXEL " << x << " " << y << " " << names[playerNumber] << std::endl;
    return ss.str();
}

bool PixelEvent::isConsistent(Event::SharedPtr event) {
    if (event->getEventType() != EventType::NewGame) {
        return false;
    }
    auto e = std::static_pointer_cast<NewGameEvent>(event);
    return e->getPlayerNames().size() > playerNumber
           && x <= e->getMaxX()
            && y <= e->getMaxY();
}
