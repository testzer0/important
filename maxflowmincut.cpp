#include <bits/stdc++.h>
using namespace std;

// Edmonds Karp : O(VE^2)

const int INF = 1000000000;

int n;
vector<vector<int>> capacity;
vector<vector<int>> adj;

int bfs(int s, int t, vector<int>& parent) {
	fill(parent.begin(), parent.end(), -1);
	parent[s] = -2;
	queue<pair<int,int>> q;
	q.push({s,INF});

	while (!q.empty()) {
		int cur = q.front().first;
		int flow = q.front().second;
		q.pop();

		for (int next : adj[cur]) {
			if (parent[next] == -1 && capacity[cur][next]) {
				parent[next] = cur;
				int new_flow = min(flow, capacity[cur][next]);
				if (next == t)
					return new_flow;
				q.push({next, new_flow});
			}
		}
	}
	return 0;
}

int maxflow(int s, int t) {
	int flow = 0;
	vector<int> parent(n);
	int new_flow;

	while (new_flow = bfs(s,t,parent)) {
		flow += new_flow;
		int cur = t;
		while (cur != s) {
			int prev = parent[cur];
			capacity[prev][cur] -= new_flow;
			capacity[cur][prev] += new_flow;
			cur = prev;
		}
	}

	return flow;
}

// improved push relabel O(VE + V^2 sqrt(E))
// usual push relabel is O (V^2 E)

vector<vector<int>> flow; // also capacity as above; and INF
vector<int> height, excess;

void push(int u, int v) {
	int d = min(excess[u], capacity[u][v] - flow[u][v]);
	flow[u][v] += d;
	flow[v][u] -= d;
	excess[u] -= d;
	excess[v] += d;
}

void relabel(int u) {
	int d = INF;
	for (int i = 0; i <  n; i++) {
		if (capacity[u][i] - flow[u][i] > 0) {
			d = min(d, height[i]);
		}
	}
	if (d < INF)
		height[u] = d+1;
}

vector<int> find_max_height_vertices(int s, int t) {
	vector<int> max_height;
	for (int i = 0; i < n; i++) {
		if (i != s && i != t && excess[i] > 0) {
			if (!max_height.empty() && height[i] > height[max_height[0]])
				max_height.clear();
			if (max_height.empty() || height[i] == height[max_height[0]])
				max_height.push_back(i);
		}
	}
	return max_height;
}

int max_flow(int s, int t) {
	height.assign(n,0);
	height[s] = n;
	flow.assign(n, vector<int>(n,0));
	excess.assign(n,0);
	excess[s] = INF;
	for (int i = 0; i < n; i++) {
		if (i != s)
			push(s,i);
	}

	vector<int> current;
	while (!(current = find_max_height_vertices(s,t)).empty()) {
		for (int i : current) {
			bool pushed = false;
			for (int j = 0; j < n && excess[i]; j++) {
				if (capacity[i][j] - flow[i][j] > 0 && height[i] == height[j]+1) {
					push(i,j);
					pushed = true;
				}
			}
			if (!pushed) {
				relabel(i);
				break;
			}
		}
	}

	int max_flow = 0;
	for (int i = 0; i < n; i++)
		max_flow += flow[i][t];
	return max_flow;
}

// Dinic's algo is O(V^2E), see cp algo

struct FlowEdge{
	int v, u;
	long long int cap, flow = 0;
	FlowEdge(int u, int v, long long cap) : u(u), v(v), cap(cap) {}
};

struct Dinic{
	const long long int flow_inf = 1e18;
	vector<FlowEdge> edges;
	vector<vector<int>> adj;
	int n, m = 0;
	int s, t;
	vector<int> level, ptr;
	queue<int> q;

	Dinic(int n, int s, int t) : n(n), s(s), t(t) {
		adj.resize(n);
		level.resize(n);
		ptr.resize(n);
	}

	void add_edge(int v, int u, long long cap) {
		edges.emplace_back(v, u, cap);
		edges.emplace_back(u, v, 0);
		adj[v].push_back(m);
		adj[u].push_back(m+1);
		m += 2;
	}

	bool bfs() {
		while (!q.empty()) {
			int v = q.front();
			q.pop();
			for (int id : adj[v]) {
				if (edges[id].cap - edges[id].flow < 1) 
					continue;
				if (level[edges[id].u] != -1)
					continue;
				level[edges[id].u] = level[v]+1;
				q.push(edges[id].u);
			}
		}
		return level[t] != -1;
	}

	long long int dfs(int v, long long int pushed) {
		if (pushed == 0)
			return 0;
		if (v == t)
			return pushed;
		for (int& cid = ptr[v]; cid < (int)adj[v].size(); cid++) {
			int id = adj[v][cid];
			int u = edges[id].u;
			if (level[v]+1 != level[u] || edges[id].cap - edges[id].flow < 1)
				continue;
			long long int tr = dfs(u, min(pushed, edges[id].cap - edges[id].flow));
			if (tr == 0)
				continue;
			edges[id].flow += tr;
			edges[id ^ 1].flow -= tr;
			return tr;
		}
		return 0;
	}

	long long int flow() {
		long long int f = 0;
		while (1) {
			fill(level.begin(), level.end(),-1);
			level[s] = 0;
			q.push(s);
			if (!bfs())
				break;
			fill(ptr.begin(), ptr.end(), 0);
			while (long long pushed = dfs(s, flow_inf)) {
				f += pushed;
			}
		}
		return f;
	}
};

