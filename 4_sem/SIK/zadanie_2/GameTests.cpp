#include "gtest/gtest.h"

#include "Position.h"
#include "ClientConfig.h"
#include "ServerConfig.h"

TEST(GameTest, PositionTest) {
    Position p = Position::generate();
    EXPECT_LE(p.getDirection(), 360);
}

TEST(CLATest, ClientConfigTestTwoParams) {
    int argc = 3;
    std::string program = "test";
    std::string player = "playerName";
    std::string server = "host";
    const char* argv[] = {program.c_str(), player.c_str(), server.c_str()};
    ClientConfig::loadFromArgs(argc, argv);
    EXPECT_EQ(ClientConfig::playerName, player);
    EXPECT_EQ(ClientConfig::serverHost, server);
}

TEST(CLATest, ClientConfigTestTwoParamsWithPort) {
    int argc = 3;
    std::string program = "test";
    std::string player = "playerName";
    std::string server = "host";
    std::string port = "1234";
    const char* argv[] = {program.c_str(), player.c_str(), (server + ":" + port).c_str()};
    ClientConfig::loadFromArgs(argc, argv);
    EXPECT_EQ(ClientConfig::playerName, player);
    EXPECT_EQ(ClientConfig::serverHost, server);
    EXPECT_EQ(ClientConfig::serverPortNumber, port);
}

TEST(CLATest, ClientConfigTestFourParamsWithPort) {
    int argc = 4;
    std::string program = "test";
    std::string player = "playerName";
    std::string server = "host";
    std::string port = "1234";
    std::string guiServer = "127.0.0.16";
    std::string guiPort = "1254";
    const char* argv[] = {program.c_str(), player.c_str(), (server + ":" + port).c_str(),
                          (guiServer + ":" + guiPort).c_str()};
    ClientConfig::loadFromArgs(argc, argv);
    EXPECT_EQ(ClientConfig::playerName, player);
    EXPECT_EQ(ClientConfig::serverHost, server);
    EXPECT_EQ(ClientConfig::serverPortNumber, port);
    EXPECT_EQ(ClientConfig::guiHost, guiServer);
    EXPECT_EQ(ClientConfig::guiPortNumber, guiPort);
}

TEST(CLATest, ClientConfigFailTooFew) {
    int argc = 1;
    std::string program = "test";
    std::string player = "playerName";
    std::string server = "host";
    std::string port = "1234";
    std::string guiServer = "127.0.0.16";
    std::string guiPort = "1254";
    try {
        const char *argv[] = {program.c_str(), player.c_str(), (server + ":" + port).c_str(),
                              (guiServer + ":" + guiPort).c_str()};
        ClientConfig::loadFromArgs(argc, argv);
        FAIL() << "Expected exception";
    } catch (...) {

    }
}

TEST(CLATest, ClientConfigFailTooMany) {
    int argc = 5;
    std::string program = "test";
    std::string player = "playerName";
    std::string server = "host";
    std::string port = "1234";
    std::string guiServer = "127.0.0.16";
    std::string guiPort = "1254";
    try {
        const char *argv[] = {program.c_str(), player.c_str(), (server + ":" + port).c_str(),
                              (guiServer + ":" + guiPort).c_str()};
        ClientConfig::loadFromArgs(argc, argv);
        FAIL() << "Expected exception";
    } catch (...) {

    }
}

TEST(CLATest, ClientConfigEmptyName) {
    int argc = 4;
    std::string program = "test";
    std::string player = "\"\"";
    std::string server = "host";
    std::string port = "1234";
    std::string guiServer = "127.0.0.16";
    std::string guiPort = "1254";
    const char *argv[] = {program.c_str(), player.c_str(), (server + ":" + port).c_str(),
                          (guiServer + ":" + guiPort).c_str()};
    ClientConfig::loadFromArgs(argc, argv);
    EXPECT_EQ(ClientConfig::playerName, "");
}

TEST(CLATest, ServerConfigNoArgs) {
    int argc = 1;
    char *argv[] = {"test"};
    ServerConfig::loadFromArgs(argc, argv);
}

TEST(CLATest, ServerConfigEmptyValue) {
    int argc = 2;
    char *argv[] = {"test", "-H"};
    try {
        ServerConfig::loadFromArgs(argc, argv);
        FAIL() << "Expected exception";
    } catch (...) {}
}

TEST(CLATest, ServerConfigGoodOneValue) {
    int argc = 3;
    char *argv[] = {"test", "-H", "300"};
    ServerConfig::loadFromArgs(argc, argv);
    EXPECT_EQ(ServerConfig::planeHeight, 300);
}

TEST(CLATest, ServerConfigGoodAllValues) {
    int argc = 13;
    char *argv[] = {"test", "-H", "300", "-W", "324", "-p", "34", "-s", "30", "-t", "8", "-r", "34"};
    ServerConfig::loadFromArgs(argc, argv);
    EXPECT_EQ(ServerConfig::planeHeight, 300);
    EXPECT_EQ(ServerConfig::planeWidth, 324);
    EXPECT_EQ(ServerConfig::portNumber, 34);
    EXPECT_EQ(ServerConfig::gameSpeed, std::chrono::microseconds(1000000/30));
    EXPECT_EQ(ServerConfig::turningSpeed, 8);
    EXPECT_EQ(ServerConfig::randomGeneratorSeed, 34);
}

TEST(CLATest, ServerConfigGoodAllButOne) {
    int argc = 12;
    try {
        char *argv[] = {"test", "-H", "300", "-W", "-p", "34", "-s", "30", "-t", "8", "-r", "34"};
        ServerConfig::loadFromArgs(argc, argv);
        FAIL() << "Expected exception";
    } catch (...) {}
}
