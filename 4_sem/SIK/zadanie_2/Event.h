#ifndef EVENT_H
#define EVENT_H

#include <memory>
#include <vector>

class Event;

#include "Serializer.h"

class BadCRC32Exception : public std::exception {
  public:
    virtual const char *what() const throw() {
        return "CRC codes are different";
    }
};

class BadEventType : public std::exception {
  public:
    int x;
    BadEventType(int x) : x(x) {}
    virtual const char *what() const throw() { return "Uknown event type"; }
};

enum EventType {
    NewGame = 0,
    Pixel = 1,
    PlayerEliminated = 2,
    GameOver = 3,
};

class Event {
  public:
    using SharedPtr = std::shared_ptr<Event>;
    Event(unsigned event_no, char event_type);

    virtual RawData serialize(bool noCRC = false) = 0;

    static std::shared_ptr<Event> deserialize(RawData data);

    unsigned getCRC32();

    unsigned getEventNo() const;

    char getEventType() const;

    virtual bool operator==(const Event &other) const;

    virtual std::string toString(const std::vector<std::string> &names) const = 0;

 protected:
    unsigned event_no;
    char event_type;
    unsigned crc32;
    bool iscrc32Ready;
};

#endif // EVENT_H
