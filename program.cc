#include <iostream>
#include <vector>
#define VERSION 1
#define S 0
#define T 1
#define sp 2
#define tp 3
//version is changed in compilation for version 1 ~ 2 of the problem
// the rest are mere aliases to make code more readable

#include "FordFulkerson.cpp"
//#include "EdmondsKarp.cpp"
//#include "dinic.cc"
using namespace std;

const int timeMargin = 15;

struct Flight {
    int origin; int destiny;
    int tdeparture; int tarrival;
};

int max(int a, int b, int c) {
    if(a>b) {
        if (a>c) return a;
        else return c;
    } else if (b > c) return b;
    else return c;
}
void printOutcome(const vector<vector < pair <int,int> > >& G) {
    for (int i = 0; i < G.size(); i++) {
        for(int j=0; j < G[i].size(); j++) {
            cout << i << '-' << G[i][j].second << "->" << G[i][j].first << endl;
        }
        cout << endl;
    }
}

void printPilot(const vector<vector < pair <int,int> > >& G, vector<vector <int> > & sol, int currentVertex) {
    while(currentVertex != tp) {
        int i=0; bool jump = false;
        while(i < sol[currentVertex].size() and not jump) {
            if(sol[currentVertex][i] > 0) {
                sol[currentVertex][i] --;
                if (G[currentVertex][i].first == T) {// he's the pilot
                    cout << currentVertex/2-1 << ' '; // function to recover flightnumber from vertex
                    currentVertex = currentVertex+1; //jump from current (departure) to end (arrival)
                    jump = true;
                } else {
                    currentVertex = G[currentVertex][i].first;
                    jump = true;
                    //normal graph transition
                }
            }
            i++;
        }
    }
    cout << endl;
}

int main () {
    Flight newFlight;
    vector <Flight> flights;
     //note that it stores the flights in the vector
    int n = 0; //cities
    while(cin >> newFlight.origin >> newFlight.destiny >> newFlight.tdeparture >> newFlight.tarrival){
        flights.push_back(newFlight);
        n = max(n, newFlight.origin+1, newFlight.destiny+1);
    }
    int m = flights.size();
    //should sort it to make dichosearch later on.
    //cout << "Input read with " << n << " cities and " << m << " flights" << endl;
    vector <vector<int> > flightArrivals(n);
    for (int i = 0; i<m; i++) {
        flightArrivals[flights[i].destiny].push_back(i);
    }
    //grafbuilding
    vector<vector < pair <int,int> > > FluxGraph (4 + 2*m);
    FluxGraph[S].push_back(make_pair(sp, m));
    FluxGraph[tp].push_back(make_pair(T, m));
    FluxGraph[sp].push_back(make_pair(tp, m)); //basic graph with k = m
    for(int i = 0; i<m; i++) {
        FluxGraph[sp].push_back(make_pair(i*2+4, 2*m)); //sp to departure
        FluxGraph[i*2+5].push_back(make_pair(tp, 2*m)); //arrival to tp
        FluxGraph[i*2+4].push_back(make_pair(T,1));
        FluxGraph[S].push_back(make_pair(i*2+5,1));
        if(VERSION == 2) FluxGraph[i*2+4].push_back(make_pair(i*2+5,m)); // departure to arrival

        // link previous flights to flight i
        for(int j = 0; j < flightArrivals[flights[i].origin].size(); j++) {
            if(flights[i].tdeparture - flights[flightArrivals[flights[i].origin][j]].tarrival >= timeMargin)
                FluxGraph[flightArrivals[flights[i].origin][j] * 2 + 5].push_back(make_pair(i*2+4, m));
        }
    }
    //printOutcome(FluxGraph);
    int nextK = m;
    bool possible = true;
    vector<vector<int> > sol;
    while (possible) {
        //cout << "Trying" << endl;
        vector<vector<int> > newSol = maxflow(FluxGraph, S, T);
        for(int i=0; i<newSol[S].size(); i++) {
            if(newSol[S][i] != FluxGraph[S][i].second) possible = false;
            //cout << newSol[S][i] << ' ';
        }
        //cout << endl;
        if(possible) {
                //cout << "Could with " << nextK << ", unused " << newSol[sp][0] << " so updating to ";
                sol = newSol;
                nextK = nextK-newSol[sp][0]-1; // lower bound to last sols - unused pilots -1
                //cout << nextK << endl;
                FluxGraph[S][0].second = nextK;
                FluxGraph[tp][0].second = nextK;
                //printOutcome(FluxGraph);
        }
    }
    //cout << "ACTUAL SOLUTION COUT STARTS HERE----------" << endl;
    cout << sol[S][0]-sol[sp][0] << endl;
    sol[sp][0]=0; // eliminate the 'useless' flux
    for(int i=0; i<sol[sp].size(); i++) {
        if(sol[sp][i] != 0) {
            sol[sp][i] -= 1;
            //printPilot (FluxGraph, sol, FluxGraph[sp][i].first);
            i--;
        }
    }
}
