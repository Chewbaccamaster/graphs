#include <iostream>
#include <stdio.h>
#include <fstream>
#include <vector>
#include <algorithm>

using namespace std;

ifstream fin("input.txt");
ofstream fout("output.txt");
vector<vector<double>>  g;
vector <vector<int>> gl;
vector <int> ans, result, path;
int N, a, b;
vector<bool> used;

void create_vector() {
	fin >> N;
	g.resize(N);
	gl.resize(N);
	used.resize(N, false);
	ans.resize(N);
	path.resize(N);

	for (int i = 0;i < N;++i) {
		for (int j = 0;j < N;++j) {
			double peak;
			fin >> peak;
			if (peak != 0)
				gl[i].push_back(j);
			g[i].push_back(peak);
		}
	}	
}

void dfs(int v) {
	used[v] = true;
	for (size_t i = 0; i < gl[v].size(); ++i) {	
		int to = gl[v][i];
		if (!used[to])
			dfs(to);
	}
	ans.emplace_back(v);
}

void topological_sort() {
	
	ans.clear();
	for (int i = 0; i < N; ++i)
		if (!used[i])
			dfs(i);
	reverse(ans.begin(), ans.end());
}

void modifyGraph() {
	vector<vector<double>> tempv;
	tempv = g;

	for (int i = 0;i < N;++i)
		for (int j = 0;j < N;++j)
			tempv[i][j] = g[ans[i]][ans[j]];
	g = tempv;
}

void find_oldnum() {
	for (int i = 0;i < N;++i) {
		if (ans[i] == 0)
			a = i;
		if (ans[i] == N - 1)
			b = i;
	}
}

void probability_search() {
	path.assign(N, 0);
	int i = a;
	for (int k = i + 1; k < N; ++k)
		for (int j = k + 1; j < N; ++j)
			if (g[i][j] < g[i][k] * g[k][j]){
				g[i][j] = g[i][k] * g[k][j];
				path[j] = k;
			}
}

void restore_path() {
	result.emplace_back(ans[b]);
	int x = b;
		while (x){
			if (path[x] != 0)
				result.emplace_back(ans[path[x]]);
			x = path[x];
		}
		result.emplace_back(ans[a]);
}

void printresult() {
	int n = result.size();
	fout << g[a][b] << ' ' << n << endl;
	for (int i = 0;i < n;++i)
		fout << result[n-i-1] << ' ';
	fout.close();
}

int main() {
 	create_vector();

	topological_sort();

	modifyGraph();

	find_oldnum();

	probability_search();
	
	restore_path();

	printresult();

	return EXIT_SUCCESS;
}