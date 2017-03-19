/**
 * simple pretty-printing library for MIM concurrent computing classes
 * code adopted from http://stackoverflow.com/questions/18277304/using-stdcout-in-multiple-threads
 **/


// FIXME: this doesn't work: output is still mixed!

#include <string>
#include <mutex>
#include <iostream>

std::ostream&
print_one(std::ostream& os)
{
    os << std::endl;
    return os;
}

template <class A0, class ...Args>
std::ostream&
print_one(std::ostream& os, const A0& a0, const Args& ...args)
{
    os << a0;
    return print_one(os, args...);
}

template <class ...Args>
std::ostream&
log(std::ostream& os, const Args& ...args)
{
    return print_one(os, args...);
}

template <class ...Args>
std::ostream&
log(const Args& ...args)
{
    static std::mutex m;
    std::lock_guard<std::mutex> lock(m);
    return log(std::cout, args...);
}
