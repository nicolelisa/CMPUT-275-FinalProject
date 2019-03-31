/*
  Contains the restaurant struct declaration, functions to read restaurants
  from the SD card, and functions to sort a restaurant around a given
  latitude/longitude
*/

#ifndef _AIRPORT_H_
#define _AIRPORT_H_

#include "airport.h"

#define NUM_AIRPORTS  4578

struct airport {
  string name; // Index of flight from 0 to NUM_FLIGHTS-1.
  int id; // Index of restaurant from 0 to NUM_FLIGHTS-1.
  int32_t lat;
  int32_t lon;
};

#endif
