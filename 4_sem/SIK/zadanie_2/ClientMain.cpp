#include <iostream>
#include <thread>
#include <atomic>
#include <set>
#include <bitset>
#include <csignal>

#include "ClientConnection.h"
#include "ClientConfig.h"
#include "GUIConnection.h"
#include "NewGameEvent.h"
#include "Fail.h"
#include "Utility.h"
#include "CommonConfig.h"

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"
using namespace std;

std::vector<std::string> names;

uint64_t session_id = to_milliseconds(Clock::now().time_since_epoch());
uint32_t last_event = 0;
uint64_t game_id = 0;
bool isGameActive = false;
NewGameEvent::SharedPtr lastGame;
bool stop = false;

const int MX_EVENTS = 1e7;
bitset<MX_EVENTS> used;

void updateLast() {
    while (used[last_event] == 1) {
        last_event++;
    }
}

void interrupt(int)
{
    stop = true;
    std::cerr << "stopping" << std::endl;
}

void keyboardEventSender(GUIConnection *guiConnection, ClientConnection *clientConnection) {
    auto last = Clock::now();
    while (!stop) {
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
    guiConnection.connect();

    ClientConnection clientConnection(ClientConfig::serverHost, ClientConfig::serverPortNumber);
    clientConnection.connect();

    if (signal(SIGINT, interrupt) == SIG_ERR) {
        failSysErrorExit("Unable to change signal handler");
    }

    auto keyboardThread = std::thread(keyboardEventSender, &guiConnection, &clientConnection);

    while (!stop) {
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
                            if (n->getPlayerNames().size() < CommonConfig::minPlayersNumber)
                                continue;
                            if (n->getMaxX() == 0)
                                continue;
                            if (n->getMaxY() == 0)
                                continue;
                            lastGame = n;
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
                            if (!e->isConsistent(lastGame)) {
                                failErrorExit("Bad event received");
                                break;
                            }
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

    keyboardThread.join();

    cerr << "closing connection" << endl;
    guiConnection.close();
    clientConnection.disconnect();

    return 0;
}

#pragma clang diagnostic pop