/*
Name: Nicole Hagerman
Name: Riona Wiberg

CMPUT 275, Winter 2019
Project: Flightpath
*/


#ifndef _DIJKSTRA_H_
#define _DIJKSTRA_H_

#include <utility>
#include <unordered_map>
#include "wdigraph.h"

using namespace std;

typedef pair<int, long long> PIL;

// run dijkstra's algorithm on the given graph from the given start vertex
// record the shortest path tree of this search in "tree"
void dijkstra(const WDigraph& graph, int startVertex, unordered_map<int, PIL>& tree);

#endif
