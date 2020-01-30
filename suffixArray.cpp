#include <bits/stdc++.h>
using namespace std;

vector<int> sort_cyclic_shifts(string const &S){
	int N = S.size();
	int alphabet = 256;
	vector<int> p(N), c(N), cnt(max(alphabet,N),0);
	for(int i = 0; i < N; i++)
		cnt[S[i]]++;
	for(int i = 1; i < alphabet; i++)
		cnt[i] += cnt[i-1];
	for(int i = 0; i < N; i++){
		p[--cnt[S[i]]] = i;
	}
	c[p[0]] = 0;
	int classes = 1;
	for(int i = 1; i < N; i++){
		if(S[p[i]] != S[p[i-1]]) classes++;
		c[p[i]] = classes - 1;
	}
	vector<int> pn(N), cn(N);
	for(int h = 0; (1 << h) < N; h++){
		for(int i = 0; i < N; i++){
			pn[i] = p[i] - (1 << h);
			if(pn[i] < 0) pn[i] += N;
		}
		fill(cnt.begin(), cnt.begin() + classes, 0);
		for(int i = 0; i < N; i++){
			cnt[c[pn[i]]]++;
		}
		for(int i = 1; i < classes; i++)
			cnt[i] += cnt[i-1];
		for(int i =N-1; i >= 0; i--){
			p[--cnt[c[pn[i]]]] = pn[i];
		}
		cn[p[0]] = 0;
		classes = 1;
		for(int i = 1; i < N; i++){
			pair<int,int> cur = {c[p[i]], c[(p[i] + (1 << h))%N]};
			pair<int,int> prev = {c[p[i-1]], c[(p[i-1] + (1 << h))%N]};
			if(cur != prev)
				classes++;
			cn[p[i]] = classes - 1;
		}
		c.swap(cn);
	}
	return p;
}

vector<int> suffix_array_construction(string S){
	S += "$";
	vector<int> sorted_shifts = sort_cyclic_shifts(S);
	sorted_shifts.erase(sorted_shifts.begin());
	return sorted_shifts;
}

int main(){
	ios_base::sync_with_stdio(false);
	cin.tie(NULL);
	string S;
	cin >> S;
	vector<int> sa = suffix_array_construction(S);
	for(int i = 0; i < sa.size(); i++)
		cout << sa[i] << ' ';
	cout << endl;	
	return 0;
}