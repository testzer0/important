#include <bits/stdc++.h>
using namespace std;

/*
Edmonds-Karp for the max flow problem,
solves in O(VE^2)
Edmonds Karp is just a version of Ford-Fulkerson.
May be used for min-cut too.
*/

int N;
vector<vector<int>> capacity;
vector<vector<int>> adj;

int bfs(int s, int t, vector<int> &parent){
	fill(parent.begin(), parent.end(), -1);
	parent[s] = -2;
	queue<pair<int,int>> qu;
	qu.push({s,std::numeric_limits<int>::infinity()});

	while(!qu.empty()){
		int cur = qu.front().first;
		int flow = qu.front().second;
		qu.pop();

		for(int next : adj[cur]){
			if(parent[next] == -1 && capacity[cur][next]){
				parent[next] = cur;
				int new_flow = min(flow, capacity[cur][next]);
				if(next == t)
					return new_flow;
				qu.push({next,new_flow});
			}
		}
	}
	return 0;
}

int maxflow(int s, int t){
	int flow = 0;
	vector<int> parent(N);
	int new_flow;

	while(new_flow = bfs(s,t,parent)){
		//while there is augmenting path, push
		flow += new_flow;
		int cur = t;
		while(cur != s){
			int prev = parent[cur];
			capacity[prev][cur] -= new_flow;
			capacity[cur][prev] += new_flow;
			cur = prev;
		}
	}

	return flow;
}
int main(){
	ios_base::sync_with_stdio(false);
	cin.tie(NULL);
	return 0;
}