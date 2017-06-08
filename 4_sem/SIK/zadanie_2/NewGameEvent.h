#ifndef NEWGAMEEVENT_H
#define NEWGAMEEVENT_H

#include "Event.h"

class NewGameEvent : public Event {
  public:
    NewGameEvent(unsigned event_no, unsigned maxx, unsigned maxy,
                 std::vector<std::string> names);


    virtual RawData serialize(bool noCRC = false) override;

    virtual std::string toString(const std::vector<std::string> &names) const override;

    static std::shared_ptr<NewGameEvent> deserialize(RawData data);

    unsigned getMaxX() const;
    unsigned getMaxY() const;
    std::vector<std::string> getPlayerNames() const;

    bool operator == (const NewGameEvent &other) const;

  private:
    unsigned maxx;
    unsigned maxy;
    std::vector<std::string> playerNames;
};

#endif // NEWGAMEEVENT_H
