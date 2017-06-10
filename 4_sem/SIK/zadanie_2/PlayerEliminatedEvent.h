#ifndef PLAYERELIMINATEDEVENT_H
#define PLAYERELIMINATEDEVENT_H

#include "Event.h"

class PlayerEliminatedEvent : public Event {
  public:
    PlayerEliminatedEvent(uint32_t event_no, char playerNumber);

    virtual RawData serialize(bool noCRC = false) override;

    virtual std::string toString(const std::vector<std::string> &data) const override;

    static std::shared_ptr<PlayerEliminatedEvent> deserialize(RawData data);

    char getPlayerNumber();

    bool operator == (const PlayerEliminatedEvent &other) const;

    virtual bool isConsistent(Event::SharedPtr event) override;

  private:
    char playerNumber;
};

#endif // PLAYERELIMINATEDEVENT_H
