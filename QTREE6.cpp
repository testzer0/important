#include <bits/stdc++.h>
using namespace std;

struct node{
	int id;
	int depth;
	int b_subtree_sz;
	int w_subtree_sz;
	int halfsize;
	int color; //1 for white, 0 for black
	vector<node*> nodesonpath;
	vector<node*> neighbors;
	vector<int> bsegtree; //segtree with range updates and point query
	//segtree used only for update values
	vector<int> wsegtree;
	set<int> start_depths;
	node* parent;
	node* headnode;
	node* hldchild;
};

node* newNode(int id){
	node* n = new node;
	n->id = id;
	n->parent = n->headnode = NULL;
	n->w_subtree_sz = 1;
	n->color = 0;
	n->hldchild = NULL;
	return n;
}

int reverse(int num, int logn){
	int res = 0;
	for (int i = 0; i < logn; i++) {
        if (num & (1 << i))
            res |= 1 << (logn - 1 - i);
    }
    return res;
}

int x,y,z;

void dfs(node* n, node* parent){
	if(!parent){
		n->depth = 0;
	}
	else n->depth = parent->depth + 1;
	//sets up subtree sizes, lca stuff
	n->parent = parent;
	n->b_subtree_sz = 1;
	for(int i = 0; i < n->neighbors.size(); i++){
		if(n->neighbors[i] == parent) continue;
		dfs(n->neighbors[i],n);
		n->b_subtree_sz += n->neighbors[i]->b_subtree_sz;
	}
}

void dfs_hld(node* n, node* parent, node* headnode){
	n->headnode = headnode;
	headnode->nodesonpath.push_back(n);
	for(auto &n1 : n->neighbors){
		if(n1 == parent) continue;
		if(n1->b_subtree_sz > (n->b_subtree_sz - 1)/2){
			n->hldchild = n1;
			dfs_hld(n1,n,headnode);
		}
		else {
			dfs_hld(n1,n,n1);
		}
	}
	if(headnode == n){
		//if(n->id == 74787) cout << 2 << endl;
		n->start_depths.insert(n->depth);
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
		n->wsegtree.resize(2*x);
		n->bsegtree.resize(2*x);
		int i = 0;
		while(i < x){
			n->wsegtree[i] = n->bsegtree[i] = 0;
			i++;
		}
		while(i < x + n->nodesonpath.size()){
			n->bsegtree[i] = n->nodesonpath[i-x]->b_subtree_sz;
			n->wsegtree[i] = 1;
			i++;
		}
		while(i < 2*x){
			n->wsegtree[i] = n->bsegtree[i] = 0;
			i++;
		}
	}
}

void update(vector<int> &segtree, int n, int l, int r, int value){
	//l and r are 0 indexed
	//n = size of segtree/2
	r++;
	for(l += n, r += n; l < r;  l >>= 1, r >>= 1){
		//if(l&1) then l is 
		if(l & 1) segtree[l++] += value;
		if(r & 1) segtree[--r] += value;
	}
}

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
}

vector<node*> nodes;

void addEdge(int a, int b){
	nodes[a]->neighbors.push_back(nodes[b]);
	nodes[b]->neighbors.push_back(nodes[a]);
}

int asksize(node* n){
	node* head = n->headnode;
	set<int>::iterator it = head->start_depths.upper_bound(n->depth);
	it--;
	int difference = *it - head->depth;
	if(!difference && head->parent){
		if(head->color == head->parent->color)
			return asksize(head->parent);
	} 
	if(n->color) return query(head->wsegtree, head->halfsize, difference);
	else return query(head->bsegtree, head->halfsize, difference);
}

