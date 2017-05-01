#include <iostream>
#include <stdio.h>
#include <fstream>
#include <vector>
#include <algorithm>
#include <map>
using namespace std;

ifstream fin("input.txt");
ofstream fout("output.txt");
int H, V, Wprice = 0, vertexnum = 0, residue = 0, Kprice = 0;
vector<int> p, rang;
vector <pair<int, pair<int, int>>> g;

struct Word
{
	int r;
	int c1;
	int c2;
	int first_isc;
	int second_isc;
};

struct Edge 
{
	int firstv;
	int secondv;
	int cost;
};

vector<Word> hor, ver;

void create_vector(vector<Word> &vec1, vector<Word> &vec2, int &cost) {
	fin >> H >> V;
	for (int i = 0; i < H; ++i) {
		Word temp;
		int a, b, c;
		fin >> a >> b >> c;  // r c1 c2   1 0 5 

							 //  r c1 c2   4 0 5
		temp.r = a;
		temp.c1 = min(b, c);
		temp.c2 = max(b, c);
		cost += abs(b - c + 1);
		
		vec1.emplace_back(temp);
	}
	for (int i = 0; i < V; ++i) {
		Word temp;
		int a, b, c;
		fin >> a >> b >> c; // r1 r2 c	 0 5 1

							// r1 r2 c   0 5 3
		temp.r = c; // r <- c
		temp.c1 = min(a,b); // c1 <- r1
		temp.c2 = max(a,b);  // c2 <- r2
		cost += abs(b - a + 1);
		temp.first_isc = -1;
		temp.second_isc = -1;

		vec2.emplace_back(temp);
	}

}


void sort_words(vector<Word> &vec1) {
	sort(vec1.begin(), vec1.end(), [](const Word &v1, const Word &v2) -> bool {
		int r1 = v1.c1;
		int r2 = v2.c1;
		return r1 < r2;
	});
}

/*void find_intersections(vector<Word> &h, vector<Word> &v, const int N1, const int N2, int &cnt)
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

void sort_intersec(vector<Word> &h, vector<Word> &v) {
	for (int i = 0; i < h.size(); ++i)
		sort(h[i].intersec.begin(), h[i].intersec.end());

	for (int i = 0; i < v.size(); ++i)
		sort(v[i].intersec.begin(), v[i].intersec.end());
}
*/
void create_graph(vector <pair<int, pair<int, int>>> &adjG, vector<Word> &h, vector<Word> &v, const int N1, const int N2, int &vernum, int &rest) {
	int cnt = 0;
	vector<int> intersec_h, intersec_v;
	bool flag = false;
	int last_intersec;
	Edge arc_h, arc_v;
	pair<int, pair<int, int>> p;
	for (int i = 0;i < N1;++i)
	{
		flag = false;
		last_intersec = h[i].c1;

		for (int j = 0;j < N2;++j)
		{
			
			if (v[j].c1 <= h[i].r && h[i].r <= v[j].c2 && h[i].c1 <= v[j].r && v[j].r <= h[i].c2)
			{
				if (!flag)
				{
					rest += v[j].r - h[i].c1;
					flag = true;
					
				}

				else
				{
					p.second.first = cnt - 1;
					p.second.second = cnt;
					p.first = abs(v[j].r - last_intersec - 1);
					adjG.emplace_back(p);
				}


				if (v[j].first_isc == -1)
					rest += h[i].r - v[i].c1;
				else 
				{
					p.second.first = v[j].first_isc;
					p.second.second = cnt;
					p.first = abs(h[i].r - v[j].second_isc - 1);
					adjG.emplace_back(p);
				}

					

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
	vertexnum = cnt;

	for (int i = 0;i < N2;++i) {
		if (v[i].first_isc == -1)
			rest += v[i].c2 - v[i].c1 + 1;
		else
			rest += v[i].c2 - v[i].second_isc;
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

void kruskal(vector <pair<int, pair<int, int>>> &adjG, int &vernum, int &c)
{
	int cost = 0, n = vernum, m = adjG.size();
	sort(adjG.begin(), adjG.end());
	
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
			//cout << a << ' ' << b << endl;
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


	create_vector(hor, ver, Wprice);

	sort_words(hor);
	sort_words(ver);

	create_graph(g, hor, ver, H, V, vertexnum, residue);

	kruskal(g, vertexnum, Kprice);
	print_result(Kprice, residue, vertexnum);

	return EXIT_SUCCESS;
}