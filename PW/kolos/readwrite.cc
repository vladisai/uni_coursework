#include <bits/stdc++.h>

#define F first
#define S second
#define pb push_back
#define INF (1 << 30)
#define SQR(a) ((a) * (a))

using namespace std;

const int N = 1111;

void sleepRandom(int lim = 5000) {
    this_thread::sleep_for(chrono::milliseconds(rand() % lim));
}

template <class... Args>
void log(Args... args) {
    static mutex mut;
    mut.lock();
    (cout << ... << args) << endl;
    mut.unlock();
}

class Server {
public:
    void exclusive() {
        unique_lock<mutex> guard(mt);
        if (working > 0) {
            exclusiveWaiting++;
            exclusives.wait(guard, [this] { return working == 0; });
            exclusiveWaiting--;
        }
        working++;
        exclusiveWorking++;
    }

    void shared() {
        unique_lock<mutex> guard(mt);
        if (exclusiveWaiting > 0 || exclusiveWorking > 0) {
            sharedWaiting++;
            shareds.wait(guard, [this] {
                return exclusiveWorking + exclusiveWaiting == 0;
            });
            sharedWaiting--;
        }
        working++;
        sharedWorking++;
    }

    void change() {
        unique_lock<mutex> guard(mt);
        if (exclusiveWorking == 1) {
            exclusiveWorking--;
            working--;
            shareds.notify_all();
            working++;
            sharedWorking++;
        } else {
            working--;
            sharedWorking--;
            if (working > 0) {
                sharedChangers++;
                sharedSwitching.wait(guard, [this] { return working == 0; });
                sharedChangers--;
            }
            working++;
            exclusiveWorking++;
        }
    }

    void finish() {
        unique_lock<mutex> guard(mt);
        if (exclusiveWorking == 1) {
            exclusiveWorking--;
            working--;
            if (sharedChangers > 0) {
                sharedSwitching.notify_one();
            } else if (sharedWaiting > 0) {
                shareds.notify_all();
            } else if (exclusiveWaiting) {
                exclusives.notify_one();
            }
        } else {
            working--;
            sharedWorking--;
            if (working == 0 && sharedChangers > 0) {
                sharedSwitching.notify_one();
            }
            if (working == 0 && exclusiveWaiting > 0) {
                exclusives.notify_one();
            }
        }
    }

private:
    condition_variable exclusives;
    condition_variable shareds;
    condition_variable sharedSwitching;
    mutex mt;
    int working = 0;
    int sharedWaiting = 0;
    int exclusiveWaiting = 0;
    int sharedWorking = 0;
    int exclusiveWorking = 0;
    int sharedChangers = 0;
};

Server s;

void writer(int id) {
    while (true) {
        sleepRandom();
        log("writer ", id, " about to start its business");
        s.exclusive();
        log("writer ", id, " is writing");
        sleepRandom();
        s.finish();
        log("writer ", id, " is leaving");
    }
}

void reader(int id) {
    while (true) {
        sleepRandom();
        log("reader ", id, " about to start its business");
        s.shared();
        log("reader ", id, " is reading");
        sleepRandom();
        log("reader ", id, " is leaving");
        s.finish();
    }
}

void wtor(int id) {
    while (true) {
        sleepRandom();
        log("wtor ", id, " about to start its business");
        s.shared();
        log("wtor ", id, " is writing");
        log("wtor ", id, " is switching");
        s.change();
        log("wtor ", id, " switched");
        log("wtor ", id, " is reading");
        sleepRandom();
        log("wtor ", id, " is leaving");
        s.finish();
    }
}

void rtow(int id) {
    while (true) {
        sleepRandom();
        log("rtow ", id, " about to start its business");
        s.shared();
        log("rtow ", id, " is reading");
        log("rtow ", id, " is switching");
        s.change();
        log("rtow ", id, " switched");
        log("rtow ", id, " is writing");
        sleepRandom();
        log("rtow ", id, " is leaving");
        s.finish();
    }
}

int main() {
    vector<thread> threads;
    for (int i = 0; i < 5; i++)
        threads.push_back(thread([i] { writer(i); }));
    for (int i = 0; i < 5; i++)
        threads.push_back(thread([i] { reader(i); }));
    for (int i = 0; i < 5; i++)
        threads.push_back(thread([i] { rtow(i); }));
    for (int i = 0; i < 5; i++)
        threads.push_back(thread([i] { wtor(i); }));

    for (auto &t : threads)
        t.join();

    return 0;
}