void changeblack(node* n, int value){
	//asssumed that n is black, have to relax later
	//updates all black contigious ancestors of n with bvalue += value
	if(!n->parent) return; //if n is root then no update needed
	node* head = n->headnode;
	set<int>::iterator it = head->start_depths.upper_bound(n->depth);
	it--;
	int difference = *it - head->depth; //index of first node on our black path in nodesonpath
	if(difference){
		//not all nodes on n-head path are black
		//update upto last+1-th node
		update(head->bsegtree,head->halfsize, difference-1,n->depth - head->depth - 1,value);
	}
	else{
		//update all nodes on path
		update(head->bsegtree,head->halfsize,0,n->depth - head->depth - 1, value);
		node* p = head->parent;
		if(p){
			//since changeblack(p) won't modify p, change value of p explicitly in the segtree
			p->headnode->bsegtree[p->headnode->halfsize + p->depth - p->headnode->depth] += value;
			if(head->color == p->color)
				changeblack(p,value);
			//propagate upwards
		}
	}
	//now all upper chains are taken care of
}

void changewhite(node* n, int value){
	//updates all white contigious ancestors of n with bvalue += value
	if(!n->parent) return; //if n is root then no update needed
	node* head = n->headnode;
	set<int>::iterator it = head->start_depths.upper_bound(n->depth);
	it--;
	int difference = *it - head->depth; //index of first node on our white path in nodesonpath
	if(difference){
		//not all nodes on n-head path are white
		//update upto last+1-th node
		update(head->wsegtree,head->halfsize, difference-1,n->depth - head->depth - 1,value);
	}
	else{
		//update all nodes on path
		update(head->wsegtree,head->halfsize,0,n->depth - head->depth - 1, value);
		node* p = head->parent;
		if(p){
			//since changewhite(p) won't modify p, change value of p explicitly in the segtree
			p->headnode->wsegtree[p->headnode->halfsize + p->depth - p->headnode->depth] += value;
			if(head->color == p->color)
				changewhite(p,value);
			//propagate upwards
		}
	}
	//now all upper chains are taken care of
}

void toggle(node* n){
	int wsubval = query(n->headnode->wsegtree,n->headnode->halfsize,n->depth - n->headnode->depth);
	int bsubval = query(n->headnode->bsegtree,n->headnode->halfsize,n->depth - n->headnode->depth);
	if(n->color){
		//white
		changewhite(n,-wsubval);
		n->color = 0;
		node* head = n->headnode;
		if(n != head){
			if(n->parent->color){
				//parent is also white : new start point to be created, also n is not startpoint so dont need to delete
				head->start_depths.insert(n->depth);
			}
			else head->start_depths.erase(n->depth);
		} //if parent is black, then delete n as a startpoint
		if(n->hldchild){ // n is not last in chain
			if(n->hldchild->color){
				//child is white, so make it startpoint
				head->start_depths.insert(1+n->depth);
			}
			else head->start_depths.erase(1+n->depth); //if child is black it was a startpoint before but is no longer
		}
		//now update blackval
		changeblack(n,bsubval);
	}
	else{
		changeblack(n,-bsubval);
		n->color = 1;
		node* head = n->headnode;
		if(n != head){
			if(n->parent->color){
				head->start_depths.erase(n->depth);
			}
			else head->start_depths.insert(n->depth);
		} 
		if(n->hldchild){ // n is not last in chain
			if(n->hldchild->color){
				head->start_depths.erase(1+n->depth);
			}
			else head->start_depths.insert(1+n->depth); 
		}
		changewhite(n,wsubval);

	}
}

void solve(){
	int N;
	cin >> N;
	nodes.resize(N);
	int M;
	for(int i = 0; i < N; i++){
		nodes[i] = newNode(i);
	}
	for(int i = 0; i < N-1; i++){
		cin >> x >> y;
		addEdge(x-1,y-1);
	}
	dfs(nodes[0], NULL);
	dfs_hld(nodes[0], NULL, nodes[0]);
	cin >> M;
	while(M--){
		cin >> x >> y;
		if(x == 0){
			cout << asksize(nodes[y-1]) << endl;
		}
		else if(x == 1) toggle(nodes[y-1]);
	}
}

int main(){
	ios_base::sync_with_stdio(false);
	cin.tie(NULL);
	solve();
	return 0;
}