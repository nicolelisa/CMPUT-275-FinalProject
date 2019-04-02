#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <utility>
#include <string>
#include <iostream>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <string>
#include "wdigraph.h"
#include "airport.h"

#define ll long long

using namespace std;

void readInData(WDigraph& flights, string filename) {
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
            // flights.addVertex(iStart);
        }
        
        if (end.substr(1,1) != "N") {
            iEnd = stoi(end);
            // flights.addVertex(iEnd);
        }

        // Still need to get their long and lat then add the weighted edge to the graphs!!!!

        counter++;
    }
    file.close();
}


// nameOfAirports = destination on main file
// distances = flights
WDigraph createGraph(vector<string> requestedAirports, unordered_map<string, long long>& distances, 
                      unordered_map<string, airport>& airportInfo) {
    WDigraph airportGraph;
    int numericalID;
    string jointNames;
    long long dist;

    // vector with pairs of the airport names and ids
    vector<pair<string, int>> bothIDs;
    for (int i = 0; i < requestedAirports.size(); i++) {
        numericalID = airportInfo.find(requestedAirports.at(i))->second.id;
        bothIDs.push_back(make_pair(requestedAirports.at(i), numericalID));

    }

    for (int i = 0; i < bothIDs.size(); i++) {
        for (int j = 0; j < bothIDs.size(); j++) {
            if (i !=j) {
                jointNames = bothIDs.at(i).first + bothIDs.at(j).first;
                dist = distances.find(jointNames)->second;
                //dist = 
                // convert i and j back to strings concatenate them back together
                // long long dist = look up distance in unordered map via concatenated string (hashtable!)
                // if the string does not exist within the unordered map make the distance NULL
                airportGraph.addEdge(bothIDs.at(i).second, bothIDs.at(j).second, dist);
            }
        }
    }

    return airportGraph;

}


// Needs a weighted graph of the airports of interest
// also needs the numerical ID of the airport that we are starting at.
// pair<long long, vector<string>> nearestNeighbour(WDigraph& airportGraph, int StartAirportID) {
//     long long distanceTravelled = 0;
//     //unordered_set<int>::const_iterator Digraph::neighbours(int v)
//     int neighbours;
//     int curNode = StartAirportID;

//     // key is airportID and value is the number of edges leading to it yet.
//     map<int, int> airportsTravelled;
//     // Set beginning node to exist in the map but not have any edges to it yet.
//     airportsTravelled[curNode] = 0;
//     while (airportsTravelled.size() != airportGraph.size()) {
//         if (airportsTravelled.count(curNode) == 0) {
//             //Case if airport does not exist in the map yet
//         } 


//     }

// }