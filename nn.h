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

void modifiedNearestNeighbour(WDigraph& fullGraph, vector<string> destinations, unordered_map<string, ll>& distances,
                              unordered_map<int, string> IDsToName, unordered_map<string, airport> airportInfo) {
    ll distanceTravelled = 0, lowestDistance = -1, curDistance;
    int closestCity, curCity;
    string curCityName, nextCityName;
    unordered_map<int, pair<int, ll>> searchTree;
    set<int> citiesSoFar, citiesReached;
    int startVertex = airportInfo[destinations.at(0)].id;
    citiesReached.insert(startVertex);
    citiesSoFar.insert(startVertex);
    list<int> path, tempPath;
    path.push_front(startVertex);

    curCityName = destinations.at(0);
    curCity = airportInfo[curCityName].id;
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
            //distanceTravelled += searchTree.find(closestCity)->second.second;
            citiesSoFar.insert(closestCity);
            while (closestCity != curCity) {
                //distanceTravelled += searchTree.find(closestCity)->second.second;
                tempPath.push_front(closestCity);
                closestCity = searchTree[closestCity].first;
            }
            if (path.size() < 2) {
                for (list<int>::iterator it = tempPath.begin(); it != tempPath.end(); ++it) {
                    path.push_front(*it);
                }
            } else {
                for (list<int>::reverse_iterator it = tempPath.rbegin(); it != tempPath.rend(); ++it) {
                    path.push_front(*it);
                }
            }
            tempPath.clear();
            searchTree.clear();
            curCityName = IDsToName[path.back()];
            curCity = path.back();
        }
    }
    // calculate round trip
    //curCity = path.front();
    // dijkstra(fullGraph, path.front(), searchTree);
    // if (searchTree.find(startVertex) == searchTree.end()) {
    //     distanceTravelled = -1;
    // } else {
    //     // goes through the path to the desired airport, adds the distance
    //     // of each flight to the distanceTravelled
    //     while (startVertex != path.front()) {
    //         distanceTravelled += searchTree.find(startVertex)->second.second;
    //         citiesSoFar.insert(startVertex);
    //         tempPath.push_front(startVertex);
    //         startVertex = searchTree[startVertex].first;
    //     }
    //     for (list<int>::iterator it = tempPath.begin(); it != tempPath.end(); ++it) {
    //         path.push_front(*it);
    //     }
    //     tempPath.clear();
    // }

    //distanceTravelled += searchTree.find(closestCity)->second.second;
    
    // auto search_start = IDsToName.find(path.front());
    // string start_name = search_start->second;        
    // auto search_end = IDsToName.find(path.back());
    // string end_name = search_end->second;
    // string path_name = start_name + end_name;
    // auto search_path = distances.find(path_name);
    // ll return_distance = search_path->second;
    // distanceTravelled += return_distance;
    // searchTree.clear();




    // cout << "SEARCH TREE DIST: " << searchTree.find(startVertex)->second.second << endl;
    // cout << "CLOSEST CITY: " << closestCity << endl;
    // cout << "CITY: " << curCity << endl;
    //cout << "Current: " << path.back() << " " << "To: " << startVertex << endl;
    
    cout << "=========== NEAREST NEIGHBOUR FLIGHTPATH COMPLETE ===========" << endl;

    // curDistance = -1;
    // lowestDistance = -1;

    // if (searchTree.find(startVertex) == searchTree.end()) {
    //     cout << "No Path Found" << endl;
    //     cout << "Distance: -1" << endl;
    // } else {
    //     while (startVertex != curCity) {
    //         citiesSoFar.insert(curCity);
    //         path.push_front(curCity);
    //         curCity = searchTree[curCity].first;
    //     }
    // }

    // Print the output to the terminal
    // Case if there is not a route to reach all desired destinations
    if (distanceTravelled == -1) {
        cout << "No path found" << endl;
    } else {
    path.push_front(startVertex);

    // Case if there is a path to all desired airports, prints an "*" before the airport
    // ID if there is a layover at another airport as the shortest route.
    for (list<int>::reverse_iterator rit = path.rbegin(); rit != path.rend(); ++rit) {
        curCityName = IDsToName[*rit];
        if (find(destinations.begin(), destinations.end(), IDsToName[*rit]) != destinations.end()) {
            cout << curCityName << " ";
        } else {
            cout << "*" << curCityName << " ";
        }
    }
    distanceTravelled = calculateNetDist(path, distances, IDsToName);

    cout << endl << "Distance: " << distanceTravelled << "km" << endl;
} 
}


#endif