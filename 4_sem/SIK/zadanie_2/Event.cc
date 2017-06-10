#include <sstream>
#include "Event.h"

#include "GameOverEvent.h"
#include "NewGameEvent.h"
#include "PixelEvent.h"
#include "PlayerEliminatedEvent.h"

#include "Serializer.h"
#include "Utility.h"

Event::Event(unsigned event_no, char event_type)
    : event_no(event_no), event_type(event_type), iscrc32Ready(false) {}

unsigned Event::getCRC32() {
    if (iscrc32Ready) {
        return crc32;
    } else {
        RawData d = this->serialize(true);
        d.erase(d.end() - 4, d.end()); // remove the crc at the end
        iscrc32Ready = true;
        return crc32 = computeCRC32(d);
    }
}

std::shared_ptr<Event> Event::deserialize(RawData data) {
    Serializer s(data);
    s.popUInt32(); // len
    s.popUInt32(); // event_no
    char event_type = s.popChar();
    switch (event_type) {
    case EventType::NewGame:
        return NewGameEvent::deserialize(data);
    case EventType::Pixel:
        return PixelEvent::deserialize(data);
    case EventType::PlayerEliminated:
        return PlayerEliminatedEvent::deserialize(data);
    case EventType::GameOver:
        return GameOverEvent::deserialize(data);
    default:
        throw BadEventType(event_type);
    }
}

unsigned Event::getEventNo() const { return event_no; }

char Event::getEventType() const { return event_type; }

bool Event::operator==(const Event &other) const {
    return (event_no == other.event_no
            && event_type == other.event_type
            && crc32 == other.crc32
            && iscrc32Ready == other.iscrc32Ready);
}

std::string Event::toString() const {
    std::stringstream ss;
    ss << "EVENT(event_no = " << event_no << ", event_type = " << (int)event_type << ")";
    return ss.str();
}

bool Event::isConsistent(Event::SharedPtr event) {
    return true;
}
