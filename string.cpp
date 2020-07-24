#include <bits/stdc++.h>
using namespace std;

long long int compute_hash(string const &s) {
	const int p = 31;
	const int m = 1e9+7;
	long long int hsh = 0;
	long long int ppow = 1;
	for (char c : s) {
		hsh = (hsh + (c-'a'+1)*ppow)%m;
		ppow = (ppow*p)%m;
	}
	return hsh;
}

int *prefix_fn(string s) {
	int n = s.length();
	int *pref = new int[n];
	pref[0] = 0;
	for (int i = 1; i < n; i++) {
		int j = pref[i-1];
		while (j > 0 && s[i] != s[j])
			j = pref[j-1];
		if (s[i] == s[j])
			j++;
		pref[i] = j;
	}
	return pref;
}

int **prefix_automaton(string s) {
	// size -> n x 26
	s += '#';
	int n = s.size();
	int *pf = prefix_fn(s);
	int **aut = new int*[n];
	for (int i = 0; i < n; i++) {
		aut[i] = new int[26];
		for (int c = 0; c < 26; c++) {
			if (i && 'a'+ c != s[i])
				aut[i][c] = aut[pf[i-1]][c];
			else 
				aut[i][c] = i + ('a' + c == s[i]);
		}
	}
	return aut;
}

int *z_fn(string s) {
	int n = s.length();
	int *z = new int[n];
	z[0] = 0;
	for (int i = 1, l = 0, r = 0; i < n; ++i) {
		if (i <= r) 
			z[i] = min(r-i+1, z[i-l]);
		while (i + z[i] < n && s[z[i]] == s[i+z[i]])
			++z[i];
		if (i+z[i]-1 > r) {
			l = i, r = i+z[i]-1;
		}
	}
	return z;
}

int *sort_cyclic_shifts(string const& s) {
	int n = s.size();
	const int alphabet = 256; //replace with a smaller no. if req
	int *p = new int[n], *c = new int[n], cnt[max(alphabet,n)] = {0};
	for (int i = 0; i < n; i++)
		cnt[s[i]]++;
	for (int i = 1; i < alphabet; i++) 
		cnt[i] += cnt[i-1];
	for (int i = 0; i < n; i++)
		p[--cnt[s[i]]] = i;
	c[p[0]] = 1;
	int classes = 1;
	for (int i = 1; i < n; i++) {
		if (s[p[i]] != s[p[i-1]])
			classes++;
		c[p[i]] = classes-1;
	}
	int *pn = new int[n], *cn = new int[n];
	for (int h = 0; (1 << h) < n; h++) {
		for (int i = 0; i < n; i++) {
			pn[i] = p[i] - (1 << h);
			if (pn[i] < 0)
				pn[i] += n;
		}
		fill(cnt, cnt+classes, 0);
		for (int i = 0; i < n; i++) 
			cnt[c[pn[i]]]++;
		for (int i = 1; i < n; i++)
			cnt[i] += cnt[i-1];
		for (int i = n-1; i >= 0; i--)
			p[--cnt[c[pn[i]]]] = pn[i];
		cn[p[0]] = 0;
		classes = 1;
		for (int i = 1; i < n; i++) {
			pair<int,int> cur = {c[p[i]], c[(p[i] + (1 << h))%n]};
			pair<int,int> prev = {c[p[i-1]], c[(p[i-1] + (1 << h))%n]};
			if (cur != prev)
				classes++;
			cn[p[i]] = classes-1;
		}
		swap(c,cn);
	}
	delete c,cn,pn;
	return p;
}

int *suffix_array(string s) {
	s += "$";
	int *sorted_shifts = sort_cyclic_shifts(s);
	return (sorted_shifts+1);
}

int *lcp(string const& s, int *p) {
	int n = s.size();
	int rank[n] = {0};
	for (int i = 0; i < n; i++)
		rank[p[i]] = i;
	int k = 0;
	int *lcp = new int[n-1]();
	for (int i = 0; i < n; i++) {
		if (rank[i] == n-1) {
			k = 0;
			continue;
		}
		int j = p[rank[i]+1];
		while (i+k < n && j+k < n && s[i+k] == s[j+k])
			k++;
		lcp[rank[i]] = k;
		if (k)
			k--;
	}
	return lcp;
}

// Aho corasick
const int K = 26;

struct Vertex{
	int next[K];
	bool leaf = false;
	int p = -1;
	char pch;
	int link = -1;
	int go[K];

	Vertex(int p = -1, char ch = '$'): p(p), pch(ch) {
		fill(next, next+K, -1);
		fill(go, go+K, -1);
	}
};

vector<Vertex> t(1);

void add_string(string const& s) {
	int v = 0;
	for (char ch : s) {
		int c = ch - 'a';
		if (t[v].next[c] == -1) {
			t[v].next[c] = t.size();
			t.emplace_back(v, ch);
		}
		v = t[v].next[c];
	}
	t[v].leaf = true;
}

int go(int v, char ch);

int get_link(int v) {
	if (t[v].link == -1) {
		if (v == 0 || t[v].p == 0)
			t[v].link = 0;
		else 
			t[v].link = go(get_link(t[v].p), t[v].pch);
	}
	return t[v].link;
}

int go(int v, char ch) {
	int c = ch - 'a';
	if (t[v].go[c] == -1) {
		if (t[v].next[c] != -1)
			t[v].go[c] = t[v].next[c];
		else
			t[v].go[c] = v==0? 0 : go(get_link(v), ch);
	}
	return t[v].go[c];
}

// aho corasick done

