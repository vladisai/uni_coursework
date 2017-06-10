#include <algorithm>

#include "ClientConfig.h"
#include "Fail.h"
#include "Utility.h"

using std::chrono::milliseconds;

std::string ClientConfig::serverPortNumber = "12345";
std::string ClientConfig::serverHost = "localhost";
std::string ClientConfig::guiPortNumber = "12346";
std::string ClientConfig::guiHost = "localhost";
std::string ClientConfig::playerName = "player";
milliseconds ClientConfig::messageFrequency = milliseconds(20);

void parseAddress(std::string s, std::string &host, std::string &port) {
    auto sem = std::count(s.begin(), s.end(), ':');
    if (sem == 0 || sem > 1) {
        host = s;
        return;
    } else {
        auto it = s.find(':');
        host = s.substr(0, it);
        port = s.substr(it + 1);
    }
}

void ClientConfig::loadFromArgs(int argc, const char **argv) {
    if (argc < 3 || argc > 4) throw IllegalCommandLineArguments();
    playerName = argv[1];
    if (playerName == "\"\"") {
        playerName = "";
    }
    if (!checkName(playerName))
        throw IllegalCommandLineArguments();
    parseAddress(argv[2], serverHost, serverPortNumber);
    if (argc == 4) {
        parseAddress(argv[3], guiHost, guiPortNumber);
    }
}

