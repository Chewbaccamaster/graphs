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
	for (int i = 0; i < H; ++i) {
		Word temp;
		int a, b, c;
		fin >> a >> b >> c;  // r c1 c2   1 0 5 

							 //  r c1 c2   4 0 5
		temp.r = a;
		temp.c1 = min(b, c);
		temp.c2 = max(b, c);
		temp.cost = temp.c2 - temp.c1 + 1;
		temp.intersec.resize(0);

		vec1.emplace_back(temp);
	}
	for (int i = 0; i < V; ++i) {
		Word temp;
		int a, b, c;
		fin >> a >> b >> c; // r1 r2 c	 0 5 1

							// r1 r2 c   0 5 3
		temp.r = c; // r <- c
		temp.c1 = a; // c1 <- r1
		temp.c2 = b;  // c2 <- r2
		temp.cost = abs(b - a + 1);
		temp.intersec.resize(0);

		vec2.emplace_back(temp);
	}

}

int count_cost(vector<Word> &vec1, vector<Word> &vec2) {
	int a = 0;
	for (int i = 0; i < vec1.size(); ++i) {
		a += vec1[i].cost;
	}
	for (int i = 0; i < vec2.size(); ++i) {
		a += vec2[i].cost;
	}
	return a;
}

void sort_words(vector<Word> &vec1) {
	sort(vec1.begin(), vec1.end(), [](const Word &v1, const Word &v2) -> bool {
		int r1 = v1.c1;
		int r2 = v2.c1;
		return r1 < r2;
	});
}


void sort_intersec(vector<Word> &h, vector<Word> &v) {
	for (int i = 0; i < h.size(); ++i)
		sort(h[i].intersec.begin(), h[i].intersec.end());

	for (int i = 0; i < v.size(); ++i)
		sort(v[i].intersec.begin(), v[i].intersec.end());
}
void find_intersections(vector<Word> &h, vector<Word> &v, const int N1, const int N2, int &cnt)
{
	for (int i = 0; i < N1; ++i)
		for (int j = 0; j < N2; ++j) {
			if (v[j].c1 <= h[i].r && h[i].r <= v[j].c2 && h[i].c1 <= v[j].r && v[j].r <= h[i].c2) {
				h[i].intersec.emplace_back(v[j].r);
				v[j].intersec.emplace_back(h[i].r);
				cnt += 1;
			}
		}


}

void create_graph(vector<Word> &h, vector<Word> &v, vector <pair<int, pair<int, int>>> &adjG, int &lnum)
{
	map<pair<int, int>, int> iss;
	
	int a = 0, b = 0;
	for (int i = 0; i < h.size(); ++i) {
		if (h[i].intersec.size() > 1) {
			a = b++;
			for (int j = 0; j < h[i].intersec.size() - 1; ++j) {
				a = b;
				b++;
				pair<int, pair<int, int>> p;

				p.second.first = a;
				
				p.second.second = b;
				///
				iss.emplace(make_pair(make_pair(h[i].r, h[i].intersec[j]), a));
				//cout << a << ':' << h[i].r << ' ' << h[i].intersec[j] << endl;
				iss.emplace(make_pair(make_pair(h[i].r, h[i].intersec[j + 1]), b));
				//cout << b << ':' << h[i].r << ' ' << h[i].intersec[j + 1] << endl;
				///
				p.first = abs(h[i].intersec[j + 1] - h[i].intersec[j] - 1);
				//cout << a << ' ' << b << ' ' << p.first << endl;
				adjG.emplace_back(p);
			}
		}
		///
		else if (h[i].intersec.size() == 1) {
			iss.emplace(make_pair(make_pair(h[i].r, h[i].intersec[0]), b));
			//cout << b << ':' << h[i].r << ' ' << h[i].intersec[0] << endl;
			a = b;
			b++;
		}
		///
	}
	lnum = b;

	for (int i = 0; i < v.size(); ++i) {
		if (v[i].intersec.size() > 0)
			for (int j = 0; j < v[i].intersec.size() - 1; ++j) {
				pair<int, pair<int, int>> p;
				///
				p.second.first = iss[make_pair(v[i].intersec[j], v[i].r)];
				p.second.second = iss[make_pair(v[i].intersec[j + 1], v[i].r)];
				///
				p.first = abs(v[i].intersec[j + 1] - v[i].intersec[j] - 1);
				//cout << p.second.first << ' ' << p.second.second << ' ' << p.first << endl;
				adjG.emplace_back(p);
			}

	}
	//cout << "-----" << endl;

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

void kruskal(const int cnt, int &c, const int lnum)
{
	int cost = 0, n = lnum, m = g.size();
	sort(g.begin(), g.end());
	
	p.resize(1001 * n);
	rang.resize(1001 * n, 0);

	for (int i = 0; i < 1001 * n; ++i)
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
			//cout << a << ' ' << b << endl;
			dsu_unite(a, b);
			cost += l;
			continue;
		}
	}
	c = cost;
}


void print_result(int &res,int &Kp, int &vernum) {
	fout << res + Kp + vernum << endl;
	fout.close();
}

void count_rest(int &rest) {
	for (auto& t : hor) {
		if (t.intersec.size() == 0)
			rest += t.c2 - t.c1 + 1;
		else
			rest += *t.intersec.begin() - t.c1 + t.c2 - *t.intersec.rbegin();
	}
	for (auto& t : ver) {
		if (t.intersec.size() == 0)
			rest += t.c2 - t.c1 + 1;
		else
			rest += *t.intersec.begin() - t.c1 + t.c2 - *t.intersec.rbegin();
	}
}

int main() {

	create_vector(hor, ver);

	Wprice = count_cost(hor, ver);
	
	sort_words(hor);

	sort_words(ver);

	find_intersections(hor, ver, H, V, vertexnum);

	sort_intersec(hor, ver);

	create_graph(hor, ver, g, lastnum);

	count_rest(residue);
	
	kruskal(vertexnum, Kprice,lastnum);

	print_result(residue, Kprice, vertexnum);

	return EXIT_SUCCESS;
}