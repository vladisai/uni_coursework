#ifndef SERIALIZER_H
#define SERIALIZER_H

#include <cassert>
#include <cstdint>
#include <queue>
#include <string>
#include <memory>

using RawData = std::deque<char>;

#include "Event.h"

class Serializer {
  public:
    Serializer(RawData data = RawData());

    void add(uint32_t value);
    void add(uint64_t value);
    void add(char value);
    void add(char *value, uint32_t len);
    void add(const std::string &str);
    void add(const RawData &data);
    void add(const std::shared_ptr<Event> event);

    RawData getData() const;
    bool isEmpty() const;

    uint32_t readUInt32();
    uint64_t readUInt64();
    char readChar();
    std::shared_ptr<Event> readEvent();

    uint32_t popUInt32();
    uint64_t popUInt64();
    char popChar();
    std::string popString(size_t len);
    std::string popString();
    std::shared_ptr<Event> popEvent();

  private:
    RawData data;

    uint64_t htonll(uint64_t value);
    uint64_t ntohll(uint64_t value);

    void eraseFront(size_t count);
};

#endif // SERIALIZER_H
