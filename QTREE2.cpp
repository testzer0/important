#include <bits/stdc++.h>
using namespace std;

#define ancestor(u,v) (u->timein <= v->timein && u->timeout >= v->timeout)

struct node{
	int id;
	int depth;
	int subtree_sz;
	int timein;
	int timeout;
	int parcost;
	vector<node*> nodesonpath;
	vector<node*> neighbors;
	vector<node*> binlift;
	vector<int> costs;
	vector<long long int> costsums;
	node* parent;
	node* headnode;
};

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
		n->neighbors[i]->parcost = n->costs[i];
		dfs(n->neighbors[i],n);
		n->subtree_sz += n->neighbors[i]->subtree_sz;
	}
	lcanodes.pop_back();
	globtime++;
	n->timeout = globtime;
}

void dfs_hld(node* n, node* parent, node* headnode, long long int sum){
	n->headnode = headnode;
	headnode->nodesonpath.push_back(n);
	n->headnode->costsums.push_back(sum);
	for(auto &n1 : n->neighbors){
		if(n1 == parent) continue;
		if(n1->subtree_sz > (n->subtree_sz - 1)/2)
			dfs_hld(n1,n,headnode,sum+n1->parcost);
		else 
			dfs_hld(n1,n,n1,0);
	}
}

node* lca(node* n1, node* n2){
	//cout << 1+n1->id << " " << 1+n2->id << endl;
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

long long int upsum(node* n, int k){
	//sum upto k nodes
	//cout << 1+n->id << ' ' << k << endl;
	if(k <= 0) return 0;
	int jump = n->depth - n->headnode->depth;
	if(!jump){
		//long long int ret = n->parcost + upsum(n->parent,k-1);
		//cout << ret << endl;
		//return ret;
		return n->parcost + upsum(n->parent,k-1);
	}
	else if(k >= jump){
		//long long int ret = n->headnode->costsums[jump] + upsum(n->headnode,k-jump);
		//cout << ret << endl;
		//return ret;
		return n->headnode->costsums[jump] + upsum(n->headnode,k-jump);
	}
	else{
		//long long int ret = (n->headnode->costsums[jump] - n->headnode->costsums[jump-k]);
		//cout << ret << endl;
		//return ret;
		return (n->headnode->costsums[jump] - n->headnode->costsums[jump-k]);
	}
}

long long int distance(node* n1, node* n2){
	node* p = lca(n1,n2);
	int a = n1->depth - p->depth, b = n2->depth - p->depth;
	return upsum(n1,a)+upsum(n2,b);
}

vector<node*> nodes;

void addEdge(int a, int b, int cost){
	nodes[a]->neighbors.push_back(nodes[b]);
	nodes[b]->neighbors.push_back(nodes[a]);
	nodes[a]->costs.push_back(cost);
	nodes[b]->costs.push_back(cost);
}

int upkth(node* n, int k){
	//cout << 1+n->id << " " << k << endl;
	if(k <= 0) return -1;

	if(k == 1) return 1+n->id;
	int jump = n->depth - n->headnode->depth;
	if(!jump) return upkth(n->parent,k-1);
	else if(k >= jump+1)
		return upkth(n->headnode,k-jump);
	else{
		return 1+(n->headnode->nodesonpath[1+jump-k])->id;
	}
}

int kth(node* n1, node* n2, int k){
	node* p = lca(n1,n2);
	if(k <= n1->depth - n2->depth + 1)
		return upkth(n1,k);
	else{
		k -= n1->depth - p->depth;
		//now find kth node on p to n2 path
		k = (n2->depth - p->depth + 2 - k);
		return upkth(n2, k);
	}
}

void solve(){
	globtime = x = y = z = 0;
	int N;
	cin >> N;
	nodes.resize(N);
	for(int i = 0; i < N; i++)
		nodes[i] = newNode(i);
	for(int i = 0; i < N-1; i++){
		cin >> x >> y >> z;
		addEdge(x-1,y-1,z);
	}
	dfs(nodes[0],NULL);
	dfs_hld(nodes[0],NULL,nodes[0],0);
	string query;
	while(true){
		cin >> query;
		if(query == "DONE"){
			cout << endl;
			break;
		}
		else if(query == "DIST"){
			cin >> x >> y;
			cout << distance(nodes[x-1],nodes[y-1]) << endl;
		}
		else if(query == "KTH"){
			cin >> x >> y >> z;
			cout << kth(nodes[x-1],nodes[y-1],z) << endl;
		}
	}
}

int main(){
	ios_base::sync_with_stdio(false);
	cin.tie(NULL);
	int T;
	cin >> T;
	while(T--)
		solve();
	return 0;
}