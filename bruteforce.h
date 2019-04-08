/*
Name: Nicole Hagerman
Student ID: 1200596

Name: Riona Wiberg
Student ID: 1532156

CMPUT 275, Winter 2019
Project: Flightpath
*/

#ifndef _BRUTEFORCE_H_
#define _BRUTEFORCE_H_

#include <string>
#include <vector>
#include "dijkstra.h"

using namespace std;
typedef pair<int, long long> PIL;

// Takes a vector of strings (dest) and adds all possible permutations to the perms vector. 
// adapted from https://www.geeksforgeeks.org/write-a-c-program-to-print-all-permutations-of-a-given-string/
void findPermutations(vector<string>& dest, vector<vector<string>>& perms, int l, int r, string home) {
    if (l == r) {
    	vector<string> current = dest;
        current.push_back(home);
        current.insert(current.begin(), home);
        perms.push_back(current);
    }
    else {
        for (int i=l; i<=r; ++i) {
        iter_swap(dest.begin()+l-1, dest.begin()+i-1);
        findPermutations(dest, perms, l+1, r, home);
        iter_swap(dest.begin()+l-1, dest.begin()+i-1); // backtrack
        } 
    }
}

// Uses dijkstra alogrithm to find the shortest path between two destinations that are not directly 
// connected. Adds the path to the current permutation (currentPerm) and adds the distance of the path
// to the total distance.
void findPath(vector<string>& currentPerm, int& position, ll& currentDist, int start, int end, const WDigraph& pathGraph, const unordered_map<int, string>& idsToAirports) {
	unordered_map<int, PIL> tree;
	dijkstra(pathGraph, start, tree);
	if (tree.find(end) == tree.end()) {
		// if there is no path that exists, set the distance of the permutation to 0
		currentDist = -1;
	} else {
		currentDist += tree.find(end)->second.second;
	    // read off the path by stepping back through the search tree
	    list<int> path;
	    while (end != start) {
	    	path.push_front(end);
	    	end = tree[end].first;
	    }
	    path.pop_back();

	    // search for airport name and add to the current permutation
	    string st, end;
	    auto search = idsToAirports.find(start);
	    st = search->second;
	    for (auto it : path) {
	    	auto search = idsToAirports.find(it);
	    	string destName =  "*" + (search->second);
	    	currentPerm.insert(currentPerm.begin()+position+1, destName);
	  	}
	  	position += path.size();
	}
}

// Run the brute force path search using the airports, idsToAirports and flights hash tables. Pass the pathGraph
// parameter in case there is no path between two destinations and a layover is required.
void bruteforce(vector<string> dest, const unordered_map<string, airport>& airports, const unordered_map<string, ll>& flights, 
				const WDigraph pathGraph, const unordered_map<int, string>& idsToAirports) {
    vector<string> min_path = dest;
    ll min_distance = 0;
	string home = dest.front();
	dest.erase(dest.begin());
	int count = dest.size();
	vector<vector<string>> perms;
    findPermutations(dest, perms, 1, count, home);
    int perm_count = perms.size();

    for (int i=0; i<perm_count; ++i) {
    	ll dist = 0;
    	// if path exists, get distance and add to the distance total
		for (int j=0; j<perms[i].size()-1; ++j) {
			auto search = flights.find(perms[i][j] + perms[i][j+1]);
			if (search != flights.end()) {
				dist += search->second;
			} else {
				// get ids of start and end airport
				int start_id, end_id;
				auto search_start = airports.find(perms[i][j]);
        		if (search_start != airports.end()) {
            		start_id = search_start->second.id;
        		} else {
        			dist = -1;
        		}
		        auto search_end = airports.find(perms[i][j+1]);
		        if (search_end != airports.end()) {
		            end_id = search_end->second.id;
		        } else {
        			dist = -1;
        		}
        		if (dist == -1) {
					break;
				}
				findPath(perms[i], j, dist, start_id, end_id, pathGraph, idsToAirports);
			}
		}

    	// check if this is the shortest distance so far and if it is, set as min path
    	if (min_distance <= 0 || dist < min_distance) {
    		min_distance = dist;
    		min_path = perms[i];
    	}
    }

    // print the shortest path to the console
    cout << endl << endl << "=========== BRUTE FORCE FLIGHTPATH COMPLETE ===========" << endl;
	if (min_distance == -1) {
		cout << "No path was found" << endl;
	} else {
	    for (auto i = min_path.begin(); i != min_path.end(); ++i) {
	    	cout << *i << " ";
		}
		cout << endl;
		cout << "Minimum distance is " << min_distance << "km" << endl;
	}
}

#endif
