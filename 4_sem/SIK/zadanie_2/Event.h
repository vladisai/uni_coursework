#ifndef EVENT_H
#define EVENT_H

#include <memory>
#include <vector>

class Event;

#include "Serializer.h"
#include "Fail.h"

enum EventType {
    NewGame = 0,
    Pixel = 1,
    PlayerEliminated = 2,
    GameOver = 3,
};

class Event {
  public:
    using SharedPtr = std::shared_ptr<Event>;
    Event(uint32_t event_no, char event_type);

    virtual RawData serialize(bool noCRC = false) = 0;

    static std::shared_ptr<Event> deserialize(RawData data);

    uint32_t getCRC32();

    uint32_t getEventNo() const;

    char getEventType() const;

    virtual bool operator==(const Event &other) const;
    virtual bool isConsistent(Event::SharedPtr event);

    virtual std::string toString(const std::vector<std::string> &names) const = 0;
    virtual std::string toString() const;

 protected:
    uint32_t event_no;
    char event_type;
    uint32_t crc32;
    bool iscrc32Ready;
};

#endif // EVENT_H
