#ifndef _NN_H_
#define _NN_H_

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
#include <map>
#include "wdigraph.h"
#include "digraph.h"
#include "airport.h"
#include "dijkstra.h"

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


vector<pair<string, int>> nameToNumber(vector<string> requestedAirports, unordered_map<string, airport>& airportInfo) {
    vector<pair<string, int>> bothIDs;
    for (int i = 0; i < (int)requestedAirports.size(); i++) {
        int numericalID = airportInfo.find(requestedAirports.at(i))->second.id;
        bothIDs.push_back(make_pair(requestedAirports.at(i), numericalID));
    }
    return bothIDs;
}

unordered_map<int, string> numberToName(vector<string> requestedAirports, unordered_map<string, airport>& airportInfo) {
    unordered_map<int, string> bothIDs;
    for (int i = 0; i < (int)requestedAirports.size(); i++) {
        int numericalID = airportInfo.find(requestedAirports.at(i))->second.id;
        //bothIDs.push_back(make_pair(requestedAirports.at(i), numericalID));
        bothIDs[numericalID] = requestedAirports.at(i);
    }
    return bothIDs;
}


// nameOfAirports = destination on main file
// distances = flights
WDigraph createGraph(vector<string> requestedAirports, unordered_map<string, long long>& distances, 
                      unordered_map<string, airport>& airportInfo) {
    WDigraph airportGraph;
    string jointNames;
    long long dist;

    // vector with pairs of the airport names and ids
    vector<pair<string, int>> bothIDs = nameToNumber(requestedAirports, airportInfo);

    for (int i = 0; i < (int)bothIDs.size(); i++) {
        for (int j = 0; j < (int)bothIDs.size(); j++) {
            if (i !=j) {
                jointNames = bothIDs.at(i).first + bothIDs.at(j).first;
                //cout << jointNames << endl;
                if (distances.find(jointNames) != distances.end()) {    
                    dist = distances.find(jointNames)->second;
                } else {
                    dist = -1;
                }

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
//lso needs the numerical ID of the airport that we are starting at.
void nearestNeighbour(WDigraph& airportGraph, vector<string> requestedAirports, 
                                                 unordered_map<string, airport>& airportInfo) {
    ll distanceTravelled = 0;
    ll shortestPathSoFar = -1;
    int stopNumber = 0;
    long long cost = -1;
    int nearestAirport;
    bool hasPath = true;

    unordered_map<int, string> numsToNames = numberToName(requestedAirports, airportInfo);
    int airportStart = airportInfo.find(requestedAirports.at(0))->second.id;

    //unordered_set<int>::const_iterator Digraph::neighbours(int v)
    //int neighbours;
    int curNode = airportStart;

    // key is airportID and value is the number of edges leading to it yet.
    map<int, int> airportsTravelled;
    // Set beginning node to exist in the map but not have any edges to it yet.
    airportsTravelled[airportStart] = 0;
    stopNumber++;
    while ((int)airportsTravelled.size() < airportGraph.size()) {
        for (auto iter = airportGraph.neighbours(curNode); iter != airportGraph.endIterator(curNode); iter++) {
            if (airportGraph.isEdge(curNode, *iter) && (airportsTravelled.find(*iter) == airportsTravelled.end())) {
                cost = airportGraph.getCost(curNode, *iter);
                if (cost != -1) {
                    if (shortestPathSoFar == -1) {
                        shortestPathSoFar = cost;
                        nearestAirport = *iter;
                    } else if (shortestPathSoFar > cost) {
                        shortestPathSoFar = cost;  
                        nearestAirport = *iter;
                    }
                }
            }
        }
        if (shortestPathSoFar != -1) {
            distanceTravelled += shortestPathSoFar;
            shortestPathSoFar = -1;
            curNode = nearestAirport;
            airportsTravelled[nearestAirport] = stopNumber;
            stopNumber++;
        } else {
            hasPath = false;
            break;            
        }
    }

    string lastAirport;
    string nextAirport;
    if (airportGraph.isEdge(curNode, airportStart) && airportGraph.getCost(curNode, airportStart) != -1 && hasPath) {
        for (map<int,int>::iterator it = airportsTravelled.begin(); it != airportsTravelled.end(); ++it) {
            //cout << it->first << " " << it->second << endl;
            cout << numsToNames[it->first] << " ";
        }
        distanceTravelled += airportGraph.getCost(curNode, airportStart);

        lastAirport = numsToNames[airportStart];
        cout << lastAirport << endl;

        cout << "Distance: " << distanceTravelled << endl;

    } else {
        cout << "Path: ";
        for (map<int,int>::iterator it = airportsTravelled.begin(); it != airportsTravelled.end(); ++it) {
            //cout << it->first << " " << it->second << endl;
            nextAirport = numsToNames[it->first];
            cout << nextAirport << " ";
        }
        cout << endl << "No path to complete rest of trip" << endl;

    }     
}


// Modified nn 
// calculate distances between airports
// find the shortest distrance between cur Airport and possible next airports.
// if there is not a direct flight to the next airport, use dijkstra to find the shortest path
// to that airport

pair <ll 

#endif