#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <string>
#include <list>
#include "wdigraph.h"
#include "digraph.h"
#include "airport.h"

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
        str_start = 0;
        str_end = line.find(",", str_start+1);
        // cout << "START END " << str_start << "  " << str_end << endl;
        str_read = line.substr(str_start, str_end-str_start);
        int32_t id = stoi(str_read);
        currentAirport.id = id;

        // cout << "LINE: " << line << endl;
        // cout << "  ID: " << str_read;
        str_end = line.find(",", str_start+1);
        for (int i = 0; i < 4; i++) {
            str_start = line.find(",", str_start+1);
        }

        name = line.substr(str_start+1, 3);
        currentAirport.name = name;
        // cout << "  NAME: " << name;
        
        for (int i = 0; i < 2; i++) {
            str_start = line.find(",", str_start+1);
        }

        str_end = line.find(",", str_start+1);
        str_read = line.substr(str_start+1, str_end-str_start-1);
        int32_t lat = static_cast <int32_t> (stof(str_read) * 100000);
        // cout << "  LAT: " << lat;
        currentAirport.lat = lat;

        str_start = line.find(",", str_start+1);
        str_end = line.find(",", str_start+1);
        str_read = line.substr(str_start+1, str_end-str_start-1);
        int32_t lon = static_cast <int32_t> (stof(str_read) * 100000);
        // cout << "  LON: " << lon << endl;
        currentAirport.lon = lon;

        airports.insert({name, currentAirport});
    }
    file.close();
}

void readInData(WDigraph& flights, string filename) {
    //string file = "CanadaAirportFlights.csv";
    ifstream file;
    string start, end, line;
    int found1, found2, iStart, iEnd;
  
    file.open(filename);

    if (file.fail()) {
        cerr << "Error encountered. File was not read." << endl;
        exit(0);
    }

    //throws away the first line which is just labels
    getline(file, start);

    int counter = 1;

    while (getline(file, line)) {
        found1 = 0;

        for (int i = 0; i < 3; i++) {
            found1 = line.find(",", found1+1);
        }
       
        found2 = line.find(",", found1+1);
        start = line.substr(found1+1, found2-found1-1);
      
        found1 = line.find(",", found2+1);
        found2 = line.find(",", found1+1);
        end = line.substr(found1+1, found2-found1-1);

        cout << counter << ": ";
        cout << "Airport 1: " << start << " ";
        cout << "Airport 2: " << end << endl;

        if (start.substr(1,1) != "N" && end.substr(1,1) != "N") {
            iStart = stoi(start);
            // flights.addVertex(iStart);
        }
        
        if (end.substr(1,1) != "N") {
            iEnd = stoi(end);
            // flights.addVertex(iEnd);
        }

        // Still need to get their long and lat then add the weighted edge to the graphs!!!!

        counter++;
    }
    file.close();
}

ll findDistance(ll lon1, ll lat1, ll lon2, ll lat2) {
    ll distance = sqrt((lon1-lon2)*(lon1-lon2) + (lat1-lat2)*(lat1-lat2));
    return distance;
}


int main() {
    string a;
    // WDigraph flights;
    // readInData(flights, "data/flightData.csv");
    unordered_map<string, airport> airports;
    readAirports(airports, "data/airportData.csv");
    cout << "Enter an airport: ";
    cin >> a;
    unordered_map<string,airport>::const_iterator got = airports.find (a);

  if ( got == airports.end() )
    cout << "not found";
  else
    cout << got->first << " is " << got->second.id;

  cout << endl;
    // cout << "ID of airport " << a << " is " << airports.find(a).id << endl;
}

