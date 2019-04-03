/*
  Contains the restaurant struct declaration, functions to read restaurants
  from the SD card, and functions to sort a restaurant around a given
  latitude/longitude
*/

#ifndef _BRUTEFORCE_H_
#define _BRUTEFORCE_H_

#include <string>
#include <vector>



using namespace std;

// ll getDistance(vector<string>& dest, string home) {
// 	string start = home;
// 	string end = dest.front();
// 	string path = start + end;
//     auto got = flights.find(path);
//     if (got == flights.end())
//       cout << "Not Found";
//     else
//       cout << "Distance between desintations " << got->first << " is: " << got->second;
//     cout << endl;	
// }

// adapted from https://www.geeksforgeeks.org/write-a-c-program-to-print-all-permutations-of-a-given-string/
void findPermutations(vector<string>& dest, vector<vector<string>>& perms, int l, int r) {
    if (l == r) {
        // cout << "Permutation found: ";
        // cout << home << " ";
        // int j = 0;
        // for (auto i = dest.begin(); i != dest.end(); ++i) {
        //     cout << j << ":" << *i << " ";
        //     ++j;
        // }
        // cout << endl;
        perms.push_back(dest);
        // cout << home << endl;
    }
    else {
        for (int i=l; i<=r; ++i) {
        iter_swap(dest.begin()+l-1, dest.begin()+i-1);
        findPermutations(dest, perms, l+1, r);
        iter_swap(dest.begin()+l-1, dest.begin()+i-1); // backtrack
        } 
    }
}

void bruteforce(vector<string>& dest, const unordered_map<string, airport>& airports, const unordered_map<string, ll>& flights) {

    vector<string> min_path = dest;
    ll min_distance = 0;

	string home = dest.front();
	dest.erase(dest.begin());
	// test.push_back(home);
	int count = dest.size();
	vector<vector<string>> perms;
    findPermutations(dest, perms, 1, count);
    int perm_count = perms.size();

    cout << "Permutations: " << perm_count << endl;
    for (int i=0; i<perm_count; ++i) {
    	cout << "====== Starting new iteration =====" << endl;
    	
    	ll dist = 0;
    	
    	// get distance of home path
    	auto search = flights.find(home + perms[i][0]);
    	if (search != flights.end()) {
    		cout << "Distance between " << home << " and " << perms[i][0] << " is " << search->second << endl;
    		dist += search->second;

    		// if home path exists, continue to calculate most paths
    		for (int j=0; j<count-1; ++j) {
    			auto search = flights.find(perms[i][j] + perms[i][j+1]);
    			if (search != flights.end()) {
    				cout << "Distance between " << perms[i][j] << " and " << perms[i][j+1] << " is " << search->second << endl;
    				dist += search->second;
    			} else {
    				cout << perms[i][j] + perms[i][j+1] << " is not found" << endl;
    				dist = -1;
    				break;
    			}
    		}

    		// get home path 
    		if (dist > 0) {
	    		auto search = flights.find(perms[i][count-1] + home);
	    		if (search != flights.end()) {
	    				cout << "Distance between " << perms[i][count-1] << " and " << home << " is " << search->second << endl;
	    				dist += search->second;
				} else {
					cout << perms[i][count-1] + home << " is not found" << endl;
					dist = -1;
				}
			}

    	} else {
    		cout << home + perms[i][0] << " is not found" << endl;
    		dist = -1;
    	}
    	cout << "Distance of iteration is: " << dist << endl << endl;
    	if (min_distance <= 0 || dist < min_distance) {
    		min_distance = dist;
    		min_path = perms[i];
    	}

    }
    cout << endl << endl << "======== PROGRAM COMPLETE ========" << endl;
	cout << "Minimum distance is: " << min_distance << endl;
}

#endif
