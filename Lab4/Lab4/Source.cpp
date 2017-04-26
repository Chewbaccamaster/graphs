#include <iostream>
#include <stdio.h>
#include <fstream>
#include <vector>
#include <algorithm>

using namespace std;

ifstream fin("input.txt");
ofstream fout("output.txt");
int H, V, vertexnum = 0, Wprice, Kprice = 0;
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
		temp.intersec.resize(0);

		vec1.emplace_back(temp);
	}
	for (int i = 0;i < V; ++i) {
		Word temp;
		int a, b, c;
		fin >> a >> b >> c; // r1 r2 c	 0 5 1

							// r1 r2 c   0 5 3
		temp.r = c; 
		temp.c1 = a;
		temp.c2 = b; 
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

void find_intersections(vector<Word> &h, vector<Word> &v, const int N1, const int N2, int &cnt)
{
	
	for (int i = 0; i < N1;++i)
		for (int j = 0;j < N2;++j)
		{
			if (h[i].r >= v[j].c1 && h[i].r <= v[j].c2 && v[j].r)

				{		
					h[i].intersec.emplace_back(v[j].r);
					v[j].intersec.emplace_back(h[i].r);
					cnt += 1;
				}	
		}

	
}

void create_graph(vector<Word> &h, vector<Word> &v, const int N1, const int N2,vector <pair<int, pair<int, int>>> &adjG)
{

	int a = 0, b = 1;
	for (int i = 0;i < N1;++i){
		if (h[i].intersec.size() > 0)
		for (int j = 0;j < h[i].intersec.size() - 1;++j) {
			pair<int, pair<int, int>> p;
			
			p.second.first = a+j;
			p.second.second = b;
			p.first = abs(h[i].intersec[j + 1] - h[i].intersec[j] - 1);
			adjG.emplace_back(p);
			a += 2;
			b += 2;
		}
	}


	for (int i = 0;i < N2;++i) {
		if (v[i].intersec.size() > 0)
		for (int j = 0;j < v[i].intersec.size() - 1;++j) {
			pair<int, pair<int, int>> p;
			p.second.first = i+j;	
			p.second.second = i + h[i].intersec.size();
			p.first = abs(v[i].intersec[j + 1] - v[i].intersec[j]-1);
			adjG.emplace_back(p);
		}
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

void kruskal(vector <pair<int, pair<int, int>>> &adjG, const int cnt, int &c)
{
	int cost = 0, n = cnt, m = adjG.size();
	sort(adjG.begin(), adjG.end());
	p.resize(n);
	rang.resize(n,0);
	for (int i = 0; i < n; ++i) 
		p[i] = i;
		
	for (int i = 0; i < m; ++i) {
		int a = g[i].second.first, b = g[i].second.second, l = g[i].first;
		if (dsu_get(a) != dsu_get(b))
			if (l == 0) {
				fout << -1 << endl;
				fout.close();
				return;
			}
			else {
				cost += l;
				continue;
		}
			dsu_unite(a, b);
	}
	c = cost;	
}


/*void kruskal_rb(vector <pair<int, pair<int, int>>> &adjG, const int cnt, int &c)
{
	int cost = 0, n = cnt, m = adjG.size();
	sort(adjG.begin(), adjG.end());
	p.resize(n);
	rang.resize(n, 0);
	for (int i = 0; i < n; ++i)
		p[i] = i;

	for (int i = 0; i < m; ++i) {
		int a = g[i].second.first, b = g[i].second.second, l = g[i].first;
		if (dsu_get(a) == dsu_get(b))
			continue;
		dsu_unite(a, b);
		cost++;
		if (cost == n - 1)
			break;
	}
	c = cost;
}*/

/*void kruskal1(vector < pair < int, pair<int, int> > > &adjG, const int cnt, int &c) {
	int cost = 0, n = cnt, m = adjG.size();

	sort(adjG.begin(), adjG.end());
	vector<int> tree_id(n);
	for (int i = 0; i<n; ++i)
		tree_id[i] = i;
	for (int i = 0; i<m; ++i)
	{
		int a = g[i].second.first, b = g[i].second.second, l = g[i].first;
		if (tree_id[a] != tree_id[b])
		{
			cost += l;
			int old_id = tree_id[b], new_id = tree_id[a];
			for (int j = 0; j<n; ++j)
				if (tree_id[j] == old_id)
					tree_id[j] = new_id;
		}
	}
	c = cost;
}
*/
void print_result(int &Wp,int &Kp, int &vernum) {
	fout << Wp - Kp - vernum << endl;
	fout.close();
}
int main() {

	create_vector(hor, ver);

	find_intersections(hor, ver, H, V,vertexnum);

	create_graph(hor, ver, H, V, g);
//	kruskal1(g, vertexnum, Kprice);
	kruskal(g, vertexnum, Kprice);

	Wprice = count_cost(hor, ver);

	print_result(Wprice, Kprice, vertexnum);



	return EXIT_SUCCESS;
}