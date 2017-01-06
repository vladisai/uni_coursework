#include <bits/stdc++.h>

#define F first
#define S second
#define pb push_back
#define INF (1 << 30)
#define SQR(a) ((a) * (a))

using namespace std;

using deltaT = unordered_map<int, double>;

unordered_map<int, vector<int>> edges;
set<int> vertices;
deltaT bc;

unordered_map<int, double> processVertex(int s) {
    unordered_map<int, vector<int>> p;
    deltaT delta, sigma;
    unordered_map<int, int> d;
    stack<int> st;

    for (auto w : vertices) {
        sigma[w] = 0;
        d[w] = -1;
        delta[w] = 0;
        p[w] = vector<int>();
    }
    sigma[s] = 1;
    d[s] = 0;
    queue<int> q;
    q.push(s);
    while (!q.empty()) {
        auto t = q.front();
        q.pop();
        st.push(t);
        for (auto x : edges[t]) {
            if (d[x] < 0) {
                q.push(x);
                d[x] = d[t] + 1;
            }
            if (d[x] == d[t] + 1) {
                sigma[x] += sigma[t];
                p[x].push_back(t);
            }
        }
    }
    while (!st.empty()) {
        auto w = st.top();
        st.pop();
        for (auto t : p[w]) {
            delta[t] += (sigma[t] / sigma[w]) * (1 + delta[w]);
        }
    }
    delta[s] = 0;
    return delta;
}

unordered_map<int, double> processBatch(vector<int> batch) {
    unordered_map<int, double> result;
    unordered_map<int, double> compensation;
    for (auto v : batch) {
        auto res = processVertex(v);
        for (auto e : res) {
            int key = e.first;
            double addedValue = e.second;
            double &currentValue = result[key];
            double &currentCompensation = compensation[key];
            double y = addedValue - currentCompensation;
            double t = currentValue + y;
            currentCompensation = (t - currentValue) - y;
            currentValue = t;
        }
    }
    return result;
}

/*unordered_map<int, double> processBatch(vector<int> batch) {
    unordered_map<int, double> result;
    unordered_map<int, double> error;
    for (auto v : batch) {
        auto res = processVertex(v);
        for (auto e : res) {
            int key = e.first;
            double value = e.second;
            result[key] += value;
        }
    }
    return result;
}*/

int main(int argc, char *argv[]) {

    assert(argc == 4);

    int threadsNumber = atoi(argv[1]);

    freopen(argv[2], "r", stdin);
    freopen(argv[3], "w", stdout);

    cerr << threadsNumber << endl;

    int x, y;
    while (cin >> x >> y) {
        vertices.insert(x);
        vertices.insert(y);
        edges[x].push_back(y);
        edges[y].push_back(x);
    }

    vector<future<deltaT>> futures;

    vector<int> nextBatch;
    size_t amountPerBatch = (vertices.size() + threadsNumber - 1) / threadsNumber;
    for (auto v : vertices) {
        nextBatch.push_back(v);
        if (nextBatch.size() == amountPerBatch) {
            futures.push_back(async(launch::async, processBatch, nextBatch));
            nextBatch.clear();
        }
    }

    if (nextBatch.size() > 0) {
        futures.push_back(async(launch::async, processBatch, nextBatch));
    }

    for (size_t i = 0; i < futures.size(); i++) {
        auto resultDelta = futures[i].get();
        for (auto e : resultDelta) {
            int key = e.first;
            double value = e.second;
            bc[key] += value;
        }
    }

    for (auto e : vertices)
        cout << e << " " << bc[e] << endl;

    return 0;
}
