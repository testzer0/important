#include <bits/stdc++.h>
using namespace std;

struct state{
	int len, link;
	map<char, int> next; //add bool terminal; if needed
};

const int MAXLEN = 10000;
state st[MAXLEN*2]; //this is our suffix automaton
int sz, last;

void sa_init(){
	//creates an automaton with a single state
	st[0].link = -1;
	st[0],len = 0;
	sz++;
	last = 0;
}

void sa_extend(char c){
	//appends char c to end of string and updates automaton 
	int cur = sz++;
	st[cur].len = st[last].len + 1;
	int p = last;
	while(p != -1 && !st[p].next.count(c)){
		st[p].next[c] = cur;
		p = st[p].link;
	}
	if(p == -1){
		//we have walked up the automaton succesfully
		st[cur].link = 0;
	}
	else{
		//there exists the suffix in consideration already in the automaton
		int q = st[p].next[c];
		if(st[p].len + 1 == st[q].len){
			//good, dont need to do anything
			st[cur].link = q;
		}
		else{
			//clone this node;
			int clone = sz++;
			st[clone].len = st[p].len + 1;
			st[clone].next = st[q].next;
			st[clone].link = st[q].link;
			while(p != -1 && st[p].next[c] == q){
				st[p].next[c] = clone;
				p = st[p].link;
			}
		}
		st[q].link = st[cur].link = clone;
	}
	last = cur;
}

int main(){
	ios_base::sync_with_stdio(false);
	cin.tie(NULL);
	return 0;
}