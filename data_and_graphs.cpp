#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <string>
#include <list>
#include "wdigraph.h"
#include "digraph.h"

using namespace std;

#define ll long long

void readInData(WDigraph& airports, string filename) {
    //string file = "CanadaAirportFlights.csv";
    ifstream file;
    string start, end, line;
    int found1, found2, iStart, iEnd;
  
    file.open(filename);

    if (file.fail()) {
        cerr << "Error encountered. File was not read." << endl;
        exit(0);
    }

    //throws away the first line which is just labels
    getline(file, start);

    int counter = 1;

    while (getline(file, line)) {
        found1 = 0;
        for (int i = 0; i < 3; i++) {
            found1 = line.find(",", found1+1);
        }
       
        found2 = line.find(",", found1+1);
        start = line.substr(found1+1, found2-found1-1);
      
        found1 = line.find(",", found2+1);
        found2 = line.find(",", found1+1);
        end = line.substr(found1+1, found2-found1-1);

        cout << counter << ": ";
        cout << "Airport 1: " << start << " ";
        cout << "Airport 2: " << end << endl;

        if (start.substr(1,1) != "N" && end.substr(1,1) != "N") {
            iStart = stoi(start);
            // airports.addVertex(iStart);
        }
        
        if (end.substr(1,1) != "N") {
            iEnd = stoi(end);
            // airports.addVertex(iEnd);
        }

        // Still need to get their long and lat then add the weighted edge to the graphs!!!!

        counter++;
    }
    file.close();
}

ll findDistance(ll lon1, ll lat1, ll lon2, ll lat2) {
    ll distance = sqrt((lon1-lon2)*(lon1-lon2) + (lat1-lat2)*(lat1-lat2));
    return distance;
}


int main() {
    WDigraph airports;
    readInData(airports, "flightData.csv");
}

