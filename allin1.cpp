#include <bits/stdc++.h>
using namespace std;

/*--------------------------------------------
Splay Tree implementation by zer0_h6cks
References:
1) https://codeforces.com/blog/entry/18462
2) https://www.geeksforgeeks.org/splay-tree-set-3-delete/
3) https://www.cs.cornell.edu/courses/cs3110/2009fa/Recitations/rec-splay.html
Supports:
1) Insert in O(log N) amortized
2) Delete in O(log N) amortized
3) Recent nodes are fast-access
---------------------------------------------*/


struct node{
	int value;
	int subtree_sz;
	node* parent;
	node* left;
	node* right;
};

node* root = NULL;

node* newNode(int value){
	node* n = new node;
	n->value = value;
	n->parent = NULL;
	n->left = NULL;
	n->right = NULL;
	n->subtree_sz = 1;
	return n;
}

#define rank(n) (n->left) ? 1+n->left->subtree_sz : 1

/*
	Rotates: visualization:
	   |  								|
	   X     (leftRotate X)             Y
	  / \     =>=>=>=>=>=>			   / \
	 A   Y    <=<=<=<=<=<=            X   C
	 	/ \   (rightRotate Y)        / \
	   B   C                        A   B
*/



//before doing splays, code rotations
void rightRotate(node* &n){
	if(!n) return;
	node* l = n->left; //assumed non NULL
	node* lr = l->right;
	node* p = n->parent;

	//change l<->parent stuff
	l->parent = p;
	if(p){
		if(p->right == n){
			p->right = l;
		}
		else p->left = l;
	}
	//n<->parent stuff
	l->right = n;
	n->parent = l;

	//new child stuff
	n->left  = lr;
	if(lr) lr->parent = n;

	//update sizes : A,B,C will remain same
	l->subtree_sz = n->subtree_sz; //for l

	n->subtree_sz--; //X is no longer child of Y
	if(l->left) n->subtree_sz -= l->left->subtree_sz; //A is no longer descendant of Y
}

void leftRotate(node* &n){
	if(!n) return;
	node* r = n->right; //assumed non NULL
	node* rl = r->left;
	node* p = n->parent;

	//change r<->parent stuff
	r->parent = p;
	if(p){
		if(p->right == n){
			p->right = r;
		}
		else p->left = r;
	}
	//n<->parent stuff
	r->left = n;
	n->parent = r;

	//new child stuff
	n->right  = rl;
	if(rl) rl->parent = n;

	//update sizes 
	r->subtree_sz = n->subtree_sz;

	n->subtree_sz--;
	if(r->right) n->subtree_sz -= r->right->subtree_sz;
}

void splay(node* &n){
	//splay n to root
	node *p,*pp;
	if(!n) return;
	while(n->parent){
		p = n->parent;
		if(!p->parent){
			if(p->left == n) rightRotate(p);
			else leftRotate(p);
		}
		else{
			pp = p->parent;
			if(pp->left == p){
				if(p->left ==  n){
					//ZigZig case
					rightRotate(pp);
					rightRotate(p);
				}
				else{
					//ZigZag case
					leftRotate(p);
					rightRotate(pp);
				}
			}
			else{
				if(p->left == n){
					rightRotate(p);
					leftRotate(pp);
				}
				else{
					leftRotate(pp);
					leftRotate(p);
				}
			}
		}
	}
	root = n;
}

void insert(int value){
	node* n = newNode(value);
	//inserts a node and performs rebalance in O(log n) time 
	if(!root){
		root = n;
		return;
	}
	node* current = root;
	node* l,r;
	while(true){
		if(current->value == n->value) break; //cannot have duplicates
		//use iterative instead of recursive for faster exec 
		else if(n->value < current->value){
			if(current->left) current = current->left;
			else{
				current->left = n;
				n->parent = current;
				break;
			} 
		}
		else{
			if(current->right) current = current->right;
			else{
				current->right = n;
				n->parent = current;
				break;
			}
		}
	}
	//update subtree sizes; n has size set to 1 already
	current = n->parent;
	while(current){
		current->subtree_sz++;
		current = current->parent;
	}
	//node inserted now splay it to root 
	splay(n);
}

node* find(int value){
	if(!root) return NULL;
	node* current = root;
	while(true){
		if(current->value == value) break;
		else if(value < current->value){
			if(current->left) current = current->left;
			else{
				current = NULL;
				break;
			}
		}
		else{
			if(current->right) current = current->right;
			else{
				current = NULL;
				break;
			}
		}
	}	
	if(current) splay(current);
	return current;
}

bool erase(int value){
	node* n = find(value);
	if(!n) return false;
	//in find n has been splayed to root
	//for new root find node in left subtree of n with maximum value
	//No need to perform size update since root doesnt contribute to any other subtree_sz's
	node* l = n->left;
	node* r = n->right;
	if(!l){
		if(!r){
			root = NULL;
			delete n;
			return true;
		}
		root = r;
		root->parent = NULL;
		delete n;
		return true;
	}
	else if(!r){
		root = l;
		root->parent = NULL;
		delete n;
		return true;
	}
	else{
		l->parent = r->parent = NULL;
		delete n; //split splay tree into two
		while(l->right) l = l->right; //max node in left subtree
		//now splay it to root in left subtree;
		splay(l);
		//note that l->right must be NULL as we did only leftRotates for the splay, and l->right was 
		//initially NULL as well. So make r as right child of l
		l->right = r;
		l->subtree_sz += r->subtree_sz;
		r->parent = l;
		root = l;
		return true;
	}
}

int findindex(int value){
	node* n = find(value);
	//now n has been splayed to root: ans is 1+leftsubtreesz
	if(!n){
		return -1;
	}
	else{
		cout << 1 << endl;
		if(n->left) return 1+n->left->subtree_sz;
		else return 1;
	}
}

int fndv(node* curr, int ind){
	int r = rank(curr);
	if(ind == r) return curr->value;
	else if(ind < r) return fndv(curr->left,ind);
	else return fndv(curr->right, ind - r);
}

int findvalue(int ind){
	if(!root) return -1;
	if(ind > root->subtree_sz) return -1;
	return fndv(root, ind);
}

int main(){
	ios_base::sync_with_stdio(false);
	cin.tie(NULL);
	int cmd;
	int x;
	while(true){
		cin >> cmd;
		if(cmd == -1) break;
		cin >> x;
		if(cmd == 1){
			insert(x);
		}
		else if(cmd == 2)
			erase(x);
		else if(cmd == 3)
			cout << findindex(x) << "\n";
		else
			cout << findvalue(x) << "\n";
	}
	//main does nothing, here as a filler
	return 0;
}