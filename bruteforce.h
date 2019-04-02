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

vector<string> bruteforce(vector<string> dest, const unordered_map<string, airport>& airports) {
	vector<string> test;
	string home = dest.front();
	dest.erase(dest.begin())
	// dest.begin());
	test.push_back(home);
	return test;
}

// void callthis() {
// 	cout << "worked";
// }

#endif
