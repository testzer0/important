#include <bits/stdc++.h>
using namespace std;

int* sort_cyclic_shifts(string const &S, int N){
	//int N = S.size();
	const int alphabet = 256;
	int *p = new int[N], *c = new int[N], *cnt = new int[max(alphabet,N)];
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
	int *pn = new int[N], *cn = new int[N];
	for(int h = 0; (1 << h) < N; h++){
		for(int i = 0; i < N; i++){
			pn[i] = p[i] - (1 << h);
			if(pn[i] < 0) pn[i] += N;
		}
		fill(cnt, cnt + classes, 0);
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
		swap(c,cn);
	}
	return p;
}

int* suffix_array_construction(string S){
	S += "$";
	int* sorted_shifts = sort_cyclic_shifts(S, S.size());
	sorted_shifts++;
	return sorted_shifts;
}

int* lcp_construction(string const& S, int* const& p) {
    int N = S.size();
    int* rank = new int[N];
    for (int i = 0; i < N; i++)
        rank[p[i]] = i;

    int k = 0;
    int* lcp = new int[N-1];
    for (int i = 0; i < N; i++) {
        if (rank[i] == N - 1) {
            k = 0;
            continue;
        }
        int j = p[rank[i] + 1];
        while (i + k < N && j + k < N && S[i+k] == S[j+k])
            k++;
        lcp[rank[i]] = k;
        if (k)
            k--;
    }
    return lcp;
}
int main(){
	ios_base::sync_with_stdio(false);
	cin.tie(NULL);
	return 0;
}