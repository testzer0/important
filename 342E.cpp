#include <bits/stdc++.h>
using namespace std;

struct node{
	int id;
	int subtree_sz;
	int minred;
	int color;
	bool marked;
	node* consider;
	node* cd_parent;
	vector<node*> neighbors;
	//vector<node*> cd_neighbors;
	vector<int> par_distances;
};

//node* cent_root;

node* newNode(int id){
	node* n = new node;
	n->id = id;
	n->subtree_sz = 1;
	n->marked = false;
	n->minred = 1000000;
	n->color = 0;
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

void cent_dfs(node* &n, node* parent, int distance){
	n->par_distances.push_back(distance);
	for(auto &n1 : n->neighbors){
		if(n1 == parent || n1->marked) continue;
		cent_dfs(n1,n,distance+1);
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
		//if(root) root->cd_neighbors.push_back(n);
		//else cent_root = n;
		cent_dfs(n,NULL,0);
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

void update(node* &n){
	n->color = 1;
	node* n1 = n;
	int i = n->par_distances.size() - 1;
	while(i >= 0){
		n1->minred = min(n1->minred, n->par_distances[i]);
		n1 = n1->cd_parent;
		i--;
	}
}

int mindis(node* n){
	int min_dst = INT_MAX;
	node* n1  = n;
	int i = n->par_distances.size() - 1;
	while(i >= 0){
		min_dst = min(min_dst, n->par_distances[i] + n1->minred);
		//if(n->id == 11) cout << n->par_distances[i] + n1->minred << endl;
		n1 = n1->cd_parent;
		i--;
	}
	return min_dst;
}

int main(){
	ios_base::sync_with_stdio(false);
	cin.tie(NULL);
	int N, M;
	cin >> N >> M;
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
	update(nodes[0]);
	int que, nod;
	/*for(auto &n : nodes){
		cout << n->id +1 << " ---------" << endl;
		for(auto &i : n->par_distances)
			cout << i << ' ';
		cout << endl;
	}*/
	while(M--){
		cin >> que >> nod;
		if(que == 1) update(nodes[nod-1]);
		else cout << mindis(nodes[nod-1]) << endl;
	}
	return 0;
}