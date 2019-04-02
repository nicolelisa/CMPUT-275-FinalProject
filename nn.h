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
#include "wdigraph.h"
#include "airport.h"

#define ll long long

using namespace std;

void readInData(WDigraph& flights, string filename);

WDigraph createGraph(vector<string> requestedAirports, unordered_map<string, 
                     long long>& distances, unordered_map<string, airport>& airportInfo);


//pair<long long, vector<string>> nearestNeighbour(WDigraph& airportGraph, int StartAirportID);


#endif