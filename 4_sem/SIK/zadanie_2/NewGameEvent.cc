#include "NewGameEvent.h"

#include <exception>
#include <sstream>

#include "Utility.h"
#include "CommonConfig.h"

using namespace std;

RawData NewGameEvent::serialize(bool noCRC) {
    Serializer s;
    uint32_t len = 0;
    len += sizeof(event_no);
    len += sizeof(event_type);
    len += sizeof(maxx);
    len += sizeof(maxx);
    for (auto e : playerNames) {
        len += e.size() + 1; // + 1 for the \0 at the end
    }
    s.add(len);
    s.add(event_no);
    s.add(event_type);
    s.add(maxx);
    s.add(maxy);
    for (auto e : playerNames) {
        s.add(e);
    }
    if (!noCRC) {
        s.add(getCRC32());
    }
    return s.getData();
}

std::shared_ptr<NewGameEvent> NewGameEvent::deserialize(RawData data) {
    Serializer s(data);
    uint32_t len = s.popUInt32();
    uint32_t event_no = s.popUInt32();
    char event_type = s.popChar();
    uint32_t maxx = s.popUInt32();
    uint32_t maxy = s.popUInt32();
    if (maxx < 1) throw BadEventDataException();
    if (maxy < 1) throw BadEventDataException();
    len -= sizeof(event_no);
    len -= sizeof(event_type);
    len -= sizeof(maxx);
    len -= sizeof(maxy);
    std::vector<std::string> names;
    while (s.getData().size() > 4) {
        std::string next = s.popString();
        if (!checkName(next))
            throw BadEventDataException();
        len -= next.size() + 1; // + 1 for the \0 at the end
        names.push_back(next);
    }
    if (names.size() < CommonConfig::minPlayersNumber) {
        throw BadEventDataException();
    }
    uint32_t crc32 = s.popUInt32();
    auto ret = std::make_shared<NewGameEvent>(event_no, maxx, maxy, names);
    if (ret->getCRC32() != crc32) {
        throw BadCRC32Exception();
    }
    return ret;
}

NewGameEvent::NewGameEvent(uint32_t event_no, uint32_t maxx, uint32_t maxy,
                           std::vector<std::string> playerNames)
    : Event(event_no, EventType::NewGame), maxx(maxx), maxy(maxy),
      playerNames(playerNames) {
    if (playerNames.size() < CommonConfig::minPlayersNumber) {
        throw BadEventDataException();
    }
    for (auto &e : playerNames) {
        if (!checkName(e))
            throw BadEventDataException();
    }
}

uint32_t NewGameEvent::getMaxX() const { return maxx; }

uint32_t NewGameEvent::getMaxY() const { return maxy; }

std::vector<std::string> NewGameEvent::getPlayerNames() const {
    return playerNames;
}

bool NewGameEvent::operator == (const NewGameEvent &other) const {
    return (Event::operator==(other)
            && maxx == other.maxx
            && maxy == other.maxy
            && playerNames == other.playerNames);
}

std::string NewGameEvent::toString(const std::vector<std::string> &names) const {
    stringstream ss;
    ss << "NEW_GAME " << maxx << " " << maxy;
    for (auto e : playerNames)
        ss << " " << e;
    ss << std::endl;
    return ss.str();
}
