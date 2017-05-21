#include <iostream>
#include <stdio.h>
#include <fstream>
#include <vector>
#include <algorithm>
using namespace std;

const double Pi = atan(1) * 4;
const double EPS = 1E-9;

ifstream fin("input.txt");
ofstream fout("output.txt");

struct pt {
	double x, y;

	pt (){}
	pt(double a, double b) {
		x = a;
		y = b;
	}
	bool operator< (const pt & p) const {
		return x < p.x - EPS || abs(x - p.x) < EPS && y < p.y - EPS;
	}
};

struct Line {
	double a, b, c;

	Line() {}
	Line(pt p, pt q) {
		a = p.y - q.y;
		b = q.x - p.x;
		c = -a * p.x - b * p.y;
		norm();
	}

	void norm() {
		double z = sqrt(a*a + b*b);
		if (abs(z) > EPS)
			a /= z, b /= z, c /= z;
	}

	double dist(pt p) const {
		return a * p.x + b * p.y + c;
	}
};

struct Segment {
	pt first_pt;
	pt second_pt;

	Segment() {}
	Segment(double a, double b, double c, double d) {
		first_pt = pt(a, b);
		second_pt = pt(c, d);
	}
	vector<pt> intersection;
};

vector<Segment> v;

pt Vintik, Shpuntik;

vector < pair<int, pair<int, int>>> g;

void create_vector(vector<Segment> &vec) {
	int N;
	fin >> N;
	for (int i = 0;i < N;++i) {
		Segment seg;
		double x1, y1, x2, y2;
		fin >> x1 >> y1 >> x2 >> y2;
	

		if (x1 > x2) {
			swap(x1, x2);
			swap(y1, y2);
		}
		seg = Segment(x1, y1, x2, y2);
		// (x1,y1)  (x2,y2)
		// x-x1/x2-x1  = y-y1/y2-y1
		
		vec.push_back(seg);
	}

	
	
	fin >> Vintik.x >> Vintik.y >> Shpuntik.x >> Shpuntik.y;
	//v1.push_back(Vintik); // Vintik
//	v2.push_back(Shpuntik); // Shpuntik
}

#define det(a,b,c,d)  (a*d-b*c)

inline bool betw(double l, double r, double x) {
	return min(l, r) <= x + EPS && x <= max(l, r) + EPS;
}

inline bool intersect_1d(double a, double b, double c, double d) {
	if (a > b)  swap(a, b);
	if (c > d)  swap(c, d);
	return max(a, c) <= min(b, d) + EPS;
}

bool intersect(pt &a, pt &b, pt &c, pt &d, pt & cross) {
	if (!intersect_1d(a.x, b.x, c.x, d.x) || !intersect_1d(a.y, b.y, c.y, d.y))
		return false;
	Line m(a, b);
	Line n(c, d);
	double zn = det(m.a, m.b, n.a, n.b);
	if (abs(zn) < EPS) {
		if (abs(m.dist(c)) > EPS || abs(n.dist(a)) > EPS)
			return false;
		if (b < a)  swap(a, b);
		if (d < c)  swap(c, d);
	//	cross = max(a, c);
	//	right = min(b, d);
		return true;
	}
	else {
		cross.x  = -det(m.c, m.b, n.c, n.b) / zn;
		cross.y  = -det(m.a, m.c, n.a, n.c) / zn;
		return betw(a.x, b.x, cross.x)
			&& betw(a.y, b.y, cross.y)
			&& betw(c.x, d.x, cross.x)
			&& betw(c.y, d.y, cross.y);
	}
}

inline void convert_from_segment_to_pt(const Segment& seg, pt &a, pt &b) {
	a = seg.first_pt;
	b = seg.second_pt;
}

/*double count_angle(const Line &ln1, const Line &ln2) {
	double cos = (ln1.a*ln2.a + ln1.b*ln2.b) / (sqrt(ln1.a*ln1.a + ln1.b*ln1.b)*sqrt(ln2.a*ln2.a + ln2.b*ln2.b));
	return acos(cos)*180.0 / Pi;
}*/

inline void norm_pt(double a, double b, double c){
	double z = sqrt(a*a + b*b);
	if (abs(z) > EPS)
		a /= z, b /= z, c /= z;
}

double count_angle(pt &a, pt &b, pt &c, pt &d) {
	double a1 = a.y - b.y, b1 = b.x - a.x, c1 = -a1*a.x - b1*b.y;
	double a2 = c.y - d.y, b2 = d.x - c.x, c2 = -a2*c.x - b2*d.y;

	norm_pt(a1, b1, c1);
	norm_pt(a2, b2, c2);

	double cos = (a1*a2 + b1*b2) / (sqrt(a1*a1 + b1*b1)*sqrt(a2*a2 + b2*b2));
	return acos(cos)*180.0 / Pi;
}

void find_intersections(vector<Segment> &vec) {
	for (int i = 0;i < vec.size(); ++i)
		for (int j = 0;j < vec.size(); ++j) {
			if (i != j) {
				pt a, b, c, d, cross_point;
				convert_from_segment_to_pt(vec[i], a, b);
				convert_from_segment_to_pt(vec[j], c, d);
				if (intersect(a, b, c, d, cross_point)) 
					vec[i].intersection.push_back(cross_point);				
			}
			
		}			
}

int main() {

	create_vector(v);

	find_intersections(v);
	
	
}