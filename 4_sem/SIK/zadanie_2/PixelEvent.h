#ifndef PIXELEVENT_H
#define PIXELEVENT_H

#include <memory>

#include "Event.h"

class PixelEvent : public Event {
  public:
    PixelEvent(uint32_t event_no, char playerNumber, uint32_t x, uint32_t y);

    virtual RawData serialize(bool noCRC = false) override;

    virtual std::string toString(const std::vector<std::string> &names) const override;

    static std::shared_ptr<PixelEvent> deserialize(RawData data);

    uint32_t getX();

    uint32_t getY();

    char getPlayerNumber();

    bool operator == (const PixelEvent &other) const;

    virtual bool isConsistent(Event::SharedPtr event) override;

  private:
    uint32_t x;
    uint32_t y;
    char playerNumber;
};

#endif // PIXELEVENT_H
