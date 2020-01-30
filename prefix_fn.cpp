#include <bits/stdc++.h>
using namespace std;

vector<int> prefix_fn(string S){
	//prefix_fn[i] is the max length of a string which is both a proper prefix and
	// proper suffix of S[0..i]. By definition prefix_fn[0] = 0.
	int N = S.length();
	vector<int> prefix(N);
	for(int i = 1; i < N; i++){
		int curr = prefix[i-1];
		while(curr && S[i] != S[curr])
			curr = prefix[curr-1];
		if(S[i] == S[curr])
			curr++;
		prefix[i] = curr;
	}
	return prefix;
}

vector<int> z_function(string S){
	//z_function[i] is the length of the longest common prefix of S and S[i:]
	//by def z[0] = 0
	//runs in O(N)
	int N = S.length();
	vector<int> z(N);
	for(int i = 1, l = 0, r = 0; i < N; i++){
		if(i <= r)
			z[i] = min(r - i + 1, z[i-l]);
		while(i + z[i] < N && S[z[i]] == S[i+z[i]])
			z[i]++;
		if(i + z[i] - 1 > r)
			l = i, r = i+z[i]-1;
	}
	return z;
}


vector<int> KMP(string pattern, string S){
	string S1 =  pattern + '#' + S;
	vector<int> prefix = prefix_fn(S1);
	vector<int> indices;
	int N = pattern.length();
	for(int i = 0; i < prefix.size(); i++){
		if(prefix[i] == N) indices.push_back(i-2*N);
	}	
	return indices;
}

int main(){
	ios_base::sync_with_stdio(false);
	cin.tie(NULL);
	vector<int> test = KMP("ada","adada");
	for(auto &i : test) cout << i << endl;
	return 0;
}