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
// Functions used are in airport.h
// Code from: https://rosettacode.org/wiki/Haversine_formula
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


// calculates the total distance travelled in all the flights.
// Parameters: listOfStops - list of all of the airports that are travelled to
//             distances - contains flight distances uses names as keys
//             IDsToName - contains the conversion between numberical IDs and the airport names with IDs as keys
ll calculateNetDist(list<int> listOfStops, unordered_map<string,ll> distances, unordered_map<int, string> IDsToName) {
    ll distanceTravelled = 0;
    string lastCityName = IDsToName[listOfStops.back()];
    listOfStops.pop_back();
    for (list<int>::reverse_iterator rit = listOfStops.rbegin(); rit != listOfStops.rend(); ++rit) {
        string curCityName = IDsToName[*rit];
        distanceTravelled += distances[lastCityName+curCityName];
        lastCityName = curCityName;

    }
    return distanceTravelled;
}


// prints the output for the function
// Parameters: distanceTravelled - tracks whether a path was found or not
//             startVertex - the ID of the airport that you started at (used to make it a round trip)
//             path - list of airports visited in order
//             destinations - vector containing airports requested by the user
//             IDsToName - contains the conversion between numberical IDs and the airport names uses IDs as keys
//             distances - contains flight distances with names as keys
void printOutput(ll distanceTravelled, int startVertex, list<int> path, vector<string> destinations,
                 unordered_map<int, string> IDsToName, unordered_map<string, ll> distances) {
    cout << "=========== NEAREST NEIGHBOUR FLIGHTPATH COMPLETE ===========" << endl;

    // Print the output to the terminal
    // Case if there is not a route to reach all desired destinations
    if (distanceTravelled == -1) {
        cout << "No path was found" << endl;
    } else {
    path.push_front(startVertex);
    set<string> stops;

    // Case if there is a path to all desired airports, prints an "*" before the airport
    // ID if there is a layover at another airport as the shortest route.
    for (list<int>::reverse_iterator rit = path.rbegin(); rit != path.rend(); ++rit) {
        string curCityName = IDsToName[*rit];
        if (find(destinations.begin(), destinations.end(), IDsToName[*rit]) != destinations.end()
            && stops.find(curCityName) == stops.end()) {
            cout << curCityName << " ";
            stops.insert(curCityName);
        } else {
            if (*rit == path.back()) {
                cout << curCityName << " ";
            } else {
                cout << "*" << curCityName << " ";
            }
        }
    }
    distanceTravelled = calculateNetDist(path, distances, IDsToName);
    cout << endl << "Distance: " << distanceTravelled << "km" << endl; 
    }
}


// Function based off of nearest neighbour approach to the travelling salesman problem
// Had to be adapted to work for an asymettrical graph (not all airports have flights to a location and back)/
// Calculates the closest airport if there was a direct flight, if there was not a direct flight to
// that airport, uses dijkstra's to find the path that is shortest using layovers.

// Parameters: fullGraph - graph of all flights in the data base, where the edges' weights are the
//                         distances between airports.
//             destinations - vector of the user's requested locations
//             distances - contains flight distances with names as keys
//             IDsToName - contains conversion between numberical IDs and the airport names with IDs as keys
//             airportInfo - contains airport struct (contains ID, longitude, latitude and name) uses name as keys.
void modifiedNearestNeighbour(WDigraph& fullGraph, vector<string> destinations, unordered_map<string, ll>& distances,
                              unordered_map<int, string> IDsToName, unordered_map<string, airport> airportInfo) {
    // Initialize values that will be used later in the function.
    ll distanceTravelled = 0, lowestDistance = -1, curDistance;
    int closestCity, curCity, startVertex = airportInfo[destinations.at(0)].id;
    string curCityName, nextCityName;
    unordered_map<int, pair<int, ll>> searchTree;
    set<int> citiesSoFar, citiesReached;
    list<int> path, tempPath;

    // Include the airport that you start at in all the lists
    citiesReached.insert(startVertex);
    citiesSoFar.insert(startVertex);
    path.push_front(startVertex);

    curCityName = destinations.at(0);
    curCity = airportInfo[curCityName].id;

    // Main loop, iterates through the vector of requested stops. Keeps track of which
    // locations have already been stopped at, does not stop there again. 
    for (int i = 0; i < (int)destinations.size(); i++) {
        //find the shortest distance from curNode to the next closest location
        for (int j = 0; j < (int)destinations.size(); j++) {
            if (i != j) {
                // find the distance of the closest city
                nextCityName = destinations.at(j);
                curDistance = findDistance(curCityName, nextCityName, airportInfo);

                // Check if the lowest distance is actually lower.
                if (lowestDistance >= curDistance && curDistance != 0 && 
                    citiesReached.count(airportInfo.find(nextCityName)->second.id) == 0) {
                    lowestDistance = curDistance;
                    closestCity = airportInfo[nextCityName].id;

                // If there is no lowest entry is add current distance as the lowest
                } else if (lowestDistance == -1 && curDistance != 0 && 
                           citiesReached.count(airportInfo.find(nextCityName)->second.id) == 0) {
                    lowestDistance = curDistance;
                    closestCity = airportInfo[nextCityName].id;
                } 
            }
        }
        
        // Insert the city into the set of ones you've travelled to
        citiesReached.insert(closestCity);
        dijkstra(fullGraph, curCity, searchTree);

        curDistance = -1;
        lowestDistance = -1;
        // Go through Dijkstra path and create a list of nodes that need to be travelled to
        // If the city cannot be reached set the distance to -1 and exit the loop.
        if (searchTree.find(closestCity) == searchTree.end()) {
            distanceTravelled = -1;
            break;
        } else {
            // goes through the path to the desired airport, adds the distance
            // of each flight to the distanceTravelled
            citiesSoFar.insert(closestCity);
            while (closestCity != curCity) {
                tempPath.push_front(closestCity);
                closestCity = searchTree[closestCity].first;
            }

            // Add the most recent flights to path (data structure that keeps track of all flights).
            if (tempPath.size() <= 2) {
                for (list<int>::iterator it = tempPath.begin(); it != tempPath.end(); ++it) {
                    path.push_front(*it);
                }
            } else {
                for (list<int>::reverse_iterator it = tempPath.rbegin(); it != tempPath.rend(); ++it) {
                    path.push_front(*it);
                }
            }

            // Clear existing search tree and path so there is no residual data in the next iteration.
            tempPath.clear();
            searchTree.clear();
           
            // Update the current city you're in to the one most recently added to the list.
            curCityName = IDsToName[path.front()];
            curCity = path.front();
        }
    }
    // Prints the outputs for the function             
    printOutput(distanceTravelled, startVertex, path, destinations, IDsToName, distances);
}


#endif

