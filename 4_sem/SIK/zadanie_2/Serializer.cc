#include "Serializer.h"

#include <arpa/inet.h>
#include "Fail.h"

Serializer::Serializer(RawData data) : data(data) { }

uint64_t Serializer::htonll(uint64_t value) {
    if (htonl(42) == 42) {
        return value;
    } else {
        uint32_t high_part = htonl((uint32_t)(value >> 32));
        uint32_t low_part = htonl((uint32_t)(value & 0xFFFFFFFFLL));
        return (((uint64_t)low_part) << 32) | high_part;
    }
}

uint64_t Serializer::ntohll(uint64_t value) {
    if (ntohl(42) == 42) {
        return value;
    } else {
        uint32_t high_part = htonl((uint32_t)(value >> 32));
        uint32_t low_part = htonl((uint32_t)(value & 0xFFFFFFFFLL));
        return (((uint64_t)low_part) << 32) | high_part;
    }
}

void Serializer::add(uint32_t value) {
    char buff[sizeof(value)];
    value = htonl(value);
    memcpy(buff, &value, sizeof(value));
    copy(buff, buff + sizeof(value), back_inserter(data));
}

void Serializer::add(uint64_t value) {
    char buff[sizeof(value)];
    value = htonll(value);
    memcpy(buff, &value, sizeof(value));
    copy(buff, buff + sizeof(value), back_inserter(data));
}

void Serializer::add(char value) {
    data.push_back(value);
}

void Serializer::add(char *value, uint32_t len) {
    copy(value, value + len, back_inserter(data));
}

void Serializer::add(const std::string &str) {
    copy(str.begin(), str.end(), back_inserter(data));
    data.push_back((char)0);
}

void Serializer::add(const RawData &data) {
    copy(data.begin(), data.end(), back_inserter(this->data));
}

void Serializer::add(const std::shared_ptr<Event> event) {
    add(event->serialize());
}

uint32_t Serializer::readUInt32() {
    if (data.size() < sizeof(uint32_t))
        throw DeserializeError();
    uint32_t value;
    memcpy(&value, &data[0], sizeof(uint32_t));
    value = ntohl(value);
    return value;
}

uint64_t Serializer::readUInt64() {
    if (data.size() < sizeof(uint64_t))
        throw DeserializeError();
    uint64_t value;
    memcpy(&value, &data[0], sizeof(uint64_t));
    value = ntohll(value);
    return value;
}

char Serializer::readChar() {
    if (data.size() < sizeof(char))
        throw DeserializeError();
    return data[0];
}

std::shared_ptr<Event> Serializer::readEvent() {
    uint32_t len = readUInt32() + 8;
    if (data.size() < len)
        throw DeserializeError();
    return Event::deserialize(RawData(data.begin(), data.begin() + len));
}

RawData Serializer::getData() const {
    return data;
}

bool Serializer::isEmpty() const {
    return data.empty();
}

uint32_t Serializer::popUInt32() {
    uint32_t value = readUInt32();
    eraseFront(sizeof(uint32_t));
    return value;
}

uint64_t Serializer::popUInt64() {
    uint64_t value = readUInt64();
    eraseFront(sizeof(uint64_t));
    return value;
}


char Serializer::popChar() {
    char value = readChar();
    eraseFront(sizeof(char));
    return value;
}

std::string Serializer::popString(size_t len) {
    if (data.size() < len)
        throw DeserializeError();
    std::string res(data.begin(), data.begin() + len);
    eraseFront(len);
    return res;
}

std::string Serializer::popString() {
    int clen = 0;
    auto it = data.begin();
    while (it != data.end() && *it != 0) {
        clen++;
        it++;
    }
    std::string ret = popString(clen);
    eraseFront(1); // 0 character
    return ret;
}

std::shared_ptr<Event> Serializer::popEvent() {
    auto ret = readEvent();
    uint32_t len = readUInt32() + 8;
    eraseFront(len);
    return ret;
}

void Serializer::eraseFront(size_t count) {
    if (data.size() < count)
        throw DeserializeError();
    data.erase(data.begin(), data.begin() + count);
}
