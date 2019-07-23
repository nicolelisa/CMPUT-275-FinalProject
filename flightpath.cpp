/*
Name: Nicole Hagerman
Name: Riona Wiberg

CMPUT 275, Winter 2019
Project: Flightpath
*/

#include <iostream>
#include <unordered_map>
#include <algorithm>
#include <fstream>
#include <string>
#include <vector>
#include <list>
#include <utility>
#include <stdlib.h>
#include <time.h>
#include "wdigraph.h"
#include "airport.h"
#include "nn.h"
#include "bruteforce.h"

using namespace std;

#define ll long long

// Open the csv file of airports and read into the 'airports' hash table with the
// 3-letter airport name as the key, and the airport object (declared in airport.h) as the value.
// Also, write the values to the idsToAirports hash table using the airport id as the key and 
// the 3-letter airport name as the value. 
void readAirports(unordered_map<string, airport>& airports, unordered_map<int, string>& idsToAirports, string filename) {
    ifstream file;
    string name,start, line, str_read;
    int str_start, str_end;
  
    file.open(filename);

    if (file.fail()) {
        cerr << "Error encountered. File was not read." << endl;
        exit(0);
    }

    //throws away the first line which is just labels
    getline(file, line);

    while (getline(file, line)) {
        airport currentAirport;
        // read the id of the airport
        str_start = 0;
        str_end = line.find(",", str_start+1);
        str_read = line.substr(str_start, str_end-str_start);
        int32_t id = stoi(str_read);
        currentAirport.id = id;

        // read the 3 digit name of the airport
        str_end = line.find(",", str_start+1);
        for (int i = 0; i < 4; i++) {
            str_start = line.find(",", str_start+1);
        }
        name = line.substr(str_start+1, 3);
        currentAirport.name = name;
        
        // read the lat of the airport
        for (int i = 0; i < 2; i++) {
            str_start = line.find(",", str_start+1);
        }
        str_end = line.find(",", str_start+1);
        str_read = line.substr(str_start+1, str_end-str_start-1);
        float lat = stof(str_read);
        currentAirport.lat = lat;

        // read the lon of the airport
        str_start = line.find(",", str_start+1);
        str_end = line.find(",", str_start+1);
        str_read = line.substr(str_start+1, str_end-str_start-1);
        float lon = stof(str_read);
        currentAirport.lon = lon;

        // insert airport into hash table
        airports.insert({name, currentAirport});
        idsToAirports.insert({id, name});
    }
    file.close();
}

// Open the csv file of flights and read into the 'flights' hash table with the
// starting and ending airports as a 6-letter key. Calculate the distance and store the 
// distance as the hash table value. 
void readFlights(unordered_map<string, ll>& flights, string filename, unordered_map<string, airport>& airports) {
    ifstream file;
    string path, start, end, line;
    int str_start, str_end;
  
    file.open(filename);

    if (file.fail()) {
        cerr << "Error encountered. File was not read." << endl;
        exit(0);
    }

    //throws away the first line which is just labels
    getline(file, line);

    while (getline(file, line)) {

        // gets start aiport
        str_start = 0;
        for (int i = 0; i < 2; i++) {
            str_start = line.find(",", str_start+1);
        }
        str_end = line.find(",", str_start+1);
        start = line.substr(str_start+1, str_end-str_start-1);

        // gets ending airport
        str_start = line.find(",", str_end+1);
        str_end = line.find(",", str_start+1);
        end = line.substr(str_start+1, str_end-str_start-1);
                
        path = start + end;

        // Makes sure that starting and ending airports exist
        auto search_start = airports.find(start);
        auto search_end = airports.find(end);
        if (search_start != airports.end() && search_end != airports.end()) {
            auto search_flight = flights.find(path);                 
            if (search_flight == flights.end()) {
                Coordinate c1(search_start->second.lat, search_start->second.lon);
                Coordinate c2(search_end->second.lat, search_end->second.lon);
                ll distance = HaversineDistance(c1, c2);
                flights.insert({path, distance});
            }
        }
    }
    file.close();
}

