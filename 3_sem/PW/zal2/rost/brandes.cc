//for vs compiler
/*#include <thread>
#include <future>
#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <fstream>
#include <unordered_map>
#include <map>
#include <unordered_set>
#include <stack>
#include <cstring>
#include <set>
#include <queue>
*/
#include <bits/stdc++.h> //should compile
using namespace std;



//needed information about graph
map <int, int> idToNum;
map <int, int> numToId;
vector <vector <int> > g;
unordered_map <int, vector <int > > g1;
int threads = 1;
vector < long double > BC;
int N = 0;
mutex mtx, mtx2;
atomic <int> currentNode{ -1 };

int next(int id) //aux. function
{
	int toReturn;
	mtx.lock();
	currentNode++;
	toReturn = currentNode;
	mtx.unlock();
	if (currentNode >= N)
		return -1;
	return toReturn;
}
//threads
void counter(int id)
{
	int cur = next(id);
	vector <vector <int> > P(N);
	vector <long double > sigma(N);
	vector <long double > d(N);
	vector <long double> myBC(N, 0);
	vector <long double > delta(N);
	while (cur != -1) //finish where there's no more nodes
	{
		stack <int> S;
		for (int i = 0; i < N; i++)
		{
			P[i].clear();
			sigma[i] = .0;
			d[i] = -1.0;
			delta[i] = .0;
		}
		sigma[cur] = 1.0;
		d[cur] = .0;
		queue <int> Q;
		Q.push(cur);
		while (!Q.empty()) { //n
			int v = Q.front();
			Q.pop();
			S.push(v);
			for (auto w : g[v]) //m
			{
				if (d[w] < 0)
				{
					Q.push(w);
					d[w] = d[v] + 1;
				}
				if (d[w] == d[v] + 1)
				{
					sigma[w] += sigma[v];
					P[w].push_back(v);
				}
			}
		}
		while (!S.empty())
		{
			int w = S.top();
			S.pop();
			for (auto v : P[w])
			{
				delta[v] += (sigma[v] * 1.0 / sigma[w])*(1.0 + delta[w]);
			}
			if (w != cur && delta[w] > 1e-9)
			{
				myBC[w] += delta[w];

			}
		}
		cur = next(id);
	}
	mtx.lock();
	for (int i = 0; i < N; i++)
	{
		BC[i] += myBC[i];
	}
	mtx.unlock();
}

int main(int argc, char* argv[])
{
	threads = atoi(argv[1]);
	ifstream cin(argv[2]);
	ofstream cout(argv[3]);


	while (!cin.eof())
	{
		int a, b;
		cin >> a >> b;
		if (idToNum.find(a) == idToNum.end())
		{
			idToNum[a] = N;
			BC.push_back(0);
			numToId[N] = a;
			N++;
		}
		if (idToNum.find(b) == idToNum.end())
		{
			idToNum[b] = N;
			BC.push_back(0);
			numToId[N] = b;
			N++;
		}
		g1[a].push_back(b);
	}
	g.resize(N);
	for (auto q : g1)
	{
		int w = idToNum[q.first];
		for (auto z : q.second)
		{
			g[w].push_back(idToNum[z]);
		}
	}
	std::vector<std::thread> myThreads;
	for (int i = 0; i < threads; i++)
	{
		std::thread t{ counter, i };
		myThreads.push_back(move(t));
	}
	for (int i = 0; i < threads; i++)
	{
		myThreads[i].join();
	}
	vector < pair <int, long double> > ans;
	for (int i = 0; i < BC.size(); i++)
	{
		if (g[i].size() > 0)
			ans.push_back(make_pair(numToId[i], BC[i]));
	}
	sort(ans.begin(), ans.end());
	for (auto x : ans)
	{
		cout << x.first << " " << setprecision(9) << x.second << endl; //for better precision
																	   //cout << x.first << " " << x.second << endl;
	}
}
