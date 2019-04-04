/*
	ADD FILE HEADER
*/

#ifndef _BRUTEFORCE_H_
#define _BRUTEFORCE_H_

#include <string>
#include <vector>
#include "dijkstra.h"

using namespace std;
typedef pair<int, long long> PIL;

// adapted from https://www.geeksforgeeks.org/write-a-c-program-to-print-all-permutations-of-a-given-string/
void findPermutations(vector<string>& dest, vector<vector<string>>& perms, int l, int r, string home) {
    if (l == r) {
    	vector<string> current = dest;
        current.push_back(home);
        current.insert(current.begin(), home);
        perms.push_back(current);
        // int j = 0;
        // for (auto i = current.begin(); i != current.end(); ++i) {
        //     // cout << j << ":" << *i << " ";
        //     ++j;
        // }
        // cout << endl;
    }
    else {
        for (int i=l; i<=r; ++i) {
        iter_swap(dest.begin()+l-1, dest.begin()+i-1);
        findPermutations(dest, perms, l+1, r, home);
        iter_swap(dest.begin()+l-1, dest.begin()+i-1); // backtrack
        } 
    }
}

void findPath(vector<string>& currentPerm, int& position, ll& currentDist, int start, int end, const WDigraph& pathGraph, const unordered_map<int, string>& idsToAirports) {
	unordered_map<int, PIL> tree;
	dijkstra(pathGraph, start, tree);
	  if (tree.find(end) == tree.end()) {
	      cout << "No path found" << endl;
	      currentDist = -1;
		}  else {
			currentDist += tree.find(end)->second.second;
			// cout << "DISTANCE IS: " << tree.find(end)->second.second << endl;
	    // read off the path by stepping back through the search tree
	    list<int> path;
	    while (end != start) {
	    	path.push_front(end);
			// auto search = idsToAirports.find(end);
			// if (search != idsToAirports.end()) {
			// 	currentPerm.insert(currentPerm.begin()+position, search->second);
			// }
	    	// cout << end << " " << search->second << endl;
	    	end = tree[end].first;
	    }
	    path.pop_back();
	    
	    // position = 1;
	    // cout << " LIST SIZE IS: " << path.size() << endl;
	    // path.push_front(start);
	    string st, end;
	    auto search = idsToAirports.find(start);
	    st = search->second;
	    for (auto it : path) {
	    	auto search = idsToAirports.find(it);
	    	string destName =  "*" + (search->second);
	    	currentPerm.insert(currentPerm.begin()+position+1, destName);
        	// cout << "  Layover added at: " << destName << endl;
      	}
      	position += path.size();
	  }

}

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

    // cout << "Permutations: " << perm_count << endl;
    for (int i=0; i<perm_count; ++i) {
    	// cout << "====== Starting new iteration =====" << endl;
    	ll dist = 0;

		// if path exists, get distance
		for (int j=0; j<perms[i].size()-1; ++j) {
			auto search = flights.find(perms[i][j] + perms[i][j+1]);
			if (search != flights.end()) {
				// cout << "Distance between " << perms[i][j] << " and " << perms[i][j+1] << " is " << search->second << endl;
				dist += search->second;
			} else {
				// cout << perms[i][j] << " to " << perms[i][j+1] << " is not found" << endl;
				// get ids of values
				int start_id, end_id;
				auto search_start = airports.find(perms[i][j]);
        		if (search_start != airports.end()) {
            		start_id = search_start->second.id;
        		} else {
        			// cout << perms[i][j] << " is not found" << endl;
        			start_id = -1;
        		}
		        auto search_end = airports.find(perms[i][j+1]);
		        if (search_end != airports.end()) {
		            end_id = search_end->second.id;
		        } else {
        			// cout << perms[i][j+1] << " is not found" << endl;
        			end_id = -1;
        		}
				findPath(perms[i], j, dist, start_id, end_id, pathGraph, idsToAirports);
				if (dist == -1) {
					// cout << "No path found" << endl;
					break;
				}
			}
		}
    	// cout << "Distance of iteration is: " << dist << endl << endl;
    	if (min_distance <= 0 || dist < min_distance) {
    		min_distance = dist;
    		min_path = perms[i];
    	}
    }
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
