#include <iostream>
#include <vector>
#include <queue>

using namespace std;

void printpath(vector< pair<int, int> > P){
    cout << "path is ";
    for (int i = 0; i < P.size(); i++){
        cout << P[i].first << "/" << P[i].second << " - ";
    }
    cout << endl;
}

void printE(vector< vector< pair<int, int> > > E){
    for (int i = 0; i < E.size(); i++){
        cout << i << ":" << endl;
        for (int j = 0; j < E[i].size(); j++){
            cout << "    " << j << ": " << E[i][j].first << "/" << E[i][j].second << endl;
        }
    }
}

vector< pair<int, int> > makepath(vector< pair <int,int> > &visited, int t){
    int prev = visited[t].first;
    if (prev == -2){
        vector< pair<int, int> > P(0);
        P.push_back(make_pair(t, -1));
        return P;
    }else{
        vector< pair<int, int> > P = makepath(visited, visited[t].first);
        P.push_back(make_pair(t, visited[t].second));
        return P;
    }
    
}

vector< pair<int, int> > BFS(vector< vector< pair<int, int> > > &E, int s, int t){
    vector< pair <int,int> > visited(E.size(),make_pair(-1,-1)); //-2 if start, -1 if not visited, otherwise node it was visited from; second is pos in E[nodeVisitedFrom] of node
    queue<int> nodes;
    vector< pair<int, int> > P(0);
    visited[s].first = -2;
    nodes.push(s);
    //printE(E); cout << "------" << endl;
    while(!nodes.empty()){
        int node = nodes.front();
        //cout << "checking node " << node << ":" << endl;
        nodes.pop();
        for(int i = 0; i < E[node].size(); i++){
            //cout << "    " << i << "-" << E[node][i].first << "-" << E[node][i].second << "-" << visited[E[node][i].first].first << endl;
            if (visited[E[node][i].first].first == -1 && E[node][i].second > 0){
                if (E[node][i].first == t){
                    visited[ E[node][i].first ].first = node;
                    visited[ E[node][i].first ].second = i;
                    P = makepath(visited, t);
                    //printpath(P);
                    return P;
                }else{
                    visited[ E[node][i].first ].first = node;
                    visited[ E[node][i].first ].second = i;
                    nodes.push(E[node][i].first);
                }
            }
        }
    }
}

void augment(vector< pair<int, int> > &P, vector< vector< int > > &f, vector< vector< pair<int, int> > > &E, vector< vector< bool > > &forw){
    int b = E[P[0].first][P[1].second].second;
    //cout << "bottleneck is : " << b << endl;
    for (int i = 2; i < P.size(); i++){
        int aux = E[P[i-1].first][P[i].second].second;
        if (aux < b){
            b = aux;
            // << "new bottleneck is : " << b << " at " << P[i-1].first << "-" << P[i].first << endl;
        }
    }
    for (int i = 1; i < P.size(); i++){
        if (forw[P[i-1].first][P[i].first]){
            f[P[i-1].first][P[i].second]+=b;
            E[P[i-1].first][P[i].second].second -= b;
            //cout << "substracting " << b << " from E " <<P[i-1].first << " Pos " << P[i].second<<endl;
            //cout << E[P[i-1].first][P[i].second].second << endl;
            
            int k = 0;
            
            while (E[P[i].first][k].first != P[i-1].first) k++;
            
            E[P[i].first][k].second+=b;
            //printE(E);
        }else{
            
            E[P[i-1].first][P[i].second].second-=b;
            //cout << "adding " << b << " from E " <<P[i-1].first << " Pos " << P[i].second<<endl;
            int k = 0;
            
            while (E[P[i].first][k].first != P[i-1].first) k++;
            f[P[i].first][k]-=b;
            E[P[i].first][k].second+=b;
        }
    }
}

vector< vector< int > > EdmondsKarp(vector< vector< pair<int, int> > > E, int s, int t){
    vector< vector< int > > f (E.size(), vector<int>(0));
    for (int i = 0; i < E.size(); i++){
        f[i].resize(E[i].size(), 0);
    }
    vector< vector< bool > > forw (E.size(), vector<bool>(E.size(),true));
    vector< pair<int, int> > P(0); //first in pair is node ID, second is position in vector
    
    for(int i = 0; i< E.size(); i++){
        for (int j = 0; j < E[i].size(); j++){
            if(forw[ i ][ E[i][j].first ] == true){
                forw[ E[i][j].first ][ i ] = false;
                E[ E[i][j].first ].push_back( make_pair(i, 0) );
            }
        }
    }
    P = BFS(E, s, t);
    while (P.size() != 0){
        augment(P, f, E, forw);
        P = BFS(E, s, t);
    }
    return f;
}

vector< vector< int > > maxflow(const vector< vector< pair<int, int> > > &E, int s, int t){
    return EdmondsKarp(E, s, t);
}
/*
int main(){
    int N,M;
    cin >> N >> M;
    vector< vector< pair<int, int> > > E (N, vector< pair<int, int> >(0)); //adjacency list
    //vector< int > V (N-1); //vertex
    //for (int i = 1; i<N; i++)
    //    V[i-1] = i;
    int a,b,c;
    for ( M; M > 0; M--){
        cin >> a >> b >> c;
        E[a].push_back(make_pair(b,c));
    }
    cout << "Executing EdmondsKarp" << endl;
    vector< vector< int > > f = maxflow(E, 0, 1);
    int maxflow = 0;
    for (int i = 0; i < f[0].size(); i++){
        maxflow += f[0][ i ];
    }
    printE(E);
    cout << maxflow << endl;
    cout << endl;
    for (int i = 0; i < E.size(); i++){
        cout << i << ":" << endl;
        for (int j = 0; j < E[i].size(); j++){
            cout << "    " << j << ": " << E[i][j].first << "/" << f[i][ j ] << endl;
        }
    }
    
}*/


