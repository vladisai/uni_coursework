#ifndef FAIL_H
#define FAIL_H

#include <cstring>
#include <iostream>
#include <string>
#include <cstdlib>
#include <errno.h>

extern const std::string usageClient;
extern const std::string usageServer;

void failWrongUsageClient();

void failWrongUsageServer();

void failErrorExit(const std::string& error);

void failSysError(const std::string& error);

void failSysErrorExit(const std::string& error);

class IllegalCommandLineArguments: public std::exception {
public:
    virtual const char *what() const throw() {
        return "Bad command line arguments";
    }
};

class BadCRC32Exception : public std::exception {
public:
    virtual const char *what() const throw() {
        return "CRC codes are different";
    }
};

class BadEventType : public std::exception {
public:
    int x;
    BadEventType(int x) : x(x) {}
    virtual const char *what() const throw() { return "Uknown event type"; }
};

class DeserializeError : public std::exception {
    virtual const char *what() const throw() { return "Serializer error"; }
};

class BadEventDataException : public std::exception {
    virtual const char *what() const throw() { return "Bad Event data"; }
};

class BadMessageDataException : public std::exception {
    virtual const char *what() const throw() { return "Bad Message data"; }
};

class ParsingError : public std::exception {
    virtual const char *what() const throw() { return "Parsing error"; }
};

#endif // FAIL_H
