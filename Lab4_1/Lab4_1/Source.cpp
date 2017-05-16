#include <iostream>
#include <stdio.h>
#include <fstream>
#include <vector>
#include <algorithm>

using namespace std;

ifstream fin("input.txt");
ofstream fout("output.txt");
int H, V, vertexnum = 0, wremainder = 0, Kprice = 0;
vector<int> p, rang;
vector <pair<int, pair<int, int>>> g;

struct hWord
{
	int r;
	int c1;
	int c2;
};

struct vWord
{
	int r;
	int c1;
	int c2;
	int first_isc;
	int second_isc;
};

vector<hWord> hor;
vector<vWord> ver;

void create_vector(vector<hWord> &vec1, vector<vWord> &vec2) {
	fin >> H >> V;
	for (int i = 0; i < H; ++i) {
		hWord temp;
		int a, b, c;
		fin >> a >> b >> c;  // r c1 c2   1 0 5 

							 //  r c1 c2   4 0 5
		temp.r = a;
		temp.c1 = b;// min(b, c);
		temp.c2 = c;//max(b, c);
		
		
		vec1.emplace_back(temp);
	}
	for (int i = 0; i < V; ++i) {
		vWord temp;
		int a, b, c;
		fin >> a >> b >> c; // r1 r2 c	 0 5 1

							// r1 r2 c   0 5 3
		temp.r = c; // r <- c
		temp.c1 = a;// min(a, b); // c1 <- r1
		temp.c2 = b;//max(a, b);  // c2 <- r2
		
		temp.first_isc = -1;
		temp.second_isc = -1;

		vec2.emplace_back(temp);
	}

}

void sort_hwords(vector<hWord> &vec1) {
	sort(vec1.begin(), vec1.end(), [](const hWord &v1, const hWord &v2) -> bool {
		int r1 = v1.r;
		int r2 = v2.r;
		return r1 < r2;
	});
}
void sort_vwords(vector<vWord> &vec1) {
	sort(vec1.begin(), vec1.end(), [](const vWord &v1, const vWord &v2) -> bool {
		int r1 = v1.r;
		int r2 = v2.r;
		return r1 < r2;
	});
}

void add_edge(int w1,int w2, int v1, int v2) {
	int w = abs(w1 - w2 - 1);
	g.emplace_back(w, make_pair(v1, v2));
}

void create_graph(vector<hWord> &h, vector<vWord> &v, const int N1, const int N2, int &vernum, int &rest) {
	int cnt = 0;
	bool flag = false;
	int last_intersec;
	for (int i = 0;i < N1;++i) {

		flag = false;
		last_intersec = h[i].c1;

		for (int j = 0;j < N2;++j) {
			if (v[j].c1 <= h[i].r && h[i].r <= v[j].c2 && h[i].c1 <= v[j].r && v[j].r <= h[i].c2) {
				if (flag) 
					add_edge(v[j].r, last_intersec, cnt - 1, cnt);
				else {
					rest += v[j].r - h[i].c1;
					flag = true;
				}
					
				if (v[j].first_isc != -1)
					add_edge(h[i].r, v[j].second_isc, v[j].first_isc, cnt);
				else
					rest += h[i].r - v[j].c1;					

				v[j].first_isc = cnt;
				v[j].second_isc = h[i].r;

				last_intersec = v[j].r;

				cnt++;
			}
		}

		if (!flag)
			rest += h[i].c2 - h[i].c1 + 1;
		else
			rest += h[i].c2 - last_intersec;
	}

	vernum = cnt;

	for (int i = 0;i < N2;++i) 
		if (v[i].first_isc == -1)
			rest += v[i].c2 - v[i].c1 + 1;
		else
			rest += v[i].c2 - v[i].second_isc;
	
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

void kruskal(int &vernum, int &c) {
	int cost = 0, n = vernum, m = g.size();

	sort(g.begin(), g.end());
	
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

void print_result(int &Kp, int &rest, int &vnum) {
	fout << Kp + vnum + rest << endl;
	fout.close();
}

int main() {

	create_vector(hor, ver);

	sort_hwords(hor);
	sort_vwords(ver);

	create_graph(hor, ver, H, V, vertexnum, wremainder);

	kruskal(vertexnum, Kprice);

	print_result(Kprice, wremainder, vertexnum);

	return EXIT_SUCCESS;
}