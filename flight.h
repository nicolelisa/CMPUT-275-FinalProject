/*
  Contains the restaurant struct declaration, functions to read restaurants
  from the SD card, and functions to sort a restaurant around a given
  latitude/longitude
*/

#ifndef _FLIGHT_H_
#define _FLIGHT_H_

#include "flight.h"

#define NUM_FLIGHTS  15190

typedef struct {
  string start; // 
  string end; // 
  int id_start; // 
  int id_end; // 
  uint16_t dist;  // Distance between the two airports.
};

#endif
