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

void create_mazes() {
	fin >> N >> R1 >> C1 >> R2 >> C2;
	maze1.resize(N);
	maze2.resize(N);
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

bool check_wall(vector<vector<int>>&v,int a1, int b1) {
	if (a1 >= 0 && a1 < N&&b1 >= 0 && b1 < N)
	{
		if (v[a1][b1] == 1) 
			return false;
		}
		else
			return false;
	return true;
}

Vertex go_up(Vertex &in) {
	Vertex tempv;
	tempv = in;
	if (check_wall(maze1, tempv.x1 - 1, tempv.y1))
		tempv.x1 -= 1;
	if (check_wall(maze2, tempv.x2 - 1, tempv.y2))
		tempv.x2 -= 1;
	return tempv;
}

Vertex go_down(Vertex &in) {
	Vertex tempv;
	tempv = in;
	if (check_wall(maze1, tempv.x1 + 1, tempv.y1))
		tempv.x1 += 1;
	if (check_wall(maze2, tempv.x2 + 1, tempv.y2))
		tempv.x2 += 1;
	return tempv;
}

Vertex go_left(Vertex &in) {
	Vertex tempv;
	tempv = in;
	if (check_wall(maze1, tempv.x1, tempv.y1 - 1))
		tempv.y1 -= 1;
	if (check_wall(maze2, tempv.x2, tempv.y2 - 1))
		tempv.y2 -= 1;
	return tempv;
}

Vertex go_right(Vertex &in) {
	Vertex tempv;
	tempv = in;
	if (check_wall(maze1, tempv.x1, tempv.y1 + 1))
		tempv.y1 += 1;
	if (check_wall(maze2, tempv.x2, tempv.y2 + 1))
		tempv.y2 += 1;
	return tempv;
}

int convert_coord(Vertex &ver) {
	return ver.x1*N*N*N + ver.y1*N*N + ver.x2*N + ver.y2;
}

int bfs(Vertex s)
{
	vector<bool> used(N*N*N*N);
	vector<int> d(N*N*N*N);
	queue<Vertex> q;
	fill(used.begin(), used.end(), false);
	fill(d.begin(), d.end(), 0);
	q.push(s);

	used[convert_coord(s)] = true;
	while (!q.empty()) {
		Vertex v = q.front();
		q.pop();

		Vertex up, down, left, right;
		up = go_up(v);
		down = go_down(v);
		left = go_left(v);
		right = go_right(v);

		if (!used[convert_coord(up)]) {
			used[convert_coord(up)] = true;
			q.push(up);
			d[convert_coord(up)] = d[convert_coord(v)] + 1;
		}
		if (!used[convert_coord(down)]) {
			used[convert_coord(down)] = true;
			q.push(down);
			d[convert_coord(down)] = d[convert_coord(v)] + 1;
		}
		if (!used[convert_coord(left)]) {
			used[convert_coord(left)] = true;
			q.push(left);
			d[convert_coord(left)] = d[convert_coord(v)] + 1;
		}
		if (!used[convert_coord(right)]) {
			used[convert_coord(right)] = true;
			q.push(right);
			d[convert_coord(right)] = d[convert_coord(v)] + 1;
		}
		
	}
	return d[0];
}


int main() {
	int rez;
	create_mazes();
	rez = bfs(start);
	fout << rez;
	fout.close();
	return EXIT_SUCCESS;
}