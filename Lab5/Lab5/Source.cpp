#include <iostream>
#include <stdio.h>
#include <fstream>
#include <vector>
#include <algorithm>

using namespace std;

ifstream fin("input.txt");
ofstream fout("output.txt");
int N;


struct Line {
	double a, b, c;
};

struct pt {
	int x, y;
};

const double Pi = atan(1) * 4;
const double EPS = 1E-9;

vector<Line> v;

vector<int> Vintik, Shpuntik;

vector < pair<int, pair<int, int>>> g;

void create_vector(vector<Line> &vec, int &n, vector<int> &v1, vector<int> &v2) {
	fin >> n;
	for (int i = 0;i < n;++i) {
		Line ln;
		int x1, y1, x2, y2;
		fin >> x1 >> y1 >> x2 >> y2;


		// (x1,y1)  (x2,y2)
		// x-x1/x2-x1  = y-y1/y2-y1

		ln.a = y2 - y1;
		ln.b = x1 - x2;
		ln.c = -x1*y2 + y1*x2;
		vec.emplace_back(ln);
	}

	int a, b, c, d;
	fin >> a, b, c, d;
	v1.emplace_back(a, b); // Vintik
	v2.emplace_back(c, d); // Shpuntik
}



double det(double a, double b, double c, double d) {
	return a * d - b * c;
}

bool intersect(Line m, Line n) {
	double zn = det(m.a, m.b, n.a, n.b);
	if (abs(zn) < EPS)
		return false;
//	res.x = -det(m.c, m.b, n.c, n.b) / zn; координаты точки пересечения прямых
//	res.y = -det(m.a, m.c, n.a, n.c) / zn;
	return true;
}

bool parallel(Line m, Line n) {
	return abs(det(m.a, m.b, n.a, n.b)) < EPS;
}

bool equivalent(Line m, Line n) {
	return abs(det(m.a, m.b, n.a, n.b)) < EPS
		&& abs(det(m.a, m.c, n.a, n.c)) < EPS
		&& abs(det(m.b, m.c, n.b, n.c)) < EPS;
}

double count_angle(const Line &ln1, const Line &ln2) {
	double cos = (ln1.a*ln2.a + ln1.b*ln2.b) / (sqrt(ln1.a*ln1.a + ln1.b*ln1.b)*sqrt(ln2.a*ln2.a + ln2.b*ln2.b));
	return acos(cos)*180.0 / Pi;
}
void create_graph(const vector<Line> &vec) {
	int cnt = 0;
	int ver1, ver2;
	for (int i = 0;i < vec.size(); ++i)
		for (int j = 0;j < vec.size(); ++j) 
			if (intersect(vec[i], vec[j]))
			{
				
			}

			
		
}

int main() {
	create_vector(v, N, Vintik, Shpuntik);
	
}