#include <thread>
#include <iostream>
#include <chrono>

thread_local int counter = 0;

void f() {
    std::cout << "f() starts" << std::endl;
    for (int i = 0; i < 1'000'000; i++) {
        // look, ma, no mutex!
        int local = counter;
        local += 1;
        counter = local;
    }
    std::cout << "f() completes: counter=" << counter << std::endl;
}

int main() {
    std::cout << "main() starts" << std::endl;
    std::thread t1{f};
    std::thread t2{f};
    t1.join();
    t2.join();
    std::cout << "main() completes" << std::endl;
}
