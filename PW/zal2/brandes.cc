#include <bits/stdc++.h>

#define F first
#define S second
#define pb push_back
#define INF (1 << 30)
#define SQR(a) ((a) * (a))

using namespace std;

const int N = 1111;

map<int, vector<int>> edges, p;
map<int, int> d;
set<int> v;
map<int, double> delta, sigma, bc;

int main() {
    freopen("input.txt", "r", stdin);

    int x, y;
    while (cin >> x >> y) {
        v.insert(x);
        v.insert(y);
        edges[x].push_back(y);
        edges[y].push_back(x);
    }

    for (auto e : v) {
        bc[e] = 0;
    }

    for (auto s : v) {
        stack<int> st;
        for (auto w : v) {
            sigma[w] = 0;
            d[w] = -1;
            delta[w] = 0;
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
            if (w != s) {
                bc[w] += delta[w];
            }
        }
    }

    for (auto e : v) {
        cout << e << " " << bc[e] << endl;
    }

    return 0;
}
