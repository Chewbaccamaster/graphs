#include <iostream>
#include <stdio.h>
#include <fstream>
#include <vector>
#include <algorithm>
#include <map>
using namespace std;

ifstream fin("input.txt");
ofstream fout("output.txt");
int H, V, Wprice = 0, vertexnum = 0, residue = 0;
vector<int> p, rang;
vector <pair<int, pair<int, int>>> g;

struct Word
{
	int r;
	int c1;
	int c2;
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
	Edge arc_h, arc_v;
	pair<int, pair<int, int>> p;
	for (int i = 0;i < N1;++i)
	{
		for (int j = 0;j < N2;++j)
		{
			if (v[j].c1 <= h[i].r && h[i].r <= v[j].c2 && h[i].c1 <= v[j].r && v[j].r <= h[i].c2)
			{

				if (intersec_h.size() == 0)
				{
					arc_h.firstv = cnt;
					rest += abs(v[j].r - h[i].r);
					intersec_h.emplace_back(v[j].r);
					intersec_v.emplace_back(h[i].r);
					vernum++;
					cnt++;
				}
				else
				{
					vernum++;
					arc_h.secondv = cnt;
					cnt++;
					arc_h.cost = abs(v[j].r - intersec_h[0] - 1);
					p.first = arc_h.cost;
					p.second.first = arc_h.firstv;
					p.second.second = arc_h.secondv;
					rest += abs(h[i].c2 - v[j].r);

					adjG.emplace_back(p);
					intersec_h.resize(0);
				}







			}
		}
		if (intersec_h.size() == 0)
			rest += abs(h[i].c2 - h[i].c1 + 1);
		
	}
		
	cout << 1;
}

int main() {


	create_vector(hor, ver, Wprice);

	sort_words(hor);
	sort_words(ver);

	create_graph(g, hor, ver, H, V, vertexnum, residue);




	return EXIT_SUCCESS;
}