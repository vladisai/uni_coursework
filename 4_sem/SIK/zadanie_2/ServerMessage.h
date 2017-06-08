#include <vector>
#include <cstdint>

#include "Serializer.h"

class ServerMessage {
  public:
    ServerMessage(uint32_t game_id);

    ServerMessage(uint32_t game_id, std::vector<Event::SharedPtr> events);

    RawData serialize() const;

    static ServerMessage deserialize(const RawData &data);

    uint32_t getGameID() const;

    std::vector<Event::SharedPtr> getEvents() const;

    bool operator==(const ServerMessage &other) const;

  private:

    uint32_t game_id;

    std::vector<Event::SharedPtr> events;
};
