#include <iostream>
#include <vector>
#define VERSION 1
#define S 0
#define T 1
#define sp 2
#define tp 3
//version is changed in compilation for version 1 ~ 2 of the problem
// the rest are mere aliases to make code more readable
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
    cout << "Input read with " << n << " cities and " << m << " flights" << endl;
    vector <vector<int> > flightArrivals(n);
    vector <vector<int> > flightDepartures(n);
    for (int i = 0; i<m; i++) {
        flightDepartures[flights[i].origin].push_back(i);
        flightArrivals[flights[i].destiny].push_back(i);
    }
    //grafbuilding
    vector<vector < pair <int,int> > > FluxGraph (4 + 2*m);
    FluxGraph[S].push_back(make_pair(sp, m));
    FluxGraph[tp].push_back(make_pair(T, m));
    FluxGraph[sp].push_back(make_pair(tp, m)); //basic graph with k = m
    for(int i = 0; i<m; i++) {
        FluxGraph[sp].push_back(make_pair(i*2+4,m)); //sp to departure
        FluxGraph[i*2+5].push_back(make_pair(tp,m)); //arrival to tp
        if(VERSION == 2) FluxGraph[i*2+4].push_back(make_pair(i*2+5,m)); // departure to arrival

        // link previous flights to flight i
        for(int j = 0; j < flightArrivals[flights[i].origin].size(); j++) {
            if(flights[i].tdeparture - flights[flightArrivals[flights[i].origin][j]].tarrival >= timeMargin)
                FluxGraph[flightArrivals[flights[i].origin][j] * 2 + 5].push_back(make_pair(i*2+4, m));
        }
        // link flight i to future flights
        for(int j = 0; j < flightDepartures[flights[i].destiny].size(); j++) {
            if(flights[flightDepartures[flights[i].destiny][j]].tdeparture - flights[i].tarrival >= timeMargin)
                FluxGraph[i*2+5].push_back(make_pair(flightDepartures[flights[i].destiny][j] * 2 + 4, m));
        }
    }
    printOutcome(FluxGraph);
    int nextK = m;
    bool possible = true;
    while (possible) {

    }
}
