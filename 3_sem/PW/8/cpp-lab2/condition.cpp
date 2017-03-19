// this code is adopted from
// http://en.cppreference.com/w/cpp/thread/condition_variable/wait
#include <array>
#include <chrono>
#include <condition_variable>
#include <iostream>
#include <thread>
#include "log.h"

std::condition_variable cv;
std::mutex cv_m;  // This mutex is used for two purposes:
                  // 1) to synchronize accesses to i
                  // 2) for the condition variable cv
int i = 0;

void waits(int id) {
    if (id == 2)
        std::this_thread::sleep_for(std::chrono::milliseconds(500));

    std::unique_lock<std::mutex> lk(cv_m);
    log("Waiting...", id);
    cv.wait(lk,
            [] { return i == 1; });  // we use a predicate: wait ends iff i == 1
    log("Finished waiting. i == ", i, id);
}

void signals() {
    std::this_thread::sleep_for(std::chrono::seconds(1));
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        std::lock_guard<std::mutex> lk(cv_m);
        i = 0;
        cv.notify_all();  
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        // simulate processing that changes i
    }
    log("Notifying...");
    cv.notify_all();  // we don't have mutex here!
    std::this_thread::sleep_for(std::chrono::seconds(1));
    {
        std::lock_guard<std::mutex> lk(cv_m);
        i = 1;  // simulate processing that changes i
    }
    log("Notifying again...");
    cv.notify_all();  // we don't have mutex here!
}

int main() {
    std::array<std::thread, 4> threads = {
        std::thread([](){waits(1);}), std::thread([](){waits(2);}), std::thread([](){waits(3);}),
        std::thread(signals)};
    for (auto& t : threads)
        t.join();
}
