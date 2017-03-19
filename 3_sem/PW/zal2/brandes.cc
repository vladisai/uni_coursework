#include "coordinate_compressor.h"
#include <bits/stdc++.h>

using namespace std;

using deltaT = vector<double>;

CoordinateCompressor compressor;

vector<vector<int>> edges;
vector<int> vertices;
vector<int>
    importantVertices; // vertices that have at least one edge starting from it
vector<int> originalImportantVertices; // original (not compressed) indices of
                                       // important vertices
size_t nextVertex = 0;
deltaT bc;
mutex nextVertexMutex, bcMutex;

size_t getNextVertexIndex() {
    nextVertexMutex.lock();
    int ret = nextVertex;
    if (nextVertex < importantVertices.size()) {
        nextVertex++;
    }
    nextVertexMutex.unlock();
    return ret;
}

void processVertex(int s, deltaT &result) {
    vector<vector<int>> p(compressor.size());
    deltaT delta(compressor.size(), 0);
    deltaT sigma(compressor.size(), 0);
    vector<int> d(compressor.size(), -1);

    stack<int> st;
    queue<int> q;

    sigma[s] = 1;
    d[s] = 0;
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
        if (w != s)
            result[w] += delta[w];
    }
}

void verticesProcessor() {
    deltaT result(compressor.size());
    size_t nextVertexIndex;
    while ((nextVertexIndex = getNextVertexIndex()) <
           importantVertices.size()) {
        processVertex(importantVertices[nextVertexIndex], result);
    }
    bcMutex.lock();
    for (size_t i = 0; i < result.size(); i++) {
        bc[i] += result[i];
    }
    bcMutex.unlock();
}

void sortAndDeleteDuplicates(vector<int> &v) {
    sort(v.begin(), v.end());
    v.erase(unique(v.begin(), v.end()), v.end());
}

int main(int argc, char *argv[]) {
    // assert(argc == 4);
    int threadsNumber = atoi(argv[1]);

    freopen(argv[2], "r", stdin);
    freopen(argv[3], "w", stdout);

    int x, y;
    while (cin >> x >> y) {
        originalImportantVertices.push_back(x);
        x = compressor.compress(x);
        y = compressor.compress(y);
        vertices.push_back(x);
        vertices.push_back(y);
        if ((int)edges.size() <= x)
            edges.resize(x + 1);
        edges[x].push_back(y);
        importantVertices.push_back(x);
    }

    bc.resize(compressor.size());

    sortAndDeleteDuplicates(vertices);
    sortAndDeleteDuplicates(importantVertices);
    sortAndDeleteDuplicates(originalImportantVertices);

    vector<thread> threads;
    for (int i = 0; i < threadsNumber; i++) {
        threads.push_back(thread(verticesProcessor));
    }

    for (auto &thread : threads) {
        thread.join();
    }

    for (auto e : originalImportantVertices) {
        cout << e << " " << bc[compressor.compress(e)] << endl;
    }

    return 0;
}
