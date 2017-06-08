#include <thread>

#include "Utility.h"

unsigned int computeCRC32(RawData data) {
   int i, j;
   unsigned int byte, crc;

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

unsigned reverse(unsigned x) {
   x = ((x & 0x55555555) <<  1) | ((x >>  1) & 0x55555555);
   x = ((x & 0x33333333) <<  2) | ((x >>  2) & 0x33333333);
   x = ((x & 0x0F0F0F0F) <<  4) | ((x >>  4) & 0x0F0F0F0F);
   x = (x << 24) | ((x & 0xFF00) << 8) |
       ((x >> 8) & 0xFF00) | (x >> 24);
   return x;
}

uint64_t RandomGenerator::next = 0;

void RandomGenerator::setSeed(uint64_t seed) {
    RandomGenerator::next = seed;
}

uint64_t RandomGenerator::getNext() {
    RandomGenerator::next = (RandomGenerator::next * 279470273) % 4294967291;
    return RandomGenerator::next;
}

bool operator==(const ServerConnection::ClientAddress &a1, const ServerConnection::ClientAddress &a2) {
   return true; // TODO
}