// ukkonen for suffix tree from cp-algo
const int N = 1000000, inf = 1000000000; 	// maximum possible nodes in s-tree
string a;		  	// input string for which stree is built
int tree[N][26], 	// array of transitions
	l[N], 			// left and
	r[N], 			// right boundaries of substring corresponding to an incoming edge
	p[N], 			// parent of node
	s[N], 			// suffix link
	tv, 			// node of current suffix (if we are mid-edge then lower node of edge)
	tp, 			// position in the string which corresponds to position on edge between l[tv] and r[tv]
	ts, 			// number of nodes
	la; 			// current character in string	

void ukkadd(int c) {	// add character c to tree
	suff:; 				// we'll return to this label after each transition to suffix (and add a character again)
	if (r[tv] < tp) {	// check if we're still within boundaries of current edge
		// If we're not, then find the next edge. If it doesnt exist then create a leaf and add it to the tree
		if (tree[tv][c] == -1) {
			tree[tv][c] = ts; 	
			l[ts] = la;
			p[ts++] = tv;
			tv = s[tv];
			tp = r[tv]+1;
			goto suff;
		}
		tv = tree[tv][c];
		tp = l[tv];
	}	// otherwise just proceed to the next edge
	if (tp == -1 || c == a[tp]-'a') {
		tp++; 	// if the letter in the edge is c then go down the edge
	}
	else {
		// otherwise split the edge in two with the middle in node ts
		l[ts] = l[tv];
		r[ts] = tp-1;
		p[ts] = p[tv];
		tree[ts][a[tp]-'a'] = tv;
		// add leaf ts+1. It corresponds to transition through c.
		tree[ts][c] = ts+1;
		l[ts+1] = la;
		p[ts+1] = ts;
		// update info for current node - remember to mark ts as parent of tv.
		l[tv] = tp;
		p[tv] = ts;
		tree[p[ts]][a[l[ts]] - 'a'] = ts;
		ts += 2;
		// prepare for descent
		// tp will mark where we are in current suffix
		tv = s[p[ts-2]];
		tp = l[ts-2];
		// while current suffix is not over, descend
		while (tp <= r[ts-2]) {
			tv = tree[tv][a[tp]-'a'];
			tp += r[tv]-l[tv]+1;
		}
		// if we're in a node, we'll add a suffic link to it. Otherwise add the link to ts.
		// (we'll create ts on next iteration.)
		if (tp == r[ts-2]+1)
			s[ts-2] = tv;
		else
			s[ts-2] = ts;
		// add tp to the new edge and return to add letter to suffix
		tp = r[tv] - (tp - r[ts-2]) + 2;
		goto suff;
	}
}

void build() {
	ts = 2;
	tv = tp = 0;
	fill(r,r+N,(int)a.size()-1);
	// initialize data for root of tree
	s[0] = 1;
	l[0] = r[0] = l[1] = r[1] = -1;
	memset(tree, -1, sizeof(tree));
	fill(tree[1], tree[1]+26, 0);
	// add the text to the tree letter by letter
	for (la = 0; la < a.size(); ++la)
		ukkadd(a[la]-'a');
}

// ukkonen finished

// suffix automaton

struct state{
	int len, link;
	map<char,int> next; 	// replace with array for alphabets
};

const int MAXLEN = 1000000;
state st[MAXLEN*2];
int sz, last;

void sa_init() {
	st[0].len = 0;
	st[0].link = -1;
	sz++;
	last = 0;
}

void sa_extend(char c) {
	int cur = sz++;
	st[cur].len = st[last].len + 1;
	int p = last;
	while (p != -1 && !st[p].next.count(c)) {
		st[p].next[c] = cur;
		p = st[p].link;
	}
	if (p == -1) {
		st[cur].link = 0;
	}
	else {
		int q = st[p].next[c];
		if (st[p].len + 1 == st[q].len) {
			st[cur].link = q;
		}
		else {
			int clone = sz++;
			st[clone].len = st[p].len + 1;
			st[clone].next = st[q].next;
			st[clone].link = st[q].link;
			while (p != -1 && st[p].next[c] == q) {
				st[p].next[c] = clone;
				p = st[p].link;
			}
			st[q].link = st[cur].link = clone;
		}
	}
	last = cur;
}

// suffix automaton end

// lyndon factorization:
// A word w is called simple or lyndon if and only if w is strictly lexicographically smaller than all of its
// proper suffixes. 
// A lyndon factorization of a word w is a (smallest) decomposition of s into w_1 w_2 ... w_k such that each of the w_i are Lyndon
// and the w_i's are strictly lexicographically decreasing. It can be shown that each string has a unique lyndon factorization.

// duval algorithm for lyndon factorization - O(n) time and O(1) additional memory

// for more see https://cp-algorithms.com/string/lyndon_factorization.html
// eg find w where s = w + w + w + w + .. + w + (pref of w)
// or find smallest cyclic shift in O(n) time.

vector<string> duval(string const& s) {
	int n = s.size();
	int i = 0;
	vector<string> factorization;
	while (i < n) {
		int j = i+1, k = i;
		while (j < n && s[k] <= s[j]) {
			if (s[k] < s[j])
				k = i;
			else
				k++;
			j++;
		}
		while (i <= k) {
			factorization.push_back(s.substr(i,j-k));
			i += j-k;
		}
	}
	return factorization;
} 


int main() {
	string S = "abcabc";
	sa_init();
	for (auto &c : S)
		sa_extend(c);
	for (int i = 0; i < 15; i++) {
		cout << i << ' ' << st[i].len << ' ' << st[i].link << endl;
		for (auto &a : st[i].next) {
			cout << "\t\t" << a.first << ' ' << a.second << endl;
		}
	}
	return 0;
}