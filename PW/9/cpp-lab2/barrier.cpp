#include <condition_variable>
#include <thread>
#include <vector>
#include "log.h"

using namespace std;

class Barrier {
public:
    Barrier(int resistance)
        : resistance(resistance), pressure(0) {}

    void reach() {
        log("reach: pressure = ", pressure, ", resistance = ", resistance);
        unique_lock<mutex> guard(lock);
        if (pressure >= resistance) return;
        pressure++;
        if (pressure < resistance) {
            breakingPoint.wait(guard, [this] { return pressure >= resistance; });
        } else if (pressure == resistance) {
            breakingPoint.notify_all();
        }
    }

    int getResistance() {
        return resistance;
    }

private:
    int resistance;
    int pressure;
    mutex lock;
    condition_variable breakingPoint;
};

mutex output;

void breakThrough(Barrier &b, int id) {
    this_thread::sleep_for(chrono::milliseconds(rand() % 3000));
    log("Thread ", id, " is calling reach");
    b.reach();
    log("Thread ", id, " has broken through");
}

int main() {
    srand(time(NULL));

    vector<thread> threads;
    Barrier b(5);
    for (int i = 0; i < 10; i++) {
        threads.push_back(thread([&b, i]() { breakThrough(b, i); }));
    }

    for (auto &e : threads)
        e.join();

    return 0;
}
