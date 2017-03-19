#include <array>
#include <chrono>
#include <iostream>
#include <thread>
#include <atomic>

const long counter = 5'000'000;

using namespace std;

long x = 0;
atomic<int> waits{1};
array<atomic<int>, 2> wants;

void critical_section(void) {
    long y;
    y = x;
    y = y + 1;
    x = y;
    //std::this_thread::sleep_for(std::chrono::milliseconds(rand() % 1000));
}

void local_section(void) {}

void entry_protocol(int nr) {
    wants[nr] = true;
    int other = (nr + 1) % 2;
    waits = nr;
    while (waits == nr && wants[other])
        ;
}

void exit_protocol(int nr) { wants[nr] = false; }

void th(int nr) {
    for (long i = 0; i < counter; i++) {
        local_section();
        entry_protocol(nr);
        critical_section();
        exit_protocol(nr);
    }
}

void monitor() {
    long prev = 0;
    for (;;) {
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
        if (prev == x)
            std::cout << "Deadlock! wants = " << wants[0] << "/" << wants[1]
                      << " "
                      << "waits " << waits << std::endl;
        else
            std::cout << "monitor: " << x << std::endl;
    }
}

int main() {
    std::cout << "main() starts" << std::endl;
    std::thread monitor_th{monitor};
    monitor_th.detach();
    std::thread t1{th, 0};
    std::thread t2{th, 1};
    t1.join(); // wait for t1 to complete
    t2.join(); // wait for t2 to complete
    std::cout << "main() completes: " << x << std::endl;
}
