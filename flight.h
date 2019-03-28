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
  char name[3]; // Index of restaurant from 0 to NUM_RESTAURANTS-1.
  uint16_t dist;  // Manhatten distance to cursor position.
};


#endif