// Build a directed graph using the flights as the edges and the airports as the verticies.
WDigraph buildGraph(unordered_map<string, long long>& flights, unordered_map<string, airport>& airportInfo) {
    WDigraph airportGraph;
    string path, start_name, end_name;
    // Iterate over the flights and add to the the hash table
    auto i = flights.begin();
    while(i != flights.end()) {
        int start_id = -1;
        int end_id = -1;
        long long dist = i->second;
        start_name = i->first.substr(0,3);
        end_name = i->first.substr(3,3);
        auto search_start = airportInfo.find(start_name);
        if (search_start != airportInfo.end()) {
            start_id = search_start->second.id;
        }
        auto search_end = airportInfo.find(end_name);
        if (search_end != airportInfo.end()) {
            end_id = search_end->second.id;
        }
        if (start_id >= 0 && end_id >= 0) {
            airportGraph.addEdge(start_id, end_id, dist);
        }
        ++i;
    }
    return airportGraph;
}

int main() {
    clock_t startTime;
    float timeTaken;

    /* Read in Airports to hash table from CSV file */
    string airport_data = "data/airportData.csv";
    unordered_map<string, airport> airports;
    unordered_map<int, string> idsToAirports;
    readAirports(airports, idsToAirports, airport_data);

    /* Read in Flights to hash table from CSV file */
    string flight_data = "data/flightData.csv";
    unordered_map<string,ll> flights;
    readFlights(flights, flight_data, airports);

    /* Create weighted graph of all flight paths */
    WDigraph pathGraph = buildGraph(flights, airports);

    while (true) {
        system("clear");
        cout << "============= WELCOME TO FLIGHTPATH ============= " << endl << endl;
        
        /* Read in user list of airports and create a vector from the airports */
        vector<string> destinations;
        int n;
        string a;
        cout << "Enter your starting airport: ";
        cin >> a;
        destinations.push_back(a);

        cout << "How many destinations would you like to visit? ";
        cin >> n;
        
        cout << "Enter a list of space-seperated airports to visit: ";
        for (int i = 0; i<n; ++i) {
            cin >> a;
            destinations.push_back(a);
        }
        cout << endl;

        /* Check whether airports exist */
        cout << "Checking airport validity..." << endl;
        auto it = destinations.begin();
        while (it != destinations.end()){
            auto exists = airports.find(*it);
            if (exists == airports.end()) {
                cout << *it << ": Airport not found and was removed from list" << endl;
                it = destinations.erase(it);
            } else {
                ++it;
            }
        }
        cout << "There are " << destinations.size() << " airports remaining: ";
        for (auto i = destinations.begin(); i != destinations.end(); ++i) {
            cout << *i << " ";
        }
        cout << endl << endl;
        cin.clear();
        char c;
        while (true) {     
            cout << "Select an Option: " << endl;
            cout << "  (1) Find Path Using Brute Force (Perfect Accuracy, Low Efficiency)" << endl;
            cout << "  (2) Find Path Using Nearest Neighbour (Moderate Accuracy, High Efficiency)" << endl;
            cout << "  (3) Enter New Destinations" << endl;
            cout << "  (4) Exit" << endl << endl;
            cin >> c;
            if (c == '1') {
                startTime = clock();
                bruteforce(destinations, airports, flights, pathGraph, idsToAirports);
                timeTaken = (clock() - startTime)/CLOCKS_PER_SEC;
                cout << "Run time was: " << (float)(clock() - startTime)/CLOCKS_PER_SEC << "s" << endl << endl;
            } else if (c == '2') {
                startTime = clock();
                modifiedNearestNeighbour(pathGraph, destinations, flights, idsToAirports, airports);
                timeTaken = (clock() - startTime)/CLOCKS_PER_SEC;
                cout << "Run time was: " << (float)(clock() - startTime)/CLOCKS_PER_SEC << "s" << endl << endl;
            } else if (c == '3') {
                break;
            } else if (c == '4') {
                cout << "Exiting Program" << endl;
                return 0;
            }  else {
                cout << "Invalid input" << endl;
                cin.clear();
            }
            cout << endl;
        }  
    }
    return 0;
}

