#include <iostream>

#include "ServerConnection.h"
#include "ServerConfig.h"

int main(int arg, char **argv) {
    ServerConnection connection(ServerConfig::portNumber);
    if (!connection.connect()) {
        std::cerr << "error conncting\n";
    }

    for (int i = 0; i < 10; i++) {
        auto e = connection.receiveData();
        connection.send(e.first, RawData({'d', 'e', 'f', 'e'}));
        for (auto c : e.second) {
            std::cerr << c << " " << int(c) << std::endl;
        }
    }
}