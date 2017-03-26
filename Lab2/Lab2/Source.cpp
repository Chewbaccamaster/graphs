#include <iostream>
#include <stdio.h>
#include <fstream>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;

struct Vertex
{
	int x1;
	int y1;
	int x2;
	int y2;
};

int N, R1, C1, R2, C2;
ifstream fin("input.txt");
ofstream fout("output.txt");
vector<vector<int>> maze1,maze2;
Vertex start;
queue<Vertex> q;
vector<bool> used;
vector<int> d;

void create_mazes() {
	fin >> N >> R1 >> C1 >> R2 >> C2;
	maze1.resize(N);
	maze2.resize(N);
	used.resize(N*N*N*N);
	d.resize(N*N*N*N);
	start.x1 = R1;
	start.y1 = C1;
	start.x2 = R2;
	start.y2 = C2;

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

bool check_wall(vector<vector<int>>&v, int a1, int b1) {
	if (a1 >= 0 && a1 < N && b1 >= 0 && b1 < N)	{
		if (v[a1][b1] == 1) 
			return false;
		}
		else
			return false;
	return true;
}

Vertex move(Vertex &in, int a,  int b) {
	Vertex tempv = in;
	if (check_wall(maze1, tempv.x1 + a, tempv.y1 + b)) {
		tempv.x1 += a;
		tempv.y1 += b;
	}
		
	if (check_wall(maze2, tempv.x2 + a, tempv.y2 + b)) {
		tempv.x2 += a;
		tempv.y2 += b;
	}
	return tempv;
		
}

int convert_coord(Vertex &ver) {
	return ver.x1*N*N*N + ver.y1*N*N + ver.x2*N + ver.y2;
}

void check_for_used(Vertex &move, Vertex &ver)
{
	if (!used[convert_coord(move)]) {
		used[convert_coord(move)] = true;
		q.push(move);
		d[convert_coord(move)] = d[convert_coord(ver)] + 1;
	}
}

void bfs(Vertex s)
{
	q.push(s);
 	used[convert_coord(s)] = true;
	while (!q.empty()) {
		if (d[0] != 0)
		{
			return;
		}
		Vertex v = q.front();
		q.pop();

		Vertex up, down, left, right;
		up = move(v, -1, 0);
		down = move(v, 1, 0);
		left = move(v, 0, -1);
		right = move(v, 0, 1);

		check_for_used(up,v);
		check_for_used(down, v);
		check_for_used(left, v);
		check_for_used(right, v);
		
	}
	
}

int main() {
	
	create_mazes();
	bfs(start);
	fout << d[0];
	fout.close();

	return EXIT_SUCCESS;
}