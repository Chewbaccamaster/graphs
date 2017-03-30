#include <iostream>
#include <stdio.h>
#include <fstream>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;

ifstream fin("input.txt");
ofstream fout("output.txt");
vector<vector<double>>  g,g1;
vector <vector<int>> ans;
int N;
vector<bool> used;

void create_vector()
{
	fin >> N;
	g.resize(N);
	used.resize(N);
	ans.resize(N*N);
	g1.resize(N*N);
	for (int i = 0;i < N;++i) {
		g[i].reserve(N);
		vector <double> v;
		for (int j = 0;j < N;++j)
		{
			
			double peak;
			fin >> peak;
			v.push_back(peak);	
		}
		g.emplace_back(v);
	}


	
}

void dfs(int v) {
	used[v] = true;
	for (size_t i = 0; i < g[v].size(); ++i) {
		
		int to = g[v][i];
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

void probability_search() {
	for (int i = 0; i < N; ++i)
		for (int k = i + 1; k < N; ++k)
			for (int j = k + 1; j < N; ++j)
				if (ans[i][j]<ans[i][k] * ans[k][j]) {
					ans[i][j] = ans[i][k] * ans[k][j];
					g1[i][j] = k;
				}
	
}

void write_result()
{
	fout << ans[0][N - 1] << ' ' << N;
	for (int i = 0;i < N - 1;++i)
		for (int j = 0;j < N;++j)
		fout << g1[i][j];
	
}

int main() {
	create_vector();
	topological_sort();
	probability_search();
	write_result();
	
	return EXIT_SUCCESS;
}