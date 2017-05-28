
#include <iostream>
#include <stdio.h>
#include <fstream>
#include <vector>
#include <algorithm>
using namespace std;

ifstream fin("input.txt");
ofstream fout("output.txt");

const int INF = 1000000;

struct edge {
	int a, b, cap, flow;
};

void create_vector(vector<int> &grid, vector<int> &pins, int& N, int& K) {
	fin >> N >> K;

	grid.resize(K);
	pins.resize(K);

	for (int i = 0; i < K; ++i) {
		fin >> grid[i];
	}
	for (int i = 0;i < K;++i) {
		int temp;
		fin >> temp;
		grid[i] *= N;
		grid[i] += temp;
	}

	for (int i = 0;i < K;++i) {	
		fin >> pins[i];
	}

	for (int i = 0;i < K;++i) {
		int temp;
		fin >> temp;
		pins[i] *= N;
		pins[i] += temp + N*N;
	}
}
void add_edge(int a, int b, vector<edge> &e, vector<vector<int>> &g) {
	edge e1 = { a, b, 1, 0 };
	edge e2 = { b, a, 0, 0 };
	g[a].push_back((int)e.size());
	e.push_back(e1);
	g[b].push_back((int)e.size());
	e.push_back(e2);
}

void check_for_border_and_add(int i, int border, int a, int b, vector<edge> &e, vector<vector<int>> &g) {
	if (i > border)
		add_edge(a, b, e, g);
}
void create_graph(vector<vector<int>> &g, vector<edge> &e, vector<int> &grid, vector<int> &pins, int &N, int &K) {
	g.resize(2 * N*N + 2);
	for (int i = 0;i < N;++i) {
		for (int j = 0;j < N;++j) {
			int low_layer = j + i*N + N*N;
			add_edge(j + i*N, low_layer, e, g);
			check_for_border_and_add(i, 0, low_layer, j + (i - 1)*N, e, g);
			check_for_border_and_add(N, i + 1, low_layer, j + (i + 1)*N, e, g);
			check_for_border_and_add(j, 0, low_layer, j - 1 + i*N, e, g);
			check_for_border_and_add(N, j + 1, low_layer, j + 1 + i*N, e, g);
		}
	}

	for (int i = 0;i < K;++i) {
		add_edge(2 * N*N, grid[i], e, g);
		add_edge(pins[i], 2 * N*N + 1, e, g);
	}
}


bool bfs(int s, int t, vector<int> &d, vector<int> &q, vector<vector<int>> &g, vector<edge> &e) {
	int qh = 0, qt = 0;
	q[qt++] = s;
	fill(d.begin(), d.end(), -1);
	d[s] = 0;
	while (qh < qt && d[t] == -1) {
		int v = q[qh++];
		for (size_t i = 0; i<g[v].size(); ++i) {
			int id = g[v][i],
				to = e[id].b;
			if (d[to] == -1 && e[id].flow < e[id].cap) {
				q[qt++] = to;
				d[to] = d[v] + 1;
			}
		}
	}
	return d[t] != -1;
}

int dfs(int v, int flow, int t, vector<int>& ptr, vector<int> &d, vector<vector<int>> &g, vector<edge>& e) {
	if (!flow)  return 0;
	if (v == t)  return flow;
	for (; ptr[v]<(int)g[v].size(); ++ptr[v]) {
		int id = g[v][ptr[v]],
			to = e[id].b;
		if (d[to] != d[v] + 1)  continue;
		int pushed = dfs(to, min(flow, e[id].cap - e[id].flow), t, ptr, d, g, e);
		if (pushed) {
			e[id].flow += pushed;
			e[id ^ 1].flow -= pushed;
			return pushed;
		}
	}
	return 0;
}

int dinic(int& s, int& t, vector<vector<int>>& g, vector<edge> & e) {
	vector<int> d(g.size());
	vector<int> ptr(g.size());
	vector<int> q(g.size());
	int flow = 0;
	for (;;) {
		if (!bfs(s, t, d, q, g, e))  break;
		fill(ptr.begin(), ptr.end(), 0);
		while (int pushed = dfs(s, INF, t, ptr, d, g, e))
			flow += pushed;
	}
	return flow;
}

void print(int &start, int &end, vector<vector<int>> &g, int &K, int &N, int& flow_weight, vector<edge> &e) {
	if (flow_weight != K) {
		fout << -1 << endl;
		return;
	}
	for (int i = 0; i < g[start].size(); ++i) {
		int temp = e[g[start][i]].b;
		while (temp != end)
			for (int j = 0;j < g[temp].size();++j) {
				if (e[g[temp][j]].flow == 1)
				{
					if (e[g[temp][j]].a < N*N)
						fout << e[g[temp][j]].a << " ";
					temp = e[g[temp][j]].b;
				}

			}
		fout << endl;
	}
}

int main() {
	vector<int> grid, pins;
	vector<vector<int>> g;
	vector<edge> e;
	int N, K;
	create_vector(grid, pins, N, K);
	create_graph(g, e, grid, pins, N, K);

	int source = 2 * N*N;
	int endsource = 2 * N*N + 1;
	int flow_weight = dinic(source, endsource, g, e);

	print(source, endsource, g, K, N, flow_weight, e);

	return EXIT_SUCCESS;
}