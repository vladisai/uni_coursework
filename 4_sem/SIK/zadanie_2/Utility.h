#ifndef UTILITY_H
#define UTILITY_H

#include <netinet/in.h>
#include <cstdint>
#include "Serializer.h"
#include "ServerConnection.h"

bool operator==(const ServerConnection::ClientAddress &a1, const ServerConnection::ClientAddress &a2);

uint32_t computeCRC32(RawData data);

uint32_t reverse(unsigned x);

namespace RandomGenerator {
    extern uint64_t next;

    void setSeed(uint64_t seed);

    uint64_t getNext();
}


#endif // UTILITY_H
