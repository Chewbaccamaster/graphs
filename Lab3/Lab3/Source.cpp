#include <iostream>
#include <stdio.h>
#include <fstream>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;

ifstream fin("input.txt");
ofstream fout("output.txt");
vector<vector<double>>  g;
vector <vector<int>> gl, path;
vector <int> ans, result;
int N;
vector<bool> used;

void create_vector()
{
	fin >> N;
	g.resize(N*N);
	used.resize(N*N);
	ans.resize(N*N);
	gl.resize(N*N);
	path.resize(N*N);
	for (int i = 0;i < N;++i) {
		g[i].reserve(N);
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
	for (int i = 0; i < N; ++i)
		used[i] = false;
	ans.clear();
	for (int i = 0; i < N; ++i)
		if (!used[i])
			dfs(i);
	reverse(ans.begin(), ans.end());
}

void modifyGraph()
{
	vector<vector<double>> a;

	for (int i = 0;i < N;++i)
		for (int j = 0;j < N;++j)
			a[i][j] = g[ans[i]][ans[j]];
	g = a;
}

void probability_search() {

	int a, b;
	path.assign(N, vector <int>(N, 0));
	for (int i = 0;i < N;++i)
	{
		if (ans[i] == 0)
			a = i;
	}
	int i = a;
	for (int k = i + 1; k < N; ++k)
		for (int j = k + 1; j < N; ++j)
			if (g[i][j] < g[i][k] * g[k][j]) {
				g[i][j] = g[i][k] * g[k][j];
				path[i][j] = k;
			}
}


void restore_path() {
	int a, b;
	for (int i = 0;i < N;++i)
	{
		if (ans[i] == 0)
			a = i;
		if (ans[i] == N - 1)
			b = i;
	}
	result.emplace_back(ans[b]);
		int i = b;
		while (i)
		{
			if (path[a][i] != 0)
				result.emplace_back(ans[path[a][i]]);
			i = path[a][i];
		}
		result.emplace_back(ans[a]);
		reverse(result.begin(), result.end());
}

void printresult() {
	int a, b;
	for (int i = 0;i < N;++i)
	{
		if (ans[i] == 0)
			a = i;
		if (ans[i] == N - 1)
			b = i;
	}

	fout << g[a][b] << ' ' << N << endl;
	for (int i = 0;i < N;++i)
		fout << result[i] << ' ';
}
int main() {
 	create_vector();
	topological_sort();
	probability_search();
	restore_path();
	printresult();
	return EXIT_SUCCESS;
}