#include <bits/stdc++.h>

#define F first
#define S second
#define pb push_back
#define INF (1 << 30)
#define SQR(a) ((a) * (a))

using namespace std;

class BinSemaphore {
public:
    BinSemaphore(int value = 0)
        : value(value) {}

    void P() {
        unique_lock<mutex> guard(mt);
        if (value == 0) {
            cv.wait(guard, [this] { return value == 1; });
        }
        value--;
    }

    void V() {
        unique_lock<mutex> guard(mt);
        if (value == 0) {
            value++;
            cv.notify_all();
        }
    }

private:
    int value;
    condition_variable cv;
    mutex mt;
};

template <class... Args>
void print(Args... args) {
    static mutex mut;
    mut.lock();
    (cout << ... << args) << endl;
    mut.unlock();
}

const int N = 3;
const int P = 5;

BinSemaphore m(1);
int waiting[2];
int watching[2];
int k = 2;
bool isWatched[N];
int waitingFilm[N][2];
BinSemaphore films[N][2];
BinSemaphore rooms[2];

void sleepRandom(int lim = 5000) {
    this_thread::sleep_for(chrono::milliseconds(rand() % lim));
}

int requestRoom(int film) {
    m.P();
    int ret = 0;
    if (isWatched[film]) {
        ret = (waiting[0] < waiting[1] ? 0 : 1);
    } else {
        ret = (watching[0] < watching[1] ? 0 : 1);
    }
    m.V();
    return ret;
}

void goToRoom(int id, int room) {
    print(id, " has entered room ", room);
}

void waitForYourTurn(int room, int film) {
    m.P();
    print(room, " ", film, " wait called");
    print(isWatched[film]);
    if (isWatched[film]) {
        waitingFilm[film][room]++;
        m.V();
        films[film][room].P();
        m.P();
        waitingFilm[film][room]--;
    }
    isWatched[film] = true;
    if (watching[room] == k) {
        waiting[room]++;
        m.V();
        rooms[room].P();
        m.P();
        waiting[room]--;
    }
    watching[room]++;
    m.V();
}

void watch(int id, int film, int room) {
    print(id, " is watching ", film, " in room ", room);
    sleepRandom();
}

void leave(int id, int room, int film) {
    print(id, " is about to leave ", room, " after watching ", film);
    m.P();
    watching[room]--;
    isWatched[film] = false;
    if (waiting[room]) {
        print(id, " is letting someone know room ", room, " is free");
        m.V();
        rooms[room].V();
        m.P();
    }
    if (waitingFilm[film][0] + waitingFilm[film][1]) {
        print(id, " is letting someone know that ", film, " is free");
        if (watching[0] < watching[1]) {
            m.V();
            films[film][0].V();
            m.P();
        } else {
            m.V();
            films[film][1].V();
            m.P();
        }
    }
    m.V();
    print(id, " left");
}

void process(int id) {
    while (true) {
        sleepRandom();
        int film = rand() % N;
        print(id, " has chosen film ", film);
        int room = requestRoom(film);
        print(id, " was directed to room ", room);
        goToRoom(id, room);
        waitForYourTurn(room, film);
        watch(id, film, room);
        leave(id, room, film);
    }
}

BinSemaphore bs;

void semcheck() {
    bs.P();    
    cout << 1 << endl;
    sleepRandom();
    cout << 1 << 2 << 3 << endl;
    bs.V();
}

int main() {
    srand(time(NULL));
    vector<thread> threads;
    for (int i = 0; i < P; i++)
        threads.push_back(thread([i] { process(i); }));
    for (auto &e : threads)
        e.join();
    return 0;
}
