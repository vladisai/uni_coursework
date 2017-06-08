#include <iostream>
#include <thread>

#include "ClientConnection.h"
#include "ClientConfig.h"
#include "GUIConnection.h"
#include "GameOverEvent.h"
#include "NewGameEvent.h"

using namespace std;

int main(int arg, char **argv) {
    GUIConnection guiConnection(ClientConfig::guiHost, ClientConfig::guiPortNumber);
    guiConnection.connect();
    while (true) {
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        cerr << "123" << endl;
        guiConnection.send(make_shared<NewGameEvent>(10, 10, 10, std::vector<std::string>({"p1", "p2"})));
    }
    return 0;
    /*ClientConnection connection(ClientConfig::serverHost, ClientConfig::serverPortNumber);
    if (!connection.connect()) {
        cerr << "FUCK" << endl;
    } else {
        connection.send(RawData({'a', 'b', 'c', 'e'}));
        cerr << "SENT" << endl;
        RawData data = connection.receiveData();
        for (auto e : data) {
            cerr << e << " " << int(e) << endl;
        }
    }*/
}
