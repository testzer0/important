#include <bits/stdc++.h>
using namespace std;

#define ancestor(u,v) (u->timein <= v->timein && u->timeout >= v->timeout)


struct node{
	int id;
	int depth;
	int subtree_sz;
	int halfsize;
	int timein;
	int timeout;
	int parcost;
	vector<node*> nodesonpath;
	vector<node*> neighbors;
	vector<int> costs;
	vector<node*> binlift;
	int* segtree;
	node* parent;
	node* headnode;
	node* head_max;
};

node** nodes;
pair<int,int>* edges;

node* newNode(int id){
	node* n = new node;
	n->id = id;
	n->parent = n->headnode = NULL;
	return n;
}

vector<node*> lcanodes;
int globtime = 0;
int x,y,z;

void dfs(node* n, node* parent){
	if(!parent){
		n->depth = 0;
		n->parcost = 0;
	}
	else n->depth = parent->depth + 1;
	globtime++;
	n->timein = globtime;
	//sets up subtree sizes, lca stuff
	n->parent = parent;
	n->subtree_sz = 1;
	x = 1, y = lcanodes.size();
	while(x <= y){
		n->binlift.push_back(lcanodes[y-x]);
		x <<= 1;
	}

	lcanodes.push_back(n);
	for(int i = 0; i < n->neighbors.size(); i++){
		if(n->neighbors[i] == parent) continue;
		n->neighbors[i]->parent = n;
		n->neighbors[i]->parcost = n->costs[i];
		dfs(n->neighbors[i],n);
		n->subtree_sz += n->neighbors[i]->subtree_sz;
	}
	lcanodes.pop_back();
	globtime++;
	n->timeout = globtime;
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
		n->segtree = new int[2*x];
		int i = 0;
		while(i < n->nodesonpath.size()){
			n->segtree[i+x] = n->nodesonpath[i]->parcost; 
			i++;
		}
		while(i < x){
			n->segtree[i+x] = 0;
			i++;
		}
		i = x-1;
		while(i){
			n->segtree[i] = n->segtree[i << 1] + n->segtree[1 + (i << 1)];
			i--;
		}
	}
}

node* lca(node* n1, node* n2){
	if(ancestor(n1,n2))
		return n1;
	else if(ancestor(n2,n1))
		return n2;
	int l = n1->binlift.size() - 1;
	while(ancestor(n1->binlift[l],n2)){
		l--;
		if(l < 0) break;
	}
	if(l >= 0) return lca(n1->binlift[l]->parent, n2);
	else return lca(n1->parent,n2);
}

void update(int* segtree, int n, int pos, int value){
	// n = size of segtree/2
	pos += n;
	segtree[pos] = value;
	pos >>= 1;
	while(pos){
		segtree[pos] = (segtree[pos << 1] + segtree[1 + (pos << 1)]);
		pos >>= 1;
	}
}


int query(int* segtree, int n, int l, int r){
	int retval = 0;
	r++;
	for(l += n, r += n; l < r; l >>= 1, r >>= 1){
		if(l & 1) retval += segtree[l++];
		if(r & 1) retval += segtree[--r];
	}
	return retval;
}

void parupdate(node* n, int newcost){
	node* head = n->headnode;
	update(head->segtree, head->halfsize, n->depth - head->depth, newcost);
}

void edgeupdate(int a, int newcost){
	x = edges[a].first, y = edges[a].second;
	if(nodes[x]->parent == nodes[y]) parupdate(nodes[x], newcost);
	else parupdate(nodes[y],newcost);
}

int upquery(node* n, int uptoDepth){
	if(n->depth <= uptoDepth) return 0;
	node* head = n->headnode;
	int retval = 0;
	if(uptoDepth < head->depth){
		retval += query(head->segtree,head->halfsize,0,n->depth - head->depth);
		retval += upquery(head->parent,uptoDepth);
	}
	else retval += query(head->segtree, head->halfsize, uptoDepth - head->depth+1, n->depth - head->depth);
	return retval;
}

int pathquery(node* n1, node* n2){
	int uptoDepth = lca(n1,n2)->depth;
	int retval = upquery(n1,uptoDepth);
	retval += upquery(n2, uptoDepth);
	return retval;
}


void addEdge(int a, int b, int cost, int i){
	nodes[a]->neighbors.push_back(nodes[b]);
	nodes[b]->neighbors.push_back(nodes[a]);
	nodes[a]->costs.push_back(cost);
	nodes[b]->costs.push_back(cost);
	edges[i] = make_pair(a,b);
}

int main(){
	ios_base::sync_with_stdio(false);
	cin.tie(NULL);
	int N, Q;
	cin >> N;
	nodes = new node*[N];
	edges = new pair<int,int>[N-1];
	for(int i = 0; i < N; i++)
		nodes[i] = newNode(i);
	for(int i = 0; i < N-1; i++){
		cin >> x >> y >> z;
		addEdge(x-1,y-1,z,i);
	}
	dfs(nodes[0], NULL);
	dfs_hld(nodes[0], NULL, nodes[0]);
	cin >> Q;
	while(Q--){
		cin >> x >> y >> z;
		if(x == 1){
			edgeupdate(y-1,z);
		}
		else if(x == 2){
			cout << pathquery(nodes[y-1],nodes[z-1]) << endl;
		}
	}
	return 0;
}