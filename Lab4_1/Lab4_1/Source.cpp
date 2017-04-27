#include <iostream>
#include <stdio.h>
#include <fstream>
#include <vector>
#include <algorithm>
#include <map>
using namespace std;

ifstream fin("input.txt");
ofstream fout("output.txt");
int H, V, Wprice = 0, vertexnum = 0;
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

void count_cost(vector<Word> &vec1, vector<Word> &vec2, int &w) {
	
	for (int i = 0; i < vec1.size(); ++i) {
		w += vec1[i].cost;
	}
	for (int i = 0; i < vec2.size(); ++i) {
		w += vec2[i].cost;
	}
	
}

void sort_words(vector<Word> &vec1) {
	sort(vec1.begin(), vec1.end(), [](const Word &v1, const Word &v2) -> bool {
		int r1 = v1.c1;
		int r2 = v2.c1;
		return r1 < r2;
	});
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

void sort_intersec(vector<Word> &h, vector<Word> &v) {
	for (int i = 0; i < h.size(); ++i)
		sort(h[i].intersec.begin(), h[i].intersec.end());

	for (int i = 0; i < v.size(); ++i)
		sort(v[i].intersec.begin(), v[i].intersec.end());
}

void create_graph(vector <pair<int, pair<int, int>>> &adjG, vector<Word> h, vector<Word> v) {
	

	for (int i = 0;i < v.size();++i)
		for (int j = 0;j < h.size();++j) {

		}
}

int main() {


	create_vector(hor, ver);

	find_intersections(hor, ver, H, V, vertexnum);

	count_cost(hor, ver, Wprice);



	return EXIT_SUCCESS;
}