#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include <fstream>
#include <sstream>
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

// void readData(unordered_map<int, vector<float>>& runStats, string filename) {
//     ifstream file;
//     string path, line, str_read;
//     int str_start, str_end;
//     vector<float> stats;
  
//     file.open(filename);

//     if (file.fail()) {
//         cerr << "Error encountered. File was not read." << endl;
//         exit(0);
//     }
//     //throws away the first line which is just labels
//     getline(file, line);

//     while (getline(file, line)) {
//         // read the id of the airport
//         str_start = 0;
//         str_end = line.find(",", str_start+1);
//         str_read = line.substr(str_start, str_end-str_start);
//         int cities = stoi(str_read);
//         cout << cities << endl;

//         // getting bf runs
//         str_start = str_end;
//         str_end = line.find(",", str_start+1);
//         str_read = line.substr(str_start, str_end-str_start);
//         float bf_runs = stof(str_read);
//         cout << bf_runs << endl;
//         stats.push_back(bf_runs);
                
//         // // getting bf time
//         // str_start = line.find(",", str_end+1);
//         // str_end = line.find(",", str_start+1);
//         // str_read = line.substr(str_start, str_end-str_start);
//         // float bf_time = stof(str_read);
//         // stats.push_back(bf_time);
        
//         // // getting nn runs
//         // str_start = line.find(",", str_end+1);
//         // str_end = line.find(",", str_start+1);
//         // str_read = line.substr(str_start, str_end-str_start);
//         // float nn_runs = stof(str_read);
//         // cout << nn_runs << endl;
//         // stats.push_back(nn_runs);
                
//         // // getting nn time
//         // str_start = line.find(",", str_end+1);
//         // str_end = line.find(",", str_start+1);
//         // str_read = line.substr(str_start, str_end-str_start);
//         // float nn_time = stof(str_read);
//         // cout << nn_time << endl;
//         // stats.push_back(nn_time);
//     }
//     file.close();
// }

WDigraph buildGraph(unordered_map<string, long long>& flights, unordered_map<string, airport>& airportInfo) {
    WDigraph airportGraph;
    string path, start_name, end_name;

    // Iterate over the flights using iterator
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
        // cout << start_name << " " << start_id << " " << end_name << " " << end_id << " " << i->second << endl;
        if (start_id >= 0 && end_id >= 0) {
            airportGraph.addEdge(start_id, end_id, dist);
        }
        ++i;
    }
    return airportGraph;
}

int main() {

    // unordered_map<int, pair<int, float>> bruteStats;
    // unordered_map<int, pair<int, float>> neighbourStats;
    // unordered_map<int, vector<float>> runStats;
    
    // string timer_data = "data/timerData.csv";
    // readData(runStats, timer_data);

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

                if (n > 5) {
                    char p;
                    cout << "Are you sure? This calculation may take a long time." << endl << "(1) Yes" << endl << "(2) No" << endl;
                    cin >> p;
                    if (p == '1') {
                        cin.clear();
                        startTime = clock();
                        bruteforce(destinations, airports, flights, pathGraph, idsToAirports);
                        timeTaken = (clock() - startTime)/CLOCKS_PER_SEC;

                        cout << "Run time was: " << (float)(clock() - startTime)/CLOCKS_PER_SEC << "s" << endl;
                        cout << endl;

                    }
                }

            } else if (c == '2') {
                startTime = clock();
                modifiedNearestNeighbour(pathGraph, destinations, flights, idsToAirports, airports);
                timeTaken = (clock() - startTime)/CLOCKS_PER_SEC;
                cout << "Run time was: " << (float)(clock() - startTime)/CLOCKS_PER_SEC << "s" << endl;
            } else if (c == '3') {
                break;
            } else if (c == '4') {
                cout << "Exiting Program" << endl;
                return 0;
            } else {
                cout << "Invalid input" << endl;
                cin.clear();
            }
            cout << endl;
        }  
    }
    return 0;
}

