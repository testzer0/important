#include <bits/stdc++.h>
using namespace std;

const int K = 26;

struct vertex{
	int next[K];
	bool leaf = false; //leaf signifies that current string exists in trie
	int p = -1;
	char pch;
	int link = -1;
	int go[K];

	vertex(int p = -1, char ch = '$'): p(p), pch(ch){
		fill(begin(next), end(next), -1);
		fill(begin(go), end(go), -1);
	}
};

vector<vertex> trie(1);

void add_String(string &s){
	int v = 0;
	for(char ch : s){
		int c = ch - 'a';
		if(trie[v].next[c] == -1){
			trie[v].next[c] = trie.size();
			trie.emplace_back(v,ch);
		}
		v = trie[v].next[c];
	}
	trie[v].leaf = true;
}

int go(int v, char ch);

int get_link(int v){
	if(trie[v].link == -1){
		if(v == 0 || trie[v].p == 0)
			trie[v].link = 0;
		else
			trie[v].link = go(get_link(trie[v].p), trie[v].pch);
	}
	return trie[v].link;
}

int go(int v, char ch){
	int c = ch - 'a';
	if(trie[v].go[c] == -1){
		if(trie[v].next[c] != -1)
			trie[v].go[c] = trie[v].next[c];
		else
			trie[v].go[c] = (c == 0)? 0 : go(get_link(v),ch);
	}
	return trie[v].go[c];
}

int main(){
	ios_base::sync_with_stdio(false);
	cin.tie(NULL);
	return 0;
}