#include <iostream>
#include <vector>
#include <limits>
#include <queue>


#define INFINITE std::numeric_limits<int>::max()

using namespace std;

/*void print_flows(const vector<vector<int> > &flows) {
	cout << endl;
	cout << "flows:" << endl;
	for (int i = 0; i < flows.size(); i++) {
		for (int j = 0; j < flows[i].size(); j++) {
			cout << "flows[" << i << "][" << j << "] = " << flows[i][j] << endl;
		}
	}
	cout << endl;
}*/


bool bfs(vector<vector<pair<int,int> > > &g,vector<int> &levels, int s, int t, vector<vector<int> > &flows) {
    for (int i = 0; i < g.size(); i++) {
        levels[i] = -1;
    }
	levels[s] = 0;
	queue<int> q;
	q.push(s);
	while(not q.empty()) {
		int front = q.front();
		q.pop();
		for (int i = 0; i < g[front].size(); i++) {
			pair<int,int> edge = g[front][i];
			int adj_v = edge.first;
			int adj_f = flows[front][i];
			int adj_c = edge.second;
			if (levels[adj_v] < 0 and adj_f < adj_c) {
				levels[adj_v] = levels[front]+1;
				q.push(adj_v);
			}

		}
	}
	if (levels[t] < 0)
		return false;
	else
		return true;
}

int dfs(int v, int t, int flow, vector<int>& visited, vector<int>& levels, vector<vector<pair<int,int> > > &g,vector<vector<int> > &flows,vector<vector<int> > &reverse) {
	if (v == t)
		return flow;
	while (visited[v] < g[v].size()) {


		pair<int,int> &edge = g[v][visited[v]];
		int &edge_flow = flows[v][visited[v]];

		if (levels[edge.first] == levels[v]+1 and edge_flow < edge.second) {
			int currentFlow = min(flow, edge.second-edge_flow);
			int temporalFlow = dfs(edge.first, t, currentFlow, visited, levels, g, flows, reverse);

			if (temporalFlow > 0) {
				edge_flow += temporalFlow;
				int edge_reverse = reverse[v][visited[v]];
				flows[edge.first][edge_reverse] -= temporalFlow;
				return temporalFlow;
			}
		}
		visited[v]++;
	}
	return 0;

}


int dinic(vector<vector<pair<int,int> > > &graph, int s, int t, vector<vector<int> > &reverse, vector<vector<int> > &flows) {
	if (s == t)
		return -1;
	vector<int> levels(graph.size());

	int max_flow = 0;
	while (bfs(graph,levels,s,t,flows)) { // bfs -> label graph (labels)
		int flow = -1;
		vector<int> visited (graph.size()+1,0);
		while (flow != 0) {
			flow = dfs(s,t,INFINITE,visited,levels, graph, flows,reverse); // dfs -> blocking flow
			max_flow += flow;
		}
	}
	return max_flow;

}


vector<vector<int > > maxflow(const vector<vector<pair<int,int> > > &graph, int s, int t) {

	vector<vector<pair<int,int> > > graph2(graph.size());

	vector<vector<int > > flows(graph.size());

	vector<vector<int > > flows_def(graph.size());


	vector<vector<int> > reverse(graph.size());
	for (int i = 0; i < graph.size(); i++) {
		for (int j = 0; j < graph[i].size(); j++) {
			pair<int,int> edge = graph[i][j];
			graph2[i].push_back(edge);
			graph2[edge.first].push_back(make_pair(i,0));	

			reverse[i].push_back(graph2[edge.first].size()-1);
			reverse[edge.first].push_back(graph2[i].size()-1);

			flows[i].push_back(0);
			flows_def[i].push_back(flows[i].size()-1);
			flows[edge.first].push_back(0);

		}
	}



	int max_flow = dinic(graph2,s,t,reverse,flows);


	for (int i = 0; i < flows_def.size(); i++) {
		if (i != t) {
			for (int j = 0; j < flows_def[i].size(); j++) {
				flows_def[i][j] = flows[i][flows_def[i][j]];
			}
		}

	} 

    return flows_def;

}




/*
int main() {
	int n, m;
	while (cin >> n >>m) {
		vector<vector<pair<int,int> > > g(n);
		for (int j = 0; j < m; j++) {
			int a, b, c;
			cin >> a >> b >> c;
			int i = a;
			int v = b;

			g[i].push_back(make_pair(v,c));
			
		}
		int s, t;
		s = 0;
		t = n-1;

		//t = 1;
		


		cout << "Executing Dinic" << endl;
    	vector< vector< int > > flows = maxflow(g,s,t);
    	int maxf = 0;
    	for (int i = 0; i < flows[0].size(); i++){
        	maxf += flows[0][i];
    	}
    
    	cout << maxf << endl;
    	cout << endl;
    	for (int i = 0; i < g.size(); i++){
        	cout << i << ":" << endl;
        	for (int j = 0; j < g[i].size(); j++){
            	cout << "    " << j << ": " << g[i][j].first << "/" << flows[i][j] << endl;
        	}
   		}

	}

}

*/
