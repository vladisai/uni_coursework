// code adopted from https://bartoszmilewski.com/2011/10/10/async-tasks-in-c11-not-quite-there-yet/

#include <thread>
#include <future>
#include <vector>
#include <iostream>
#include <algorithm>
#include "../cpp-lab2/log.h"

int main() 
{
    std::cout << "Main thread id: " << std::this_thread::get_id() << std::endl;
    std::vector<std::future<void>> futures;
    for (int i = 0; i < 50000; ++i)
    {
        auto fut = std::async([i]
        {
            log("start task ", i, " ", std::this_thread::get_id()); // who am I?
            std::this_thread::sleep_for(std::chrono::seconds(1));
            log("finish task ", i, " ", std::this_thread::get_id()); // am I the same as before?
        });
        futures.push_back(std::move(fut));
    }
    std::for_each(futures.begin(), futures.end(), [](std::future<void> & fut)
    {
        fut.wait();
    });
    std::cout << std::endl;
}
