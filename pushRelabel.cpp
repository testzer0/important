#include <bits/stdc++.h>
using namespace std;

/*
An improved push relabel algorithm.
Time complexity O(VE + V^2 sqrt(E))
*/

const int INF = 1000000000;

int N;
vector<vector<int>> capacity, flow;
vector<int> height, excess;

void push(int u, int v){
    int d = min(excess[u], capacity[u][v]-flow[u][v]);
    flow[u][v] += d;
    flow[v][u] -= d;
    excess[u] -= d;
    excess[v] += d;
}

void relabel(int u){
    int d = INF;
    for(int i = 0; i < N; i++){
        if(capacity[u][i]-flow[u][i] > 0)
            d = min(d, height[i]);
    }
    if(d < INF)
        height[u] = d+1;
}

vector<int> find_max_height_vertices(int s, int t){
    vector<int> max_height;
    for(int i = 0; i < N; i++){
        if(i != s && i != t && excess[i] > 0){
            if(!max_height.empty() && height[i] > height[max_height[0]])
                max_height.clear();
            if(max_height.empty() || height[i] == height[max_height[0]])
                max_height.push_back(i);
        }
    }
    return max_height;
}

int max_flow(int s, int t){
    height.assign(N,0);
    height[s] = N;
    flow.assign(N,vector<int>(N,0));
    excess.assign(N,0);
    excess[s] = INF;
    for(int i = 0; i < N; i++){
        if(i != s)
            push(s,i);
    }
    vector<int> current;
    while(!(current = find_max_height_vertices(s,t)).empty()){
        for(int i : current){
            bool pushed = false;
            for(int j = 0; j < N && excess[i]; j++){
                if(capacity[i][j] - flow[i][j] > 0 && height[i] == height[j]+1){
                    push(i,j);
                    pushed = true;
                }
            }
            if(!pushed){
                relabel(i);
                break;
            }
        }
    }
    int max_flow = 0;
    for(int i = 0; i < N; i++)
        max_flow += flow[0][i];
    return max_flow;
}

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    return 0;
}