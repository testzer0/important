#include <bits/stdc++.h>
using namespace std;


/*-------------------
Implementation of treap by zer0_h6cks, referring to CP-algorithms.
Available on github.
Treap: Tree + heap;
A binary search tree on key value;
A max-heap on priority.
Supported operations:
1) Split(T,X) : Splits the tree into two subtrees; one has all 
nodes with key < X, and the other with all > X 
2) Merge(T1, T2) : Merges two given treaps
3) Insert(X,Y) : Add new node
4) Search(X) : searches and returns node with value X 
5) Erase(X) : erases a node with value X 
6) Build : builds treap in O(N) time. [OFFLINE ONLY]
7) Union : Finds the union of two treaps
----------------------*/

struct node{
    int key;
    int priority;
    node* left;
    node* right;
};

node* newNode(int key, int priority){
    node* n = new node;
    n->left = n->right = NULL;
    n->key = key;
    n->priority = priority;
    return n;
}


//Splits treap into < X and > X treaps in O(log n) time
void split(node* n, int key, node* &l, node* &r){
    //Splits the treap at node n, and fills l and r with 
    //resultant treaps
    if(!n) l = r = NULL; //No treap 
    else if(key < n->key){
        //traverse left 
        split(n->left,key,l,n->left); //Split and rejoin unused part of left with >X key
        r = n;
    }
    else{
        split(n->right,key,n->right,r);
        l = n;
    }
}

//Inserts a node into treap in O(log n) time
void insert(node* &n, node* in){
    if(!n) n = in;
    else if(in->priority > n->priority){
        split(n,in->key,in->left,in->right);
        n = in;
    }
    else{
        if(in->key < n->key)
            insert(n->left,in);
        else
            insert(n->right,in);
    }
}

void merge(node* &result, node* &l, node* &r){
    //merges l and r and fills into result
    if(!l || !r){
        if(l) result = l;
        else result = r;
    }
    else if(l->priority > r->priority){
        merge(l->right,l->right,r);
        result = l;
    }
    else{
        merge(r->left,r->left,l);
        result = r;
    }
}

void erase(node* n, int key){
    //searches for node with key = key, and deletes it 
    if(n->key == key)
        merge(n,n->left,n->right);
    else{
        if(key < n->key)
            erase(n->left,key);
        else
            erase(n->right,key);
    }
}

node* unify(node* l, node* r){
    //merges treaps with roots n1, n2. If sizes are N,M 
    //then takes time N log(M/N) but fast in practice
    if(!l || !r){
        if(l) return l;
        else return r;
    }
    if(l->priority < r->priority){
        //swap
        node* temp = r;
        r = l;
        l = temp;
    }
    node *n1,*n2;
    split(r,l->key,n1,n2);
    l->left = unify(l->left,n1);
    l->right = unify(l->right,n2);
    return l;
}

/*
* Helper functions:
* heapify : fixes the priorties at a node and its children to preserve heap orderness
* build : Build tree in offline mode in O(N) time. This randomly initializes priorties
* and 'fixes' them in linear time.
*/

void heapify(node* &n){
    if(!n) return; //nothing to do here
    node* nn = n; //placeholder for node with max priority
    if(n->left && (n->left->priority > nn->priority))
        nn = n;
    if(n->right && (n->right->priority > nn->priority))
        nn = n;
    if(nn != n){
        //swap
        int pr = n->priority;
        n->priority = nn->priority;
        nn->priority = pr;
        heapify(nn);
    }
}

node* build(int *keys, int n){
    if(!n) return NULL;
    int mid = n/2;
    node* n1 = newNode(keys[mid], rand());
    n1->left = build(keys,mid);
    n1->right = build(keys+mid+1,n-mid-1);
    heapify(n1);
    return n1;
}

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    return 0;
}