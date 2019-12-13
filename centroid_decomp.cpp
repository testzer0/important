#include <bits/stdc++.h>
using namespace std;

struct node{
	int id;
	int subtree_sz;
	bool marked;
	node* consider;
	node* cd_parent;
	vector<node*> neighbors;
	vector<node*> cd_neighbors;
};

node* cent_root;

node* newNode(int id){
	node* n = new node;
	n->id = id;
	n->subtree_sz = 1;
	n->marked = false;
	return n;
}

void addEdge(node* &a, node* &b){
	a->neighbors.push_back(b);
	b->neighbors.push_back(a);
}

void dfs(node* &n, node* parent){
	for(auto &n1 : n->neighbors){
		if(n1 == parent) continue;
		dfs(n1,n);
		n->subtree_sz += n1->subtree_sz;
	}
}

void centroid_decomp(node* root, node* n){
	//cout << "considering " << 1+n->id;
	//if(root) cout << " for " << 1+root->id;
	//cout << endl;
	n->consider = NULL;
	for(auto &n1 : n->neighbors){
		if(n1->marked) continue;
		if(n1->subtree_sz > n->subtree_sz/2){
			n->consider = n1;
		}
	}
	if(!n->consider){
		//cout << "chosen " << n->id + 1 << endl;
		n->marked = true;
		n->cd_parent = root;
		if(root) root->cd_neighbors.push_back(n);
		else cent_root = n;
		for(auto &n1 : n->neighbors){
			//cout << n->id << ' ' << n1->id << endl;
			if(!n1->marked)	centroid_decomp(n,n1);
		}
	}
	else{
		n->subtree_sz -= n->consider->subtree_sz;
		n->consider->subtree_sz += n->subtree_sz;
		//cout << n->subtree_sz << ' ' << n->consider->subtree_sz << endl;

		centroid_decomp(root,n->consider);
	}
}

vector<node*> nodes;

int main(){
	ios_base::sync_with_stdio(false);
	cin.tie(NULL);
	int N;
	cin >> N;
	nodes.resize(N);
	for(int i = 0; i < N; i++)
		nodes[i] = newNode(i);
	int x,y;
	for(int i = 0; i < N-1; i++){
		cin >> x >> y;
		addEdge(nodes[x-1],nodes[y-1]);
	}
	dfs(nodes[0],NULL);
	centroid_decomp(NULL,nodes[0]);
	for(auto &n : nodes){
		cout << 1+n->id << ' ';
		if(n->cd_parent) cout << 1+n->cd_parent->id << ' ';
		cout << endl;
	}
	return 0;
}