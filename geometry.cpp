#include <bits/stdc++.h>
using namespace std;

struct point{
	double x;
	double y;
}

typedef vector<point> polygon;

double area(polygon P){
	double ar = 0;
	for(int i = 0; i < P.size() - 1; i++){
		ar += 0.5*(P[i].x - P[i+1].x)*(P[i].y + P[i+1].y);
	}
	ar += 0.5*(P[P.size()-1].x - P[0].x)*(P[P.size()-1].y + P[0].y);
	return fabs(ar);
}

int main(){
	ios_base::sync_with_stdio(false);
	cin.tie(NULL);
	return 0;
}