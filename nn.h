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
#include <list>
#include <set>
#include "wdigraph.h"
#include "digraph.h"
#include "airport.h"
#include "dijkstra.h"

#define ll long long

using namespace std;

// Finds distances from the coordinares (radial distances)
ll findDistance(string city1, string city2, unordered_map<string, airport> airportInfo) {
    double startLon = airportInfo.find(city1)->second.lon;
    double startLat = airportInfo.find(city1)->second.lat;
    double endLon = airportInfo.find(city2)->second.lon;
    double endLat = airportInfo.find(city2)->second.lat;
    Coordinate startCoord(startLat, startLon);
    Coordinate endCoord(endLat, endLon);
    ll curDistance = HaversineDistance(startCoord, endCoord);
    return curDistance;
}

void modifiedNearestNeighbour(WDigraph& fullGraph, vector<string> destinations, unordered_map<string, ll>& distances,
                              unordered_map<int, string> IDsToName, unordered_map<string, airport> airportInfo) {
    ll distanceTravelled = 0;
    ll lowestDistance = -1;
    int closestCity, curCity;
    string curCityName, nextCityName;
    ll curDistance;
    ll highestDist = 0;
    unordered_map<int, pair<int, ll>> searchTree;
    set<int> citiesSoFar;
    int startVertex = airportInfo[destinations.at(0)].id;



    set<int> citiesReached;
    citiesReached.insert(startVertex);

    list<int> path;
    path.push_front(startVertex);

    curCityName = destinations.at(0);
    curCity = airportInfo[curCityName].id;
    for (int i = 0; i < (int)destinations.size(); i++) {
        //find the shortest distance from curNode to the next closest location
        if (i > 0) { 
            curCityName = IDsToName[closestCity];
            curCity = closestCity;
        }

        for (int j = 0; j < (int)destinations.size(); j++) {
            if (i != j) {
                // find the distance of the closest city
                nextCityName = destinations.at(j);
                curDistance = findDistance(curCityName, nextCityName, airportInfo);

                // Check if the lowest distance is actually lower.
                if (lowestDistance != -1 && lowestDistance >= curDistance && curDistance != 0 && 
                    citiesReached.count(airportInfo.find(nextCityName)->second.id) == 0) {
                    lowestDistance = curDistance;
                    closestCity = airportInfo[nextCityName].id;

                // If there is no lowest entry is add current distance as the lowest
                } else if (lowestDistance == -1 && curDistance != 0 && 
                           citiesReached.count(airportInfo.find(nextCityName)->second.id) == 0) {
                    lowestDistance = curDistance;
                    curCityName = destinations.at(j);
                    closestCity = airportInfo[nextCityName].id;
                } 
            }
        }
        
        // Insert the city into the set of ones you've travelled to
        citiesReached.insert(closestCity);
        dijkstra(fullGraph, curCity, searchTree);
        distanceTravelled += searchTree.find(closestCity)->second.second;
        if (highestDist < searchTree.find(closestCity)->second.second){
            highestDist = searchTree.find(closestCity)->second.second;
        }

        //cout << searchTree.find(closestCity)->second.second << endl;
        
        curDistance = -1;
        lowestDistance = -1;
        // Go through Dijkstra path and create a list of nodes that need to be travelled to
        if (searchTree.find(closestCity) == searchTree.end()) {
            cout << "No Path Found" << endl;
            cout << "Distance: -1" << endl;
            break;
        } else {
            while (closestCity != curCity) {
                citiesSoFar.insert(closestCity);
                path.push_front(closestCity);
                closestCity = searchTree[closestCity].first;
            }
        }
        searchTree.clear();
    }
    
    // --------------------------------------------------------------------
    // THIS IS DIJKSTRA DOESN'T ADD ANYTHING!
    // For some reason the curCity is always the same as the start city!
    // It is never goign to be a round trip. I couldn't find a way for it to work!
    //
    dijkstra(fullGraph, curCity, searchTree);
    distanceTravelled += searchTree.find(startVertex)->second.second;
    distanceTravelled += 1.5* highestDist;

    cout << searchTree.find(startVertex)->second.second << endl;

    //cout << "Current: " << path.back() << " " << "To: " << startVertex << endl;
    
    cout << endl << endl << "=========== NEAREST NEIGHBOUR FLIGHTPATH COMPLETE ===========" << endl;

    curDistance = -1;
    lowestDistance = -1;

    if (searchTree.find(startVertex) == searchTree.end()) {
        cout << "No Path Found" << endl;
        cout << "Distance: -1" << endl;
    } else {
        while (startVertex != curCity) {
            citiesSoFar.insert(curCity);
            path.push_front(curCity);
            curCity = searchTree[curCity].first;
        }
    }
    path.push_front(startVertex);

    for (list<int>::reverse_iterator rit = path.rbegin(); rit != path.rend(); ++rit) {
        curCityName = IDsToName[*rit];
        if (find(destinations.begin(), destinations.end(), IDsToName[*rit]) != destinations.end()) {
            cout << curCityName << " ";
        } else {
            cout << "*" << curCityName << " ";
        }
    }

    cout << endl << "Distance: " << distanceTravelled << "km" << endl;
} 


#endif