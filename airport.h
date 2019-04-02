/*
  Contains the restaurant struct declaration, functions to read restaurants
  from the SD card, and functions to sort a restaurant around a given
  latitude/longitude
*/

#ifndef _AIRPORT_H_
#define _AIRPORT_H_
#include <math.h>
#include <iostream>
#include "airport.h"

// https://rosettacode.org/wiki/Haversine_formula
using namespace std;

const static double EarthRadiusKm = 6372.8;
 
inline double DegreeToRadian(double angle)
{
	return M_PI * angle / 180.0;
}
 
class Coordinate
{
public:
	Coordinate(double latitude ,double longitude):myLatitude(latitude), myLongitude(longitude)
	{}
 
	double Latitude() const
	{
		return myLatitude;
	}
 
	double Longitude() const
	{
		return myLongitude;
	}
 
private:
 
	double myLatitude;
	double myLongitude;
};
 
long long HaversineDistance(const Coordinate& p1, const Coordinate& p2)
{
	double latRad1 = DegreeToRadian(p1.Latitude());
	double latRad2 = DegreeToRadian(p2.Latitude());
	double lonRad1 = DegreeToRadian(p1.Longitude());
	double lonRad2 = DegreeToRadian(p2.Longitude());
 
	double diffLa = latRad2 - latRad1;
	double doffLo = lonRad2 - lonRad1;
 
	double computation = asin(sqrt(sin(diffLa / 2) * sin(diffLa / 2) + cos(latRad1) * cos(latRad2) * sin(doffLo / 2) * sin(doffLo / 2)));
	return 2 * EarthRadiusKm * computation;
}
 

struct airport {
  string name; // Index of flight from 0 to NUM_FLIGHTS-1.
  int id; // Index of restaurant from 0 to NUM_FLIGHTS-1.
  float lat;
  float lon;
};

#endif
