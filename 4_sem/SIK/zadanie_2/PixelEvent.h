#ifndef PIXELEVENT_H
#define PIXELEVENT_H

#include <memory>

#include "Event.h"

class PixelEvent : public Event {
  public:
    PixelEvent(unsigned event_no, char playerNumber, unsigned x, unsigned y);

    virtual RawData serialize(bool noCRC = false) override;

    virtual std::string toString(const std::vector<std::string> &names) const override;

    static std::shared_ptr<PixelEvent> deserialize(RawData data);

    unsigned getX();

    unsigned getY();

    char getPlayerNumber();

    bool operator == (const PixelEvent &other) const;

    virtual bool isConsistent(Event::SharedPtr event) override;

  private:
    unsigned x;
    unsigned y;
    char playerNumber;
};

#endif // PIXELEVENT_H
