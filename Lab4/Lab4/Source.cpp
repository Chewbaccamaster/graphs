#include <iostream>
#include <stdio.h>
#include <fstream>
#include <vector>
#include <algorithm>

using namespace std;

ifstream fin("input.txt");
ofstream fout("output.txt");
int H, V;


struct Word
{
	int r;
	int c1;
	int c2;
}; 

vector<Word> v1, v2;


void create_vector(vector<Word> &vec1, vector<Word> &vec2) {
	fin >> H >> V;
	for (int i = 0;i < H; ++i) {
		Word temp;
		int a, b, c;
		fin >> a >> b >> c;
		temp.r = a;
		temp.c1 = b;
		temp.c2 = c;

		vec1.emplace_back(temp);
	}
	for (int i = 0;i < V; ++i) {
		Word temp;
		int a, b, c;
		fin >> a >> b >> c;
		temp.r = a;
		temp.c1 = b;
		temp.c2 = c;

		vec2.emplace_back(temp);
	}
		
}


int main() {

	create_vector(v1, v2);
	
	return EXIT_SUCCESS;
}