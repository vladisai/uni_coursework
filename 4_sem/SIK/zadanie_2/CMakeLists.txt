cmake_minimum_required(VERSION 3.7)
project(netacka_sik)

SET(CMAKE_BUILD_TYPE Debug)

add_library(Serializer Serializer.cc)

add_library(Event Event.cc)

add_library(NewGameEvent NewGameEvent.cc)

add_library(PixelEvent PixelEvent.cc)

add_library(PlayerEliminatedEvent PlayerEliminatedEvent.cc)

add_library(GameOverEvent GameOverEvent.cc)

add_library(ClientMessage ClientMessage.cc)

add_library(ServerMessage ServerMessage.cc)

add_library(Position Position.cpp)

add_library(ServerConfig ServerConfig.cpp)

add_library(ClientConnection ClientConnection.cpp)

add_library(ServerConnection ServerConnection.cc)

add_library(Client Client.cpp)

add_library(Game Game.cpp)

add_library(GUIConnection GUIConnection.cpp)

add_library(ClientConfig ClientConfig.cpp)

target_link_libraries(Serializer Event)
target_link_libraries(ClientMessage Serializer)
target_link_libraries(Event NewGameEvent PixelEvent Utility GameOverEvent PlayerEliminatedEvent)

add_library(Utility Utility.cc)

set(ALL_LIBS
    Utility
    ServerConfig
    ClientConfig
    Event
    Serializer
    Client
    Game
    ClientConnection
    ServerConnection
    GUIConnection
    ClientMessage 
    ServerMessage
    GameOverEvent
    NewGameEvent
    PixelEvent
    PlayerEliminatedEvent
    Position
    pthread
)

add_library(SerializerTests SerializerTests.cpp)
set(ALL_TESTS
        SerializerTests)

enable_testing()
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIR})

# Specify include dir
set(TARGET netacka)
set(GTEST_TARGET ${TARGET}_GTest)

set(TARGET_SERVER server)
add_executable(${TARGET_SERVER} ServerMain.cpp)
target_link_libraries(${TARGET_SERVER} ${ALL_LIBS})

set(TARGET_CLIENT client)
add_executable(${TARGET_CLIENT} ClientMain)
target_link_libraries(${TARGET_CLIENT} ${ALL_LIBS})

add_executable(${GTEST_TARGET} test.cc)
target_link_libraries(${GTEST_TARGET} ${ALL_LIBS} ${GTEST_LIBRARY} ${ALL_TESTS})
