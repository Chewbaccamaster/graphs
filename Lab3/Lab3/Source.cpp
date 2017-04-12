#include <iostream>
#include <stdio.h>
#include <fstream>
#include <vector>
#include <algorithm>

using namespace std;

ifstream fin("input.txt");
ofstream fout("output.txt");
vector<vector<double>>  g;
vector <vector<int>> gl, path;
vector <int> ans, result;
int N, a, b;
vector<bool> used;

void create_vector()
{
	fin >> N;
	g.resize(N);
	gl.resize(N);
	used.resize(N);
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
	fill(used.begin(), used.end(), false);
	ans.clear();
	for (int i = 0; i < N; ++i)
		if (!used[i])
			dfs(i);
	reverse(ans.begin(), ans.end());
}

void modifyGraph()
{
	vector<vector<double>> a;
	a = g;

	for (int i = 0;i < N;++i)
		for (int j = 0;j < N;++j)
			a[i][j] = g[ans[i]][ans[j]];
	g = a;
}

void find_oldstart() {
	for (int i = 0;i < N;++i) {
		if (ans[i] == 0)
			a = i;
	}
}

void find_oldfinish() {
	for (int i = 0;i < N;++i) {
		if (ans[i] == N - 1)
			b = i;
	}
}
void probability_search() {
	path.assign(N, vector <int>(N, 0));
	int i = a;
	for (int k = i + 1; k < N; ++k)
		for (int j = k + 1; j < N; ++j)
			if (g[i][j] < g[i][k] * g[k][j]){
				g[i][j] = g[i][k] * g[k][j];
				path[i][j] = k;
			}
}

void restore_path() {
	

	result.emplace_back(ans[b]);
		while (b){
			if (path[a][b] != 0)
				result.emplace_back(ans[path[a][b]]);
			b = path[a][b];
		}
		result.emplace_back(ans[a]);
		reverse(result.begin(), result.end());
}

void printresult() {
	fout << g[a][b] << ' ' << result.size() << endl;
	for (int i = 0;i < result.size();++i)
		fout << result[i] << ' ';
	fout.close();
}
int main() {
 	create_vector();

	topological_sort();

	modifyGraph();

	find_oldstart();

	probability_search();

	find_oldfinish();
	
	restore_path();

	find_oldfinish();

	printresult();


	return EXIT_SUCCESS;
}