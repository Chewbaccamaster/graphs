#include <iostream>
#include <stdio.h>
#include <fstream>
#include <vector>
#include <algorithm>

using namespace std;

ifstream fin("input.txt");
ofstream fout("output.txt");
int H, V, vertexnum = 0, Wprice, Kprice = 0;
vector<int> p;
vector <pair<int, pair<int, int>>> g;

struct Word
{
	int r;
	int c1;
	int c2;
	vector<int> intersec;
	int cost;
}; 

vector<Word> hor, ver;

void create_vector(vector<Word> &vec1, vector<Word> &vec2) {
	fin >> H >> V;
	for (int i = 0;i < H; ++i) {
		Word temp;
		int a, b, c;
		fin >> a >> b >> c;  // r c1 c2   1 0 5 

							//  r c1 c2   4 0 5
		temp.r = a; 
		temp.c1 = b; 
		temp.c2 = c; 
		temp.cost = c - b+1;
		temp.intersec.resize(0);

		vec1.emplace_back(temp);
	}
	for (int i = 0;i < V; ++i) {
		Word temp;
		int a, b, c;
		fin >> a >> b >> c; // r1 r2 c	 0 5 1

							// r1 r2 c   0 5 3
		temp.r = a; 
		temp.c1 = b;
		temp.c2 = c; 
		temp.cost = b - a + 1;
		temp.intersec.resize(0);

		vec2.emplace_back(temp);
	}
		
}

int count_cost(vector<Word> &vec1, vector<Word> &vec2)
{
	int a = 0;
	for (int i = 0;i < vec1.size();++i)
	{
		a += vec1[i].cost;
	}
	for (int i = 0;i < vec2.size();++i)
	{
		a += vec2[i].cost;
	}
	return a;
}
// hor:  1 0 5
// ver:  0 5 1

void find_intersections(vector<Word> &v, vector<Word> &h, const int N1, const int N2, int &cnt)
{
	
	for (int i = 0; i < N1;++i)
		for (int j = 0;j < N2;++j)
		{
			if (v[i].r >= h[j].r)
				if (v[i].c1 <= h[j].c2)
				{		
					v[i].intersec.emplace_back(h[j].c2);
					h[j].intersec.emplace_back(v[i].r);
					cnt += 1;
				}	
		}

	
}

void create_graph(vector<Word> &v, vector<Word> &h, const int N1, const int N2,vector <pair<int, pair<int, int>>> &adjG)
{
	for (int j = 0;j < v[0].intersec.size() - 1;++j) {
		pair<int, pair<int, int>> p;
		p.second.first = j;
		p.second.second = j + 1;
		p.first = v[0].intersec[j + 1] - v[0].intersec[j] - 1;
		adjG.emplace_back(p);
	}

	for (int i = 1;i < N1;++i)
		for (int j = 0;j < v[i].intersec.size() - 1;++j) {
			pair<int, pair<int, int>> p;
			p.second.first = i + v[i - 1].intersec.size() - 1;
			p.second.second = i + v[i - 1].intersec.size();
			p.first = v[i].intersec[j + 1] - v[i].intersec[j] - 1;
			adjG.emplace_back(p);
		}
		

	for (int j = 0;j < h[0].intersec.size() - 1;++j) {
		pair<int, pair<int, int>> p;
		p.second.first = j;
		p.second.second = v[j].intersec.size();
		p.first = h[0].intersec[j + 1] - h[0].intersec[j] - 1;
		adjG.emplace_back(p);
	}

	for (int i = 1;i < N2;++i)
		for (int j = 0;j < h[i].intersec.size() - 1;++j) {
			pair<int, pair<int, int>> p;
			p.second.first = v[i - 1].intersec.size() - 1;
			p.second.second = i + v[i - 1].intersec.size();
			p.first = h[i].intersec[j + 1] - h[i].intersec[j] - 1;
			adjG.emplace_back(p);
		}
			
		
}

int dsu_get(int v) {
	return (v == p[v]) ? v : (p[v] = dsu_get(p[v]));
}

void dsu_unite(int a, int b) {
	a = dsu_get(a);
	b = dsu_get(b);
	if (rand() & 1)
		swap(a, b);
	if (a != b)
		p[a] = b;
}

void kruskal(vector <pair<int, pair<int, int>>> &adjG, const int cnt, int &c)
{
	int cost = 0, n = cnt, m = adjG.size();
	sort(adjG.begin(), adjG.end());
	p.resize(n);

	for (int i = 0; i < n; ++i)
		p[i] = i;

	for (int i = 0; i<m; ++i) {
		int a = g[i].second.first, b = g[i].second.second, l = g[i].first;
		if (dsu_get(a) != dsu_get(b)) {
			cost += l;
			dsu_unite(a, b);
		}
		else if (l == 0) {
			fout << -1 << endl;
			fout.close();
			return;
		}
	}
	c = cost;
}

void print_result(int &Wp,int &Kp, int &vernum) {
	fout << Wp - Kp - vernum << endl;
	fout.close();
}
int main() {

	create_vector(hor, ver);

	find_intersections(hor, ver, H, V,vertexnum);

	create_graph(hor, ver, H, V, g);

	kruskal(g, vertexnum, Kprice);

	Wprice = count_cost(hor, ver);

	print_result(Wprice, Kprice, vertexnum);


	/*
	for (int i = 0; i < hor.size();++i)
		cout << hor[i].r << ' ' << hor[i].c1 << ' ' << hor[i].c2  << endl;

	for (int i = 0; i < hor.size();++i)
		cout << ver[i].c1 << ' ' << hor[i].c2 << ' ' << hor[i].r << endl;
		*/

	return EXIT_SUCCESS;
}