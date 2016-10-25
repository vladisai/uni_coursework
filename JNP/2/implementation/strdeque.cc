extern "C" {
#include "strdeque.h"
}

#include <deque>
#include <iostream>
#include <map>
#include <sstream>
#include <stack>
#include <string>
#include <vector>
#include <cstdlib>

using std::vector;
using std::deque;
using std::string;
using std::stack;
using std::cerr;
using std::endl;
using std::to_string;
using std::stringstream;

vector<deque<string>*> v = { new deque<string>() };
stack<int> unused;

void logMessage(const string& message)
{
#ifndef NDEBUG
    cerr << message << endl;
#endif
}

void logMessage(const string& functionName, const string& message)
{
#ifndef NDEBUG
    cerr << functionName << ": " << message << endl;
#endif
}

stringstream& joinWithComma(stringstream& ss)
{
    return ss;
}

template <typename T>
stringstream& joinWithComma(stringstream& ss, const T& v)
{
    ss << v;
    return ss;
}

template <typename T, typename... Args>
stringstream& joinWithComma(stringstream& ss, const T& v, Args... args)
{
    ss << v << ", ";
    joinWithComma(ss, args...);
    return ss;
}

template <typename... Args>
void logFunctionCall(string name, Args... args)
{
    stringstream ss;
    joinWithComma(ss, args...);
    string message = name + "(" + ss.str() + ")";
    logMessage(message);
}

bool exists(unsigned long id)
{
    if (id >= v.size()) {
        return false;
    }
    if (v[id] == nullptr) {
        return false;
    }
    return true;
}

bool is_mutable(unsigned long id)
{
    if (id == 0) {
        return false;
    } else {
        return exists(id);
    }
}

bool check_and_maybe_log_accessibility(string name, unsigned long id)
{
    if (!exists(id)) {
        logMessage(name, "Attempt to access an unexisting deque. "
                         "Ignored.");
        return false;
    }
    return true;
}

bool check_and_maybe_log_mutability(string name, unsigned long id)
{
    if (!exists(id)) {
        logMessage(name, "Attempt to modify an unexisting deque. "
                         "Ignored.");
        return false;
    }
    if (!is_mutable(id)) {
        logMessage(name, "Attempt to modify an immutable deque. "
                         "Ignored.");
        return false;
    }
    return true;
}

extern "C" {

unsigned long strdeque_new()
{
    logFunctionCall(__func__);
    if (!unused.empty()) {
        long id = unused.top();
        unused.pop();
        v[id] = new deque<string>();
        return id;
    } else {
        if (v.empty()) {
            v.push_back(new deque<string>());
        }
        v.push_back(new deque<string>());
        return v.size() - 1;
    }
}

void strdeque_delete(unsigned long id)
{
    logFunctionCall(__func__, id);
    if (!check_and_maybe_log_mutability(__func__, id))
        return;
    if (id != 0) {
        delete v[id];
        v[id] = nullptr;
        unused.push(id);
    }
}

size_t strdeque_size(unsigned long id)
{
    logFunctionCall(__func__, id);
    if (!check_and_maybe_log_accessibility(__func__, id))
        return 0;
    return v[id]->size();
}

void strdeque_insert_at(unsigned long id, size_t pos, const char* value)
{
    logFunctionCall(__func__, id, pos, value);
    if (!check_and_maybe_log_mutability(__func__, id))
        return;
    if (value == 0) {
        logMessage(__func__, "Attempt to insert a NULL value. Ignored.");
        return;
    }
    auto it = v[id]->begin() + pos;
    if (it > v[id]->end()) {
        logMessage(__func__, "Index is out of bounds. "
                             "Appending to the end of the deque.");
        it = v[id]->end();
    }
    v[id]->insert(it, string(value));
}

void strdeque_remove_at(unsigned long id, size_t pos)
{
    logFunctionCall(__func__, id, pos);
    if (!check_and_maybe_log_mutability(__func__, id))
        return;
    auto it = v[id]->begin() + pos;
    if (it >= v[id]->end()) {
        logMessage(__func__, "Index is out of bounds.");
        return;
    }
    v[id]->erase(it);
}

const char* strdeque_get_at(unsigned long id, size_t pos)
{
    logFunctionCall(__func__, id, pos);
    if (!check_and_maybe_log_accessibility(__func__, id))
        return NULL;
    auto it = v[id]->begin() + pos;
    if (it >= v[id]->end()) {
        logMessage(__func__, "Index is out of bounds.");
        return NULL;
    }
    return it->c_str();
}

void strdeque_clear(unsigned long id)
{
    logFunctionCall(__func__, id);
    if (!check_and_maybe_log_mutability(__func__, id))
        return;
    v[id]->clear();
}

int strdeque_comp(unsigned long id1, unsigned long id2)
{
    logFunctionCall(__func__, id1, id2);
    if (!exists(id1)) {
        logMessage(__func__, "First deque index is out of bounds. "
                             "Falling back to the empty deque.");
        id1 = 0;
    }
    if (!exists(id2)) {
        logMessage(__func__, "Second deque index is out of bounds. "
                             "Falling back to the empty deque.");
        id2 = 0;
    }
    if (*v[id1] < *v[id2]) {
        return -1;
    } else if (*v[id1] == *v[id2]) {
        return 0;
    } else {
        return 1;
    }
}
}
