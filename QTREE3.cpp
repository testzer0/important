#include <bits/stdc++.h>
using namespace std;

struct node{
	int id;
	int depth;
	int subtree_sz;
	int color; //0 is white, 1 is black
	vector<node*> nodesonpath;
	vector<node*> neighbors;
	set<int> blacknodes;
	node* parent;
	node* headnode; // head of heavy-path
};

node* newNode(int id){
	node* n = new node;
	n->id = id;
	n->parent = n->headnode = NULL;
	n->color = 0;
	return n;
}

int x,y,z;

void dfs(node* n, node* parent){
	if(!parent){
		n->depth = 0; //root
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
		if(n1->subtree_sz > (n->subtree_sz - 1)/2)
			dfs_hld(n1,n,headnode);
		else 
			dfs_hld(n1,n,n1);
	}
}

vector<node*> nodes;

void change(node* &n){
	if(n->color == 0){
		//white
		n->color = 1;
		n->headnode->blacknodes.insert(n->depth);
	}
	else{
		//black
		n->color = 0;
		n->headnode->blacknodes.erase(n->depth);
	}
}

int findmin(node* &n){
	int minval = -2, temp;
	node *ncur = n, *ntemp;
	while(ncur){
		if(!ncur->headnode->blacknodes.empty()){
			temp = *(ncur->headnode->blacknodes.begin());
			//cout << temp << endl;
			if(temp <= ncur->depth){
				ntemp = ncur->headnode;
				minval = (ntemp->nodesonpath[temp - ntemp->depth])->id;
			}
		}
		ncur = ncur->headnode->parent;
	}
	return 1+minval;
}

void addEdge(int a, int b){
	nodes[a]->neighbors.push_back(nodes[b]);
	nodes[b]->neighbors.push_back(nodes[a]);
}

int main(){
	ios_base::sync_with_stdio(false);
	cin.tie(NULL);

	int N,Q;
	cin >> N >> Q;
	nodes.resize(N);
	for(int i = 0; i < N; i++)
		nodes[i] = newNode(i);

	for(int i = 0; i < N-1; i++){
		cin >> x >> y;
		addEdge(x-1,y-1);
	}
	dfs(nodes[0],NULL);
	dfs_hld(nodes[0],NULL,nodes[0]);
	int query;
	while(Q--){
		cin >> query >> x;
		if(query == 0){
			change(nodes[x-1]);
		}
		else{
			cout << findmin(nodes[x-1]) << endl;
		}
	}

	return 0;
}