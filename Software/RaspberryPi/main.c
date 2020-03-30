/**
 * \file main.c
 * \brief Main Program that will run the VNCL 4010, PN532 and LCD
 * \author Robert Dinh
 * \date 01MAR2020
*/

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <nfc/nfc.h>
#include "Presence.h"
#include "NDEFData.h"


/**
 * \brief Function used to initialize Presence and setup the NFC reader.
 *  
 *	The fuction sets up the VNCL4010 and PN532 using the Wiring Pi library through I2C.
 *	It then sets up the registers of the devices so that data can be retrieved. When a
 *	presence is detected by the VNCL4010 it triggers the NFC reader into a read state to
 *	to grab an NDEF encoded message from a mobile device.
 *
 */
int main (void)
{
	initNFC();
	int fd = initPresence();
	
	int present = 0;
	
	fprintf(stdout,"%d \n",fd);
	present = detectPresence(fd);
	if (present)
		getNdef();
	
	stopFunction(2);
	return 0;
}
