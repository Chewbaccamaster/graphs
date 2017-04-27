#include <iostream>
#include <stdio.h>
#include <fstream>
#include <vector>
#include <algorithm>
#include <map>
using namespace std;

ifstream fin("input.txt");
ofstream fout("output.txt");
int H, V, vertexnum = 0, Wprice, Kprice = 0, lastnum = 0, residue = 0;
vector<int> p, rang;
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
		temp.cost = c - b + 1;
		if (temp.c1 > temp.c2)
			swap(temp.c1, temp.c2);
		temp.intersec.resize(0);

		vec1.emplace_back(temp);
	}
	for (int i = 0;i < V; ++i) {
		Word temp;
		int a, b, c;
		fin >> a >> b >> c; // r1 r2 c	 0 5 1

							// r1 r2 c   0 5 3
		temp.r = c; // r <- c
		temp.c1 = a; // c1 <- r1
		temp.c2 = b;  // c2 <- r2
		if (temp.c1 > temp.c2)
			swap(temp.c1, temp.c2);
		temp.cost = abs(b - a + 1);
		temp.intersec.resize(0);

		vec2.emplace_back(temp);
	}
		
}

int count_cost(vector<Word> &vec1, vector<Word> &vec2) {
	int a = 0;
	for (int i = 0;i < vec1.size();++i)	{
		a += vec1[i].cost;
	}
	for (int i = 0;i < vec2.size();++i)	{
		a += vec2[i].cost;
	}
	return a;
}

void sort_words(vector<Word> &vec1) {
	sort(vec1.begin(), vec1.end(), [](const Word &v1, const Word &v2) -> bool {
		int r1 = v1.r;
		int r2 = v2.r;
		return r1 < r2;
	});
}





void sort_intersec(vector<Word> &h, vector<Word> &v) {
	for (int i = 0;i < h.size();++i)
		sort(h[i].intersec.begin(), h[i].intersec.end());

	for (int i = 0;i < v.size();++i)
		sort(v[i].intersec.begin(), v[i].intersec.end());
}
void find_intersections(vector<Word> &h, vector<Word> &v, const int N1, const int N2, int &cnt) {
	for (int i = 0; i < N1;++i) 
		for (int j = 0;j < N2;++j) {
			if (v[j].c1 <= h[i].r && h[i].r <= v[j].c2 && h[i].c1 <= v[j].r && v[j].r <= h[i].c2) {
				h[i].intersec.emplace_back(v[j].r);
				v[j].intersec.emplace_back(h[i].r);
				cnt += 1;
			}
		}
	
	
}

void create_graph(vector<Word> &h, vector<Word> &v, vector <pair<int, pair<int, int>>> &adjG, int &lnum,int &resd) {
	map<pair<int, int>, int> iss;
	vector<int> lastnumber(0);
	int a = 0, b = 1;
	for (int i = 0;i < h.size();++i) {
		a++;
		b++;
		if (h[i].intersec.size() > 1) {

			resd += h[i].intersec[0] - h[i].c1 + h[i].c2 - h[i].intersec[h[i].intersec.size()-1];
			for (int j = 0;j < h[i].intersec.size() - 1;++j) {
				pair<int, pair<int, int>> p;


				p.second.first = a;
				p.second.second = b;

				iss.emplace(make_pair(make_pair(h[i].r, h[i].intersec[j]), a));
				iss.emplace(make_pair(make_pair(h[i].r, h[i].intersec[j + 1]), b));

				p.first = abs(h[i].intersec[j + 1] - h[i].intersec[j] - 1);
				adjG.emplace_back(p);
				a++;
				b++;
			}

		}
		else if (h[i].intersec.size() == 1) {
			iss.emplace(make_pair(make_pair(h[i].r, h[i].intersec[0]), a));
			a++;
			b++;
		}
		else
		{
			resd += h[i].cost;
		}
		
	}
	lastnumber.emplace_back(b);

	lnum = lastnumber[0];
	for (int i = 0;i < v.size();++i) {
		if (v[i].intersec.size() > 0) {
			if (v[i].intersec.size() != 1)
				resd += v[i].intersec[0] - v[i].c1 + v[i].c2 - v[i].intersec[v[i].intersec.size() - 1];
			for (int j = 0;j < v[i].intersec.size() - 1;++j) {
				pair<int, pair<int, int>> p;

				p.second.first = iss[make_pair(v[i].intersec[j], v[i].r)];
				p.second.second = iss[make_pair(v[i].intersec[j + 1], v[i].r)];

				p.first = abs(v[i].intersec[j + 1] - v[i].intersec[j] - 1);
				adjG.emplace_back(p);
			}
		}
		else
			resd += v[i].cost;
	}


}
int dsu_get(int v) {
	if (v == p[v])
		return v;
	return p[v] = dsu_get(p[v]);
}

void dsu_unite(int a, int b) {
	int	ra = dsu_get(a);
	int rb = dsu_get(b);
	if (ra == rb)
		return;
	if (rang[ra] > rang[rb])
		swap(ra, rb);
	p[ra] = rb;
	if (rang[ra] == rang[rb])
		rang[rb]++;

}

void kruskal(vector <pair<int, pair<int, int>>> &adjG, const int cnt, int &c, const int lnum)
{
	int cost = 0, n = lnum, m = adjG.size();
	sort(adjG.begin(), adjG.end(), [](auto& t1, auto& t2) {
		return (t1.second < t2.second);
	});
	p.resize(n);
	rang.resize(n, 0);

	for (int i = 0; i < n; ++i)
		p[i] = i;

	for (int i = 0; i < m; ++i) {
		int a = g[i].second.first, b = g[i].second.second, l = g[i].first;
		if (dsu_get(a) == dsu_get(b)) {
			if (l == 0) {
				fout << -1 << endl;
				fout.close();
				return;
			}
		}
		else {
			dsu_unite(a, b);
			cost += l;
			continue;
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

	Wprice = count_cost(hor, ver);

	sort_words(hor);

	sort_words(ver);

	find_intersections(hor, ver, H, V,vertexnum);

	sort_intersec(hor, ver);

	create_graph(hor, ver, g,lastnum,residue);

	kruskal(g, vertexnum, Kprice,lastnum);

	print_result(Wprice, Kprice, vertexnum);

	return EXIT_SUCCESS;
}