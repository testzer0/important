#include <bits/stdc++.h>
using namespace std;

struct node{
	int id;
	int depth;
	int subtree_sz;
	//int color; //1 for white, 0 for black
	int halfsize;
	int maxdistance;
	int secondmax;
	vector<node*> nodesonpath;
	vector<node*> neighbors;
	//vector<int> upsegtree; //segtree with point update and range query
	//vector<int> downsegtree;
	int* upsegtree;
	int* downsegtree;
	node* parent;
	node* headnode;
	node* head_max;
};

node* newNode(int id){
	node* n = new node;
	n->id = id;
	n->parent = n->headnode = NULL;
	//n->subtree_sz = 1;
	//n->color = 0;
	n->head_max = NULL;
	n->maxdistance = -100000;
	n->secondmax = -100000;
	return n;
}

int global_max;

/*
int reverse(int num, int logn){
	int res = 0;
	for (int i = 0; i < logn; i++) {
        if (num & (1 << i))
            res |= 1 << (logn - 1 - i);
    }
    return res;
}*/

int x,y,z;

void dfs(node* n, node* parent){
	if(!parent){
		n->depth = 0;
	}
	else n->depth = parent->depth + 1;
	//sets up subtree sizes, lca stuff
	n->parent = parent;
	n->subtree_sz = 1;
	for(int i = 0; i < n->neighbors.size(); i++){
		if(n->neighbors[i] == parent) continue;
		dfs(n->neighbors[i],n);
		n->subtree_sz += n->neighbors[i]->subtree_sz;
	}
}

void dfs_hld(node* n, node* parent, node* headnode){
	n->headnode = headnode;
	headnode->nodesonpath.push_back(n);
	for(auto &n1 : n->neighbors){
		if(n1 == parent) continue;
		if(n1->subtree_sz > (n->subtree_sz - 1)/2){
			dfs_hld(n1,n,headnode);
		}
		else {
			dfs_hld(n1,n,n1);
		}
	}
	if(headnode == n){
		//if(n->id == 74787) cout << 2 << endl;
		x = n->nodesonpath.size();
		//smallest power of 2 >= x; assume x fits in 32 bit

		x--;
		x |= x >> 1;
		x |= x >> 2;
		x |= x >> 4;
		x |= x >> 8;
		x |= x >> 16;
		x++;
		n->halfsize = x;
		//n->upsegtree.resize(x<<1);
		//n->downsegtree.resize(x<<1);
		n->upsegtree = new int[x << 1];
		n->downsegtree = new int[x << 1];
		for(int i = 0; i < x<<1; i++){
			n->upsegtree[i] = n->downsegtree[i] = -100000; //maxdistance initially none
		}
	}
}

/*
void update(vector<int> &segtree, int n, int l, int r, int value){
	//l and r are 0 indexed
	//n = size of segtree/2
	r++;
	for(l += n, r += n; l < r;  l >>= 1, r >>= 1){
		//if(l&1) then l is 
		if(l & 1) segtree[l++] += value;
		if(r & 1) segtree[--r] += value;
	}
}*/

void update(int* segtree, int n, int pos, int value){
	//l and r 0 indexed
	// n = size of segtree/2
	pos += n;
	if(segtree[pos] >= value) return;
	segtree[pos] = value;
	pos >>= 1;
	while(pos){
		//if(value == 5){cout << pos << endl;
		//cout << segtree[pos << 1] << ' ' << segtree[1 + (pos << 1)] << endl;}
		segtree[pos] = max(segtree[pos << 1], segtree[1 + (pos << 1)]);
		pos >>= 1;
	}
}

/*
int query(vector<int> &segtree, int n, int pos){
	int cur = 1, direction = reverse(pos, log2(n));
	while(cur < n){
		segtree[cur << 1] += segtree[cur];
		segtree[1 + (cur << 1)] += segtree[cur];
		segtree[cur] = 0;
		cur <<= 1;
		if(direction & 1) cur++;
		direction >>= 1;
	}
	return segtree[cur];
}*/

int query(int* segtree, int n, int l, int r){
	int retval = -10000;
	r++;
	for(l += n, r += n; l < r; l >>= 1, r >>= 1){
		if(l & 1) retval = max(retval, segtree[l++]);
		if(r & 1) retval = max(retval,segtree[--r]);
	}
	return retval;
}

vector<node*> nodes;

void addEdge(int a, int b){
	nodes[a]->neighbors.push_back(nodes[b]);
	nodes[b]->neighbors.push_back(nodes[a]);
}

void updateNode(node* &n, node* curhead, int distance){
	//cout << "u " << n->id << " " << distance << endl; 
	node* head = n->headnode;
	if(distance > n->maxdistance){
		if(n->head_max == curhead){
			n->maxdistance = distance;
		}
		else{
			n->secondmax = n->maxdistance;
			n->maxdistance = distance;
			n->head_max = curhead;
		}
	}
	else if(distance > n->secondmax){
		if(curhead != n->head_max)
			n->secondmax = distance;
	}
	int pos = n->depth - head->depth;
	update(head->upsegtree, head->halfsize, pos, distance - n->depth);
	update(head->downsegtree, head->halfsize, pos, distance + n->depth);
	if(head->parent){
		updateNode(head->parent, head, distance + pos + 1);
	}
}

int maxinCurrent(node* n, node* headcome){
	int retval, temp, pos;
	if(n->head_max == headcome) retval = n->secondmax;
	else retval = n->maxdistance;
	//cout << "----" << endl;
	node* head = n->headnode;
	pos = n->depth - head->depth;
	if(pos){
		temp = query(head->upsegtree, head->halfsize, 0, pos - 1);
		retval = max(temp + n->depth, retval);
		//cout << retval << endl;
	}
	if(pos < head->nodesonpath.size() - 1){
		temp = query(head->downsegtree, head->halfsize, pos + 1, head->nodesonpath.size() - 1);
		retval = max(temp - n->depth, retval);
	}
	return retval;
}

int maxOverall(node* n, node* headcome){
	int retval = maxinCurrent(n, headcome), temp;
	node* head = n->headnode;
	if(head->parent){
		temp = maxOverall(head->parent, head);
		retval = max(retval, temp + n->depth - head->depth + 1);
	}
	return retval;
}

void makewhite(node* &n, bool print){
	int curmax = maxOverall(n, NULL);
	global_max = max(global_max, curmax);
	//if(print) cout << max(global_max, 0) << endl;
	if(print) printf("%d\n", max(global_max,0));
	//n->color = 1;
	updateNode(n,NULL,0);
}

void solve(){
	global_max = -100000;
	int N;
	//cin >> N;
	scanf("%d", &N);
	nodes.resize(N);
	for(int i = 0; i < N; i++){
		nodes[i] = newNode(i);
	}
	for(int i = 1; i < N; i++){
		//cin >> x;
		scanf("%d",&x);
		addEdge(i, x-1);
	}
	dfs(nodes[0], NULL);
	dfs_hld(nodes[0], NULL, nodes[0]);
	makewhite(nodes[0], false);
	for(int i = 1; i < N; i++){
		makewhite(nodes[i], true);
	}
}

int main(){
	ios_base::sync_with_stdio(false);
	cin.tie(NULL);
	int T;
	//cin >> T;
	scanf("%d",&T);
	while(T--){
		solve();
	}
	return 0;
}