// Next: MPM algo O(V^3)

struct MPM{
	struct FlowEdge{
		int v, u;
		long long int cap, flow;
		FlowEdge() {}
		FlowEdge(int _v, int _u, long long int _cap, long long int _flow) 
			: v(_v), u(_u), cap(_cap), flow(_flow) {}
		FlowEdge(int _v, int _u, long long int _cap)
			: v(_v), u(_u), cap(_cap), flow(0ll) {}
	};
	const long long int flow_inf = 1e18;
	vector<FlowEdge> edges;
	vector<char> alive;
	vector<long long int> pin, pout;
	vector<list<int>> in, out;
	vector<vector<int>> adj;
	vector<long long int> ex;
	int n, m = 0;
	int s, t;
	vector<int> level;
	vector<int> q;
	int qh, qt;
	void resize(int _n) {
		n = _n;
		ex.resize(n);
		q.resize(n);
		pin.resize(n);
		pout.resize(n);
		adj.resize(n);
		level.resize(n);
		in.resize(n);
		out.resize(n);
	}
	MPM() {}
	MPM(int _n, int _s, int _t) {
		resize(_n);
		s = _s;
		t = _t;
	}

	void add_edge(int v, int u, long long int cap) {
		edges.push_back(FlowEdge(v, u, cap));
		edges.push_back(FlowEdge(u, v, 0));
		adj[v].push_back(m);
		adj[u].push_back(m+1);
		m += 2;
	}

	bool bfs() {
		while (qh < qt) {
			int v = q[qh++];
			for (int id : adj[v]) {
				if (edges[id].cap - edges[id].flow < 1)
					continue;
				if (level[edges[id].u] != -1)
					continue;
				level[edges[id].u] = level[v]+1;
				q[qt++] = edges[id].u;
			}
		}
		return level[t] != -1;
	}

	long long int pot(int v) {
		return min(pin[v], pout[v]);
	}

	void remove_node(int v) {
		for (int i : in[v]) {
			int u = edges[i].v;
			auto it = find(out[u].begin(), out[u].end(), i);
			out[u].erase(it);
			pout[u] -= edges[i].cap - edges[i].flow;
		}
		for (int i : out[v]) {
			int u = edges[i].u;
			auto it = find(in[u].begin(), in[u].end(), i);
			in[u].erase(it);
			pin[u] -= edges[i].cap - edges[i].flow;
		}
	}

	void push(int from, int to, long long int f, bool forw) {
		qh = qt = 0;
		ex.assign(n, 0);
		ex[from] = f;
		q[qt++] = from;
		while (qh < qt) {
			int v = q[qh++];
			if (v == to)
				break;
			long long int must = ex[v];
			auto it = forw? out[v].begin() : in[v].begin();
			while (1) {
				int u = forw? edges[*it].u : edges[*it].v;
				long long int pushed = min(must, edges[*it].cap - edges[*it].flow);
				if (!pushed)
					break;
				if (forw) {
					pout[v] -= pushed;
					pin[u] -= pushed;
				}
				else {
					pin[v] -= pushed;
					pout[u] -= pushed;
				}
				if (ex[u] == 0)
					q[qt++] = u;
				ex[u] += pushed;
				edges[*it].flow += pushed;
				edges[(*it)^1].flow -= pushed;
				must -= pushed;
				if (edges[*it].cap - edges[*it].flow == 0) {
					auto jt = it;
					++jt;
					if (forw) {
						in[u].erase(find(in[u].begin(), in[u].end(), *it));
						out[v].erase(it);
					}
					else {
						out[u].erase(find(out[u].begin(), out[u].end(), *it));
						in[v].erase(it);
					}
					it = jt;
				}
				else
					break;

				if (!must)
					break;
			}
		}
	}

	long long int flow() {
		long long int ans = 0;
		while (1) {
			pin.assign(n, 0);
			pout.assign(n, 0);
			level.assign(n, -1);
			alive.assign(n, true);
			level[s] = 0;
			qh = 0, qt = 1;
			q[0] = s;
			if (!bfs())
				break;
			for (int i = 0; i < n; i++) {
				out[i].clear();
				in[i].clear();
			}
			for (int i = 0; i < m; i++) {
				if (edges[i].cap - edges[i].flow == 0) 
					continue;
				int v = edges[i].v, u = edges[i].u;
				if (level[v] + 1 == level[u] && (level[u] < level[t] || u == t)) {
					in[u].push_back(i);
					out[v].push_back(i);
					pin[u] += edges[i].cap - edges[i].flow;
					pout[v] += edges[i].cap - edges[i].flow;
				}
			}
			pin[s] = pout[t] = flow_inf;
			while (1) {
				int v = -1;
				for (int i = 0; i < n; i++) {
					if (!alive[i])
						continue;
					if (v == -1 || pot(i) < pot(v))
						v = i;
				}
				if (v == -1)
					break;
				if (pot(v) == 0) {
					alive[v] = false;
					remove_node(v);
					continue;
				}
				long long int f = pot(v);
				ans += f;
				push(v, s, f, false);
				push(v, t, f, true);
				alive[v] = false;
				remove_node(v);
			}
		}
		return ans;
	}
};

int main() {
	return 0;
}