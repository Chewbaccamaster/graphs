#include <iostream>
#include <stdio.h>
#include <fstream>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;


int N, R1, C1, R2, C2, start;
ifstream fin("input.txt");
ofstream fout("output.txt");
vector<vector<int>> maze1,maze2;
vector<vector<int>> g;


void create_mazes() {
	fin >> N >> R1 >> C1 >> R2 >> C2;
	maze1.resize(N);
	maze2.resize(N);
	start = R1*N*N*N + C1*N*N + R2*N + C2;
	
	
	for (int i = 0;i <N;++i) {
		maze1[i].reserve(N);
		for (int j = 0;j < N;++j) {
			int peak;
			fin >> peak;
			maze1[i].push_back(peak);
		}
	}

	for (int i = 0;i <N;++i) {
		maze2[i].reserve(N);
		for (int j = 0;j < N;++j) {
			int peak;
			fin >> peak;
			maze2[i].push_back(peak);
		}
	}
	fin.close();
}

void print_maze(vector<vector<int>> v) {
	for (int i = 0;i < N; i++) {
		for (int j = 0;j < N;j++) {
			cout << v[i][j] << ' ';
		}
		cout << endl;
	}
}

bool check_wall(vector<vector<int>>&v,int a1, int b1) {
	if (a1 >= 0 && a1 < N&&b1>=0 && b1 < N)
	{
		if (v[a1][b1] == 1) 
			return false;
		}
		else
			return false;
	return true;
}

int go_up(int a, int b, int c, int d) {
	int x = a, y = c;
	if (check_wall(maze1, a - 1, b))
		x -= 1;
	if (check_wall(maze2, c - 1, d))
		y -= 1;
	return x*N*N*N + b*N*N + y*N + d;
}

int go_down(int a, int b, int c, int d) {
	int x = a, y = c;
	if (check_wall(maze1, a + 1, b))
		x += 1;
	if (check_wall(maze2, c + 1, d))
		y += 1;
	return x*N*N*N + b*N*N + y*N + d;
}


int go_left(int a, int b, int c, int d) {
	int x = b, y = d;
	if (check_wall(maze1, a, b - 1))
		x -= 1;
	if (check_wall(maze2, c, d - 1))
		y -= 1;
	return a*N*N*N + x*N*N + c*N + y;
}

int go_right(int a, int b, int c, int d) {
	int x = b, y = d;
	if (check_wall(maze1, a, b + 1))
		x += 1;
	if (check_wall(maze2, c, d + 1))
		y += 1;
	return a*N*N*N + x*N*N + c*N + y;
}


void create_list() {
	for (int i1 = 0;i1 < N;i1++) 
		for (int j1 = 0; j1 < N;j1++)
			for (int i2 = 0;i2 < N;i2++)
				for (int j2 = 0;j2 < N;j2++)
				{
					vector<int> v;
					int up, left, down, right;
					up = go_up(i1, j1, i2, j2);
					left = go_left(i1, j1, i2, j2);
					down = go_down(i1, j1, i2, j2);
					right = go_right(i1, j1, i2, j2);

					v.push_back(up);
					v.push_back(down);
					v.push_back(left);
					v.push_back(right);

					g.push_back(v);
				}
	
}


int bfs(int s)
{	
	vector<bool> used(N*N*N*N);
	vector<int> d(N*N*N*N);
	queue<int> q;
	fill(used.begin(), used.end(), false);
	fill(d.begin(), d.end(), 0);
	q.push(s);
	used[s] = true;
	while (!q.empty()) {
		int v = q.front();
		q.pop();
		for (size_t i = 0; i<g[v].size(); ++i) {
			int to = g[v][i];
			if (!used[to]) {
				used[to] = true;
				q.push(to);
				d[to] = d[v] + 1;			
			}
		}
	}

	return d[0];
}


int main() {
	int rez;
	create_mazes();

	create_list();	

	rez = bfs(start);
	
	fout << rez;
	fout.close();
	




	return EXIT_SUCCESS;
}


