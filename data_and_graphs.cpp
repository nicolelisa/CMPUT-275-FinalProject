#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <string>
#include <list>
#include <utility>
#include "wdigraph.h"
#include "digraph.h"
#include "airport.h"
#include "bruteforce.h"
#include "nn.h"

using namespace std;

#define ll long long

void readAirports(unordered_map<string, airport>& airports, string filename) {
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
    }
    file.close();
}

void readFlights(unordered_map<string, ll>& flights, const vector<string>& destinations, string filename, unordered_map<string, airport>& airports) {
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
        
        // checks if the start destination of the flight is in the user inputted list
        if(find(destinations.begin(), destinations.end(), start) != destinations.end()) {

        // gets ending airport
        str_start = line.find(",", str_end+1);
        str_end = line.find(",", str_start+1);
        end = line.substr(str_start+1, str_end-str_start-1);

            // checks if the end destination of the flight is in the user inputted list
            if (find(destinations.begin(), destinations.end(), end) != destinations.end()) {
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
                        // cout << "Distance between destinations " << path << " is: " << distance << endl;
                        flights.insert({path, distance});
                    }
                }
            }
        }
    }
    file.close();
}

int main() {
    
    string a;

    /* Read in Airports from CSV file */
    unordered_map<string, airport> airports;
    readAirports(airports, "data/airportData.csv");
    //   cout << "Enter an airport: ";
    //   cin >> a;
    //   unordered_map<string,airport>::const_iterator got = airports.find (a);
    // if ( got == airports.end() )
    //   cout << "not found";
    // else
    //   cout << got->first << " is " << got->second.id;
    // cout << endl;

    
    /* Read in user list of airports and create a vector from the airports */
    vector<string> destinations;
    int n;
    cout << "How many destinations would you like to visit? ";
    cin >> n;
    cout << "Enter a list of airports: ";
    for (int i = 0; i<n; ++i) {
        cin >> a;
        destinations.push_back(a);
    }
    cout << endl;

    /* Read in Flights to a Hash table */
    auto it = destinations.begin();
    while (it != destinations.end()){
        auto exists = airports.find(*it);
        if (exists == airports.end()) {
            cout << *it << ": Airport not found and was removed from list" << endl;
            it = destinations.erase(it);
            --n;
        } else {
            cout << *it << ": Airport exists and is okay" << endl; 
            ++it;
        }
    }
    cout << "There are " << n << " airports: ";
    for (auto i = destinations.begin(); i != destinations.end(); ++i) {
        cout << *i << " ";
    }
    cout << endl;
    string flight_data = "data/flightData.csv";
    unordered_map<string,ll> flights;
    readFlights(flights, destinations, flight_data, airports);
    string start,end;
    cout << "Enter a starting airport for the flight: ";
    cin >> start;
    cout << "Enter an end airport for the flight: ";
    cin >> end;
    a = start + end;
    auto got = flights.find(a);
    if (got == flights.end())
      cout << "Not Found";
    else
      cout << "Distance between desintations " << got->first << " is: " << got->second;
    cout << endl;

    /* Testing for brute force */
    // vector<string> path = bruteforce(destinations, airports);
    // cout << "Result: ";
    // for (vector<string>::const_iterator i = path.begin(); i != path.end(); ++i) {
    //     cout << *i << ' ';
    // }
}

