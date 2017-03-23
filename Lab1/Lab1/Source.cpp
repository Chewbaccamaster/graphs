#include <stdio.h>
#include <vector>
#include <algorithm>
#include <iostream>
#include <fstream>
using namespace std;

vector<vector<int>> graph;
vector<bool> used;
vector<int> f, tin;
int timer, N,counter = 0;

void create_graph(){
	ifstream fin("input.txt");
	int peak;
	fin >> N;
	
	graph.resize(N);
	tin.resize(N);
	f.resize(N);
	used.resize(N);
	
	for (int i = 0; i < N; ++i){
		int d;
		fin >> d;
		for (int j = 0; j < d; ++j){
			fin >> peak;
			graph[i].push_back(peak);
		}
	}
}

void dfs(vector<vector<int>> &g, int v, int p,int b1,int b2){	
	
	used[v] = true;
	tin[v] = f[v] = timer++;
	for (int i = 0;i < g[v].size();++i){
		int to = g[v][i];
		if (to == p) continue;
		if (used[to])
			f[v] = min(f[v], tin[to]);
		else {
			dfs(g, to, v,b1,b2);
			f[v] = min(f[v], f[to]);
			if (f[to] > tin[v])
			if (g[v].size()!=1 && g[to].size()!=1){
				if (counter == 1)
					return;
				ofstream fout("output.txt");
				fout << v << " " << to << endl;
				fout << b1 << " " << b2 << endl;
				fout.close();
				counter++;
				return;
			}
		}
	}
}

int main(){
	create_graph();
	vector<vector<int>> newg;
	int br1, br2;
	for (int i = 0;i < N;++i) {
		for (int j = 0; j < graph[i].size();++j){
			if (counter == 1)
				return EXIT_SUCCESS;

			fill(f.begin(), f.end(), 0);
			fill(tin.begin(), tin.end(), 0);
			fill(used.begin(), used.end(), false);

			newg = graph;
			int newpeak = newg[i][j];
			newg[i].erase(newg[i].begin() + j);
			br1 = newpeak;
			br2 = i;
			for (int k = 0; k < newg[newpeak].size();++k){
				if (newg[newpeak][k] == i) {
					newg[newpeak].erase(newg[newpeak].begin() + k);
					break;
				}
			}
			dfs(newg, 0, -1,br1,br2);
		}
	}
	return EXIT_SUCCESS;
}