#include "gtest/gtest.h"

#include "ClientMessage.h"
#include "GameOverEvent.h"
#include "NewGameEvent.h"
#include "PixelEvent.h"
#include "PlayerEliminatedEvent.h"
#include "ServerMessage.h"

std::shared_ptr<NewGameEvent> constructTestNewGameEvent() {
    uint32_t event_no = 1343123555;
    uint32_t maxx = 10;
    uint32_t maxy = 14;
    std::vector<std::string> playerNames({"Vlados", "pat", "dfd"});
    return std::make_shared<NewGameEvent>(event_no, maxx, maxy, playerNames);
}

std::shared_ptr<GameOverEvent> constructTestGameOverEvent() {
    uint32_t event_no = 1343123555;
    return std::make_shared<GameOverEvent>(event_no);
}

std::shared_ptr<PlayerEliminatedEvent> constructTestPlayerEliminatedEvent() {
    uint32_t event_no = 1343123555;
    char playerNumber = 4;
    return std::make_shared<PlayerEliminatedEvent>(event_no, playerNumber);
}

std::shared_ptr<PixelEvent> constructTestPixelEvent() {
    uint32_t len = 0;
    uint32_t event_no = 1343123555;
    len += sizeof(event_no);
    char event_type = EventType::Pixel;
    len += sizeof(event_type);
    char playerNumber = 4;
    len += sizeof(playerNumber);
    uint32_t x = 10;
    len += sizeof(x);
    uint32_t y = 14;
    len += sizeof(y);

    return std::make_shared<PixelEvent>(event_no, playerNumber, x, y);
}

ServerMessage constructTestServerMessage() {
    uint32_t game_id = 5;
    std::vector<std::shared_ptr<Event>> events;
    events.push_back(constructTestNewGameEvent());
    return ServerMessage(game_id, events);
}

ClientMessage constructTestClientMessage() {
    uint64_t session_id = 12123341343ULL;
    char turn_direction = 1;
    uint32_t next_expected_event_no = 34343333;
    std::string playerName = "HERO";
    return ClientMessage(session_id, turn_direction, next_expected_event_no,
                         playerName);
}

TEST(SerializerTest, ServerMessageTest) {
    ServerMessage m1 = constructTestServerMessage();

    RawData data = m1.serialize();

    ServerMessage m2 = ServerMessage::deserialize(data);

    EXPECT_EQ(m1, m2);
}

TEST(SerializerTest, ClientMessageTest) {
    ClientMessage cm = constructTestClientMessage();

    RawData data = cm.serialize();

    ClientMessage ds = ClientMessage::deserialize(data);

    EXPECT_EQ(cm, ds);
}

TEST(SerializerTest, NewGameEventTest) {
    auto e1 = constructTestNewGameEvent();

    RawData data = e1->serialize();

    auto e2 = std::static_pointer_cast<NewGameEvent>(Event::deserialize(data));

    EXPECT_EQ(*e1, *e2);
}

TEST(SerializerTest, PixelEventTest) {
    auto event = constructTestPixelEvent();

    RawData data = event->serialize();

    std::shared_ptr<PixelEvent> n =
            std::static_pointer_cast<PixelEvent>(Event::deserialize(data));

    EXPECT_EQ(*event, *n);
}

TEST(SerializerTest, PlayerEliminatedEventTest) {
    auto event = constructTestPlayerEliminatedEvent();

    RawData data = event->serialize();

    std::shared_ptr<PlayerEliminatedEvent> n =
            std::static_pointer_cast<PlayerEliminatedEvent>(
                    Event::deserialize(data));

    EXPECT_EQ(*event, *n);
}

TEST(SerializerTest, GameOverEventTest) {
    auto event = constructTestGameOverEvent();

    RawData data = event->serialize();

    std::shared_ptr<GameOverEvent> n =
            std::static_pointer_cast<GameOverEvent>(Event::deserialize(data));

    EXPECT_EQ(*event, *n);
}

TEST(SerializerTest, PushPopTest) {
    Serializer s;
    uint64_t val1 = 3545;
    uint32_t val2 = 234;
    std::string val3 = "player asdfan";
    char val4 = 'a';
    s.add(val1);
    s.add(val2);
    s.add(val3);
    s.add(val4);
    uint64_t ret1 = s.popUInt64();
    uint32_t ret2 = s.popUInt32();
    std::string ret3 = s.popString();
    char ret4 = s.popChar();
    EXPECT_EQ(val1, ret1);
    EXPECT_EQ(val2, ret2);
    EXPECT_EQ(val3, ret3);
    EXPECT_EQ(val4, ret4);
}
