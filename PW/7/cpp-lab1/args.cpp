#include <thread>
#include <iostream>
#include <chrono>

void f(int timeout) {
    std::cout << "f() starts; my arg is " << timeout << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(timeout));
    std::cout << "f() completes; my arg is " << timeout << std::endl;
}

int main() {
    std::cout << "main() starts" << std::endl;
    std::thread t1{f, 2}; // thread arguments as constructor arguments
    std::thread t2{[]{ f(5); }}; // []{ f(5);} is a lambda (anonymous function) that executes f(5)
    t1.join();
    t2.join(); 
    std::cout << "main() completes" << std::endl;
}
