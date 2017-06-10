#include <iostream>
#include <thread>
#include <atomic>
#include <set>
#include <bitset>

#include "ClientConnection.h"
#include "ClientConfig.h"
#include "GUIConnection.h"
#include "NewGameEvent.h"
#include "Fail.h"
#include "Utility.h"

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"
using namespace std;

std::vector<std::string> names;

uint64_t session_id = to_milliseconds(Clock::now().time_since_epoch());
uint32_t last_event = 0;
uint64_t game_id = 0;
bool isGameActive = false;

const int MX_EVENTS = 1e7;
bitset<MX_EVENTS> used;

void updateLast() {
    while (used[last_event] == 1) {
        last_event++;
    }
}

void keyboardEventSender(GUIConnection *guiConnection, ClientConnection *clientConnection) {
    auto last = Clock::now();
    while (true) {
        sleepUntil(last + ClientConfig::messageFrequency);
        last = Clock::now();
        try {
            cerr << "sending current key press " << endl;
            updateLast();
            ClientMessage m(session_id,
                            guiConnection->getCurrentDirection(),
                            last_event,
                            ClientConfig::playerName);
            cerr << "last event " << last_event << endl;
            clientConnection->send(m);
        } catch (...) {
            cerr << "Error reading from gui" << endl;
        }
    }
}

int main(int argc, const char **argv) {
    try {
        ClientConfig::loadFromArgs(argc, argv);
    } catch (...) {
        failWrongUsageClient();
    }
    GUIConnection guiConnection(ClientConfig::guiHost, ClientConfig::guiPortNumber);
    if (guiConnection.connect()) {
        failSysError("Couldn't connect to gui");
    }

    ClientConnection clientConnection(ClientConfig::serverHost, ClientConfig::serverPortNumber);
    if (!clientConnection.connect()) {
        failSysError("Couldn't connect to server");
    }

    std::thread(keyboardEventSender, &guiConnection, &clientConnection).detach();

    while (true) {
        cerr << "receiving from server" << endl;
        try {
            ServerMessage m = clientConnection.receiveMessage();
            cerr << "sending to gui" << endl;
            for (auto &e : m.getEvents()) {
                try {
                    cerr << "received " << e->toString() << endl;
                    if (isGameActive && m.getGameID() == game_id && used[e->getEventNo()]) {
                        cerr << "skipping" << endl;
                        continue;
                    }
                    if (e->getEventType() == EventType::NewGame) {
                        if (!isGameActive) {
                            auto n = dynamic_pointer_cast<NewGameEvent>(e);
                            isGameActive = true;
                            names = n->getPlayerNames();
                            game_id = m.getGameID();
                            last_event = n->getEventNo();
                            used.reset();
                            cerr << e->getEventNo();
                            used[e->getEventNo()] = 1;
                            guiConnection.send(e->toString(names));
                        } // ignore otherwise
                    } else if (game_id == m.getGameID()) {
                        if (e->getEventType() == EventType::GameOver) {
                            cerr << "Game over received" << endl;
                            isGameActive = false;
                            // not sending, gui doesn't know game over
                        } else {
                            guiConnection.send(e->toString(names));
                        }
                        used[e->getEventNo()] = 1;
                    }
                } catch (exception &e) {
                    cerr << e.what() << endl;
                }
            }
        } catch (exception &e) {
            cerr << e.what() << endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(400));
            continue;
        }
    }
    return 0;
}

#pragma clang diagnostic pop