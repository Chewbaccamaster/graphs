#include <iostream>
#include <stdio.h>
#include <fstream>
#include <vector>
#include <algorithm>
#include <map>
#include <queue>
using namespace std;

const double Pi = atan(1) * 4;
const double EPS = 1E-9;
#define INF 2e9

ifstream fin("input.txt");
ofstream fout("output.txt");

struct pt {
	double x, y;

	pt() {}
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

struct Road {
	pt first_pt;
	pt second_pt;

	Road() {}
	Road(double a, double b, double c, double d) {
		first_pt = pt(a, b);
		second_pt = pt(c, d);
	}

	vector<int> intersections;
};

vector<vector<int>> adj;
vector<pt> cross_vector;
vector<Road> v;
map <pt, int> cross_list;
pt Vintik, Shpuntik;

vector < pair<double, pair<int, int>>> g;

void create_vector(vector<Road> &vec) {
	int N;
	fin >> N;
	for (int i = 0;i < N;++i) {
		Road road;
		double x1, y1, x2, y2;
		fin >> x1 >> y1 >> x2 >> y2;


	//	if (x1 > x2) {
		//	swap(x1, x2);
			//swap(y1, y2);
	//	}
		road = Road(x1, y1, x2, y2);
		// (x1,y1)  (x2,y2)
		// x-x1/x2-x1  = y-y1/y2-y1

		vec.push_back(road);
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
		cross = max(a, c);
		//	right = min(b, d);
		return true;
	}
	else {
		cross.x = -det(m.c, m.b, n.c, n.b) / zn;
		cross.y = -det(m.a, m.c, n.a, n.c) / zn;
		return betw(a.x, b.x, cross.x)
			&& betw(a.y, b.y, cross.y)
			&& betw(c.x, d.x, cross.x)
			&& betw(c.y, d.y, cross.y);
	}
}

inline void convert_from_segment_to_pt(const Road& seg, pt &a, pt &b) {
	a = seg.first_pt;
	b = seg.second_pt;
}

inline void norm_pt(double a, double b, double c) {
	double z = sqrt(a*a + b*b);
	if (abs(z) > EPS)
		a /= z, b /= z, c /= z;
}
inline bool compare(double a, double b) {
	return abs(b - a) <= EPS;
}
double count_angle(pt &a, pt &b, pt &c, pt &d) {
	Line ln1 = Line(a, b), ln2 = Line(c, d);

	double angle = (ln1.a*ln2.a + ln1.b*ln2.b) / ((sqrt(ln1.a*ln1.a + ln1.b*ln1.b))*sqrt(ln2.a*ln2.a + ln2.b*ln2.b));
	if (compare(angle,1.0))
		angle = 1.0;
	double result = acos(angle) / Pi*180.0;
	return (compare(result, 180.0)) ? 2e9 : result;
}

void find_intersections(vector<Road> &vec, map <pt, int> &crosses) {
	int n = vec.size();
	int cnt = 0;
	for (int i = 0;i < n;++i) {

		for (int j = i + 1;j < n;++j) {

			pt a, b, c, d, cross_point;
			convert_from_segment_to_pt(vec[i], a, b);
			convert_from_segment_to_pt(vec[j], c, d);
			if (intersect(a, b, c, d, cross_point)) {
				auto temp = crosses.find(cross_point);
				if (temp == crosses.end()) {
					crosses.emplace(make_pair(cross_point, cnt));
					vec[i].intersections.push_back(cnt);
					vec[j].intersections.push_back(cnt);
					cnt++;
				}
				else {
					vec[i].intersections.push_back(temp->second);
					vec[j].intersections.push_back(temp->second);
				}

			}
		}
	}
}

void delete_same_crosses(vector<Road> &vec) {
	for (int i = 0;i < vec.size();++i)
		sort(vec[i].intersections.begin(), vec[i].intersections.end());
	for (int i = 0;i < vec.size();++i)
		vec[i].intersections.resize(unique(vec[i].intersections.begin(), vec[i].intersections.end()) - vec[i].intersections.begin());
}

void create_start_end_crosses(vector<Road> &vec, map<pt, int> &crosses) {

	crosses.emplace(Vintik, crosses.size());
	for (size_t i = 0; i < vec.size(); ++i) {
		pt a, b;
		convert_from_segment_to_pt(vec[i], a, b);
		Line ln = Line(a, b);
		if ((ln.a*Vintik.x + ln.b*Vintik.y + ln.c) == 0)
			vec[i].intersections.push_back(crosses.size() - 1);
	}

	crosses.emplace(Shpuntik, crosses.size());
	for (size_t i = 0; i < vec.size(); ++i) {
		pt a, b;
		convert_from_segment_to_pt(vec[i], a, b);
		Line ln = Line(a, b);
		if ((ln.a*Shpuntik.x + ln.b*Shpuntik.y + ln.c) == 0)
			vec[i].intersections.push_back(crosses.size() - 1);
	}
}

void convert_from_map_to_cross_vector(map<pt, int> &crosses, vector<pt> &intersections) {
	intersections.resize(crosses.size());
	for (auto &i : crosses) {
		intersections[i.second] = i.first;
	}
}

void sort_roads_intersections(vector<Road> &vec, vector<pt> crosses_vector) {
	for (int i = 0;i < vec.size();++i) {
		sort(vec[i].intersections.begin(), vec[i].intersections.end(), [&](int a, int b) {
			return (crosses_vector[a] < crosses_vector[b]);
		});
	}
}

void create_graph(vector <Road> &vec, vector<vector<int>> &g) {
	g.resize(cross_vector.size());
	for (int i = 0; i < vec.size();++i) {
		for (int j = 0; j < vec[i].intersections.size() - 1;++j) {
			int v = vec[i].intersections[j];
			int to = vec[i].intersections[j + 1];
			g[v].push_back(to);
			g[to].push_back(v);
		}
	}
}

double dijkstra(const vector<pt> &crosses, vector<vector<int>> &g, int start, int end) {
	int n = crosses.size();
	vector<vector<double>> d(n, vector<double>(n, INF));
	vector<vector<bool>> used(n, vector<bool>(n, false));



	priority_queue<pair<double, pair<int, int>>> q;

	for (auto road : g[start]) {
		d[road][start] = 0.0;
		q.emplace(0.0, make_pair(start, road));
	}

	while (!q.empty()) {
		pair<double, pair<int, int>> v = q.top();
		q.pop();

		pair<int, int> road = v.second;
		if (used[road.second][road.first]) //if try to go reverse
			continue;

		used[road.first][road.second] = true;
		used[road.second][road.first] = true;

		for (int j = 0; j < g[road.second].size();++j) {
			int to = g[road.second][j];

			if (to == road.first)
				continue;

			if (used[road.second][to])
				continue;

			pt pt1, pt2, pt3, pt4;
			pt1 = crosses[road.first];
			pt2 = crosses[road.second];
			pt3 = crosses[road.second];
			pt4 = crosses[to];

			double angle = count_angle(pt1, pt2, pt3, pt4);
			double temp = d[road.second][road.first] + angle;
			double& tempdistance = d[to][road.second];

			if (tempdistance > temp) {
				tempdistance = temp;
				q.emplace(-tempdistance, make_pair(road.second, to));
			}

		}
	}
	double result = *min_element(d[end].begin(), d[end].end());
	if (result < INF)
		return result;
	else return -1;

}

void print_roads() {
	for (size_t i = 0; i < v.size(); i++)
	{
		cout << "road[" << i << "] has following intersections: ";
		//cout << "road[" << i << "] coordinates: " << v[i].first_pt.x << "  " << v[i].first_pt.y << "  " << v[i].second_pt.x << "  " << v[i].second_pt.y << endl;
		for (size_t j = 0; j < v[i].intersections.size(); j++)
		{
			cout << v[i].intersections[j] << ' ';
		}
		cout << endl;
	}
}

double result;
int main() {

	create_vector(v);

	find_intersections(v, cross_list);

	delete_same_crosses(v);

	create_start_end_crosses(v, cross_list);

	convert_from_map_to_cross_vector(cross_list, cross_vector);

	sort_roads_intersections(v, cross_vector);

	create_graph(v, adj);

	result = dijkstra(cross_vector, adj, cross_vector.size() - 2, cross_list.size() - 1);

	fout << result;

	

	//print_roads();

	//cout << endl;
	return EXIT_SUCCESS;

}