#include "ServerMessage.h"

ServerMessage::ServerMessage(uint32_t game_id) :
        ServerMessage(game_id, std::vector<std::shared_ptr<Event>>()) {}

ServerMessage::ServerMessage(uint32_t game_id, std::vector<std::shared_ptr<Event>> events) :
        game_id(game_id), events(events) {}

uint32_t ServerMessage::getGameID() const {
    return game_id;
}

std::vector<std::shared_ptr<Event>> ServerMessage::getEvents() const {
    return events;
}

RawData ServerMessage::serialize()const {
    Serializer s;
    s.add(game_id);
    for (auto e : events) {
        s.add(e);
    }
    return s.getData();
}

ServerMessage ServerMessage::deserialize(const RawData &data) {
    Serializer s(data);
    uint32_t game_id = s.popUInt32();
    std::vector<std::shared_ptr<Event>> events;
    while (!s.isEmpty()) {
        auto event = s.popEvent();
        events.push_back(event);
    }
    return ServerMessage(game_id, events);
}

bool ServerMessage::operator==(const ServerMessage &other) const {
    if (game_id != other.game_id) {
        return false;
    }
    if (events.size() != other.events.size()) {
        return false;
    }
    for (size_t i = 0; i < events.size(); i++) {
        if (!(*events[i] == *other.events[i])) {
            return false;
        }
    }
    return true;
}
