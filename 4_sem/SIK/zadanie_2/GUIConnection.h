#ifndef NETACKA_SIK_GUICONNECTION_H
#define NETACKA_SIK_GUICONNECTION_H

#include "Position.h"
#include "Event.h"

#include <string>
#include <mutex>
#include <thread>

class GUIConnection {
public:
    using SharedPtr = std::shared_ptr<GUIConnection>;

    GUIConnection(const std::string &host, const std::string &port);
    bool connect();

    Position::TurnDirection getCurrentDirection();

    bool close();

    void send(const std::string &str);

private:
    std::string host, port;
    int sock;
    std::mutex m;
    std::thread messageLoop;
    bool shouldStop = false;

    Position::TurnDirection currentDirection;

    static const std::string LEFT_KEY_DOWN_MSG;
    static const std::string LEFT_KEY_UP_MSG;
    static const std::string RIGHT_KEY_DOWN_MSG;
    static const std::string RIGHT_KEY_UP_MSG;

    void setCurrentDirection(Position::TurnDirection direction);

    static void receiveLoop(int sock, GUIConnection *connection);
};


#endif //NETACKA_SIK_GUICONNECTION_H
