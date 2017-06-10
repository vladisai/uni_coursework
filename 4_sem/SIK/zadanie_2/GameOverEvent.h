#ifndef GAMEOVEREVENT_H
#define GAMEOVEREVENT_H

#include "Event.h"

struct GameOverEvent : public Event {
    GameOverEvent(uint32_t event_no);

    virtual RawData serialize(bool noCRC = false) override;

    virtual std::string toString(const std::vector<std::string> &names) const override;

    static std::shared_ptr<GameOverEvent> deserialize(RawData data);

    bool operator == (const GameOverEvent &other) const;
};

#endif // GAMEOVEREVENT_H
