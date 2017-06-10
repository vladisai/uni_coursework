#ifndef UTILITY_H
#define UTILITY_H

#include <netinet/in.h>
#include <cstdint>
#include <chrono>

#include "Serializer.h"
#include "ServerConnection.h"
#include "ServerConfig.h"

bool operator==(const ServerConnection::ClientAddress &a1, const ServerConnection::ClientAddress &a2);

uint32_t computeCRC32(RawData data);

uint32_t reverse(uint32_t x);

namespace RandomGenerator {
    extern uint64_t next;

    void init();

    void setSeed(uint64_t seed);

    uint64_t getNext();
}

using Clock = std::chrono::system_clock;

uint64_t to_milliseconds(Clock::duration d);

std::vector<RawData> compressMessages(uint32_t game_id, std::vector<Event::SharedPtr> event);

void sleepUntil(Clock::time_point t);

bool checkName(const std::string &s);

uint32_t parseUint32(const std::string &s);

#endif // UTILITY_H
