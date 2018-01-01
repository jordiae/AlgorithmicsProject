#include <iostream>
#include <vector>


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

vector< pair<int, int> > DFS(vector< vector< pair<int, int> > > &E, vector<bool> &visited, int s, int t, vector< pair<int, int> > P , int prev){
    //printpath(P);
    visited[s] = true;
    P.push_back(make_pair(s, prev));
    if (s == t)
        return P;
    for (int i = 0; i < E[s].size(); i++){
        int next = E[s][i].first;
        if (visited[next] == false &&  E[s][i].second > 0){
            vector< pair<int, int> > aux = DFS(E,visited, next, t, P, i);
            if (aux.size()!=0)
                return aux;
        }
    }
    return vector< pair<int, int> >(0);
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
            f[P[i-1].first][P[i].first]+=b;
            E[P[i-1].first][P[i].second].second -= b;
            //cout << "substracting " << b << " from E " <<P[i-1].first << " Pos " << P[i].second<<endl;
            //cout << E[P[i-1].first][P[i].second].second << endl;
            
            int k = 0;
            
            while (E[P[i].first][k].first != P[i-1].first) k++;
            
            E[P[i].first][k].second+=b;
            //printE(E);
        }else{
            f[P[i].first][P[i-1].first]-=b;
            E[P[i-1].first][P[i].second].second+=b;
            //cout << "adding " << b << " from E " <<P[i-1].first << " Pos " << P[i].second<<endl;
            int k = 0;
            
            while (E[P[i].first][k].first != P[i-1].first) k++;
            
            E[P[i].first][k].second-=b;
        }
    }
}

vector< vector< int > > FordFulkerson(vector< vector< pair<int, int> > > E, int s, int t){
    vector< vector< int > > f (E.size(), vector<int>(E.size(),0));
    vector<bool> visited(E.size(),false);
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
    P = DFS(E, visited, s, t, P, -1);
    while (P.size() != 0){
        augment(P, f, E, forw);
        for (int i = 0; i<visited.size(); i++){
            visited[i] = false;
        }
        vector< pair<int, int> > aux(0);
        P = DFS(E, visited, s, t, aux, -1);
    }
    return f;
}

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
    cout << "Executing FordFulkerson" << endl;
    vector< vector< int > > f = FordFulkerson(E, 0, N-1);
    int maxflow = 0;
    for (int i = 0; i < E[0].size(); i++){
        maxflow += f[0][ E[0][i].first ];
    }
    
    cout << maxflow << endl;
    cout << endl;
    for (int i = 0; i < E.size(); i++){
        cout << i << ":" << endl;
        for (int j = 0; j < E[i].size(); j++){
            cout << "    " << j << ": " << E[i][j].first << "/" << f[i][ E[i][j].first ] << endl;
        }
    }
    
}


