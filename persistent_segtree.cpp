#include <bits/stdc++.h>
using namespace std;

#define M (L+R)/2
#define SIZE 100 //change as per wish
//In general size ~ 8NlogN is good

/*
struct node{
	int L;
	int R;
	int value;
	int lazy;
	node* lef;
	node* righ;
};*/

//persistent segment tree for RMQ

int lef[SIZE], righ[SIZE], value[SIZE];
bool hasflag[SIZE] = {false};
int flag[SIZE];
int numnodes = 0;

int newlazykid(int node, int delta, int L, int R){
	int pos = ++numnodes;
	lef[pos] = lef[node];
	righ[pos] = righ[node];
	flag[pos] = flag[node];
	hasflag[pos] = true;

	//range increase
	flag[pos] += delta;
	value[pos] += (R-L+1)*delta; //for range sum, typically can be changed
	//value[pos] = min(value[pos], delta);
	//above for range min

	return pos;
}

int newLeaf(int val){
	int pos = ++numnodes;
	lef[pos] = righ[pos] = 0;
	value[pos] = val;
	return pos;
}

int newParent(int leff, int rig){
	int pos = ++numnodes;
	lef[pos] = leff;
	righ[pos] = rig;
	value[pos] = value[leff] + value[rig];
	return pos;
}

void propagate(int pos, int L, int R){
	if(hasflag){
		if(L != R){
			lef[pos] = newlazykid(lef[pos],flag[pos],L,M);
			righ[pos] = newlazykid(righ[pos],flag[pos],M+1,R);
		}
		hasflag[pos] = false;
	}
}

int build(int arr[], int L, int R){
	if(L == R) return newLeaf(arr[L]);
	else return newParent(build(arr,L,M),build(arr,M+1,R));
}

int update(int where, int addwhat, int pos, int L, int R){
	//returns pos of new root
	//use like:
	//int new_root = update(...);
	//or store in version array/vector
	if(L == R) return newLeaf(value[pos] + addwhat);
	if(where <= M)
		return newParent(update(where,addwhat,lef[pos],L,M),righ[pos]);
	else
		return newParent(lef[pos],update(where,addwhat,righ[pos],M+1,R));
}

//reverts range [l:r] to old version
//returns reverted root
int rangeRevert(int l, int r, int pos, int revert, int L, int R){
	if(r < L || R < l) return pos;//keep version
	if(l <= L && R <= r) return revert;//reverted version
	return newParent(rangeRevert(l,r,lef[pos],lef[revert],L,M),
		rangeRevert(l,r,righ[pos],righ[revert],M+1,R));
}

int range_update_lazy(int l, int r, int x, int pos, int L, int R){
	if(r < L || R < l) return pos; //do nothing
	if(l <= L && R <= r) return newlazykid(pos, x, L, R);
	propagate(pos,L,R); //propagate before continuing
	return newParent(range_update_lazy(l,r,x,lef[pos],L,M),
		range_update_lazy(l,r,x,righ[pos],M+1,R));
}

int range_query(int l, int r, int pos, int L, int R){
	if(r < L || R < l) return 0;
	if(l <= L && R <= r) return value[pos];
	propagate(pos,L,R);
	return range_query(l,r,lef[pos],L,M) + range_query(l,r,righ[pos],M+1,R);
}

int main(){
	ios_base::sync_with_stdio(false);
	cin.tie(NULL);

	return 0;
}