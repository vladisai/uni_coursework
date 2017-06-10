#include <thread>

#include "Utility.h"
#include "ServerConfig.h"
#include "CommonConfig.h"

uint32_t computeCRC32(RawData data) {
    int i, j;
    uint32_t byte, crc;

    i = 0;
    crc = 0xFFFFFFFF;
    for (size_t i = 0; i < data.size(); i++) {
        byte = data[i];            // Get next byte.
        byte = reverse(byte);         // 32-bit reversal.
        for (j = 0; j <= 7; j++) {    // Do eight times.
            if ((int)(crc ^ byte) < 0)
                crc = (crc << 1) ^ 0x04C11DB7;
            else crc = crc << 1;
            byte = byte << 1;          // Ready next msg bit.
        }
    }
    return reverse(~crc);
}

uint32_t reverse(uint32_t x) {
    x = ((x & 0x55555555) <<  1) | ((x >>  1) & 0x55555555);
    x = ((x & 0x33333333) <<  2) | ((x >>  2) & 0x33333333);
    x = ((x & 0x0F0F0F0F) <<  4) | ((x >>  4) & 0x0F0F0F0F);
    x = (x << 24) | ((x & 0xFF00) << 8) |
        ((x >> 8) & 0xFF00) | (x >> 24);
    return x;
}

uint64_t RandomGenerator::next = ServerConfig::randomGeneratorSeed;

void RandomGenerator::init() {
    RandomGenerator::setSeed(ServerConfig::randomGeneratorSeed);
}

void RandomGenerator::setSeed(uint64_t seed) {
    RandomGenerator::next = seed;
}

uint64_t RandomGenerator::getNext() {
    auto nxt = (RandomGenerator::next * 279470273) % 4294967291;
    auto cur = RandomGenerator::next;
    RandomGenerator::next = nxt;
    return cur;
}

static inline int ipv6_addr_cmp(const struct in6_addr *a1, const struct in6_addr *a2)
{
    return memcmp(a1, a2, sizeof(struct in6_addr));
}

bool operator==(const ServerConnection::ClientAddress &a1, const ServerConnection::ClientAddress &a2) {
    return a1.sin6_port == a2.sin6_port && ipv6_addr_cmp(&a1.sin6_addr, &a2.sin6_addr) == 0;
}

uint64_t to_milliseconds(Clock::duration d) {
    return std::chrono::duration_cast<std::chrono::milliseconds>(d).count();
}

std::vector<RawData> compressMessages(uint32_t game_id, std::vector<Event::SharedPtr> event) {
    RawData header = ServerMessage(game_id, {}).serialize();
    std::vector<RawData> ret;
    RawData current = header;

    for (auto e : event) {
        RawData next = e->serialize();
        if (current.size() + next.size() <= ServerConfig::maxMessageLen) {
            copy(next.begin(), next.end(), std::back_inserter(current));
        } else {
            ret.push_back(current);
            current = header;
            if (current.size() + next.size() <= ServerConfig::maxMessageLen) {
                copy(next.begin(), next.end(), std::back_inserter(current));
            }
        }
    }
    if (current.size() > 4)
        ret.push_back(current);
    return ret;
}

void sleepUntil(Clock::time_point t) {
    auto d = t - Clock::now();
    std::this_thread::sleep_for(d);
}

bool checkName(const std::string &s) {
    if (s.size() > CommonConfig::maxPlayerNameLength) {
        return false;
    }
    for (auto c : s) {
        if (int(c) < 33 || int(c) > 126) {
            return false;
        }
    }
    return true;
}

uint32_t parseUint32(const std::string &s) {
    if (s.size() > 9)
        throw ParsingError();
    for (auto c : s) {
        if (!isdigit(c))
            throw ParsingError();
    }
    return (uint32_t) std::stoul(s);
}
