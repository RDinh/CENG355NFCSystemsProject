/**
 * \file Presence.h
 * \brief Constants, structures, function prototypes for Presence
 * \author Robert Dinh
 * \date 01MAR2020
*/

#ifndef PRESENCE_H
#define PRESENCE_H

#include <wiringPiI2C.h>

// Structures
typedef struct PresenceData
{
		int proximity; 		///<Proximity - Combination of upper byte and lower byte.
		int luminosity;		///<Luminoxity - Combination of upper byte and lower byte.
}presenceD;

///\cond INTERNAL
// Function Prototypes
int initPresence(void); 
int detectPresence(int fd);
int getProximity(int fd);
int getLuminosity(int fd);
///\endcond



#endif