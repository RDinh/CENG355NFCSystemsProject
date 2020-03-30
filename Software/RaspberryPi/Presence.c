/**
 * \file Presence.c
 * \brief Various functions to aid in Presence Detection with the VNCL4010
 * \author Robert Dinh
 * \date 01MAR2020
 *
 */
#include <stdio.h>
#include <wiringPiI2C.h>
#include <unistd.h>
#include "Presence.h"


/**
 * \brief Function used to initialize Presence.
 *  
 *	The fuction sets up the VNCL4010 using the Wiring Pi library through I2C.
 *	It then sets up the registers of the device so that data can be retrieved.
 */
int initPresence(void)
{
	//Setup Wiring Pi thorugh I2C
	int fd = wiringPiI2CSetup(0x13);
	
	//Select Command Register
	wiringPiI2CWriteReg8(fd,0x80,0xFF);
	
	//Select Prox Register
	wiringPiI2CWriteReg8(fd,0x82,0x03);

	//Select Current for IR LEDto 200ma
	wiringPiI2CWriteReg8(fd,0x83,0x14);

	//Select ALS Register
	wiringPiI2CWriteReg8(fd,0x84,0x9D);
	
	return fd;
}

/**
 * \brief Function used to detect the presence of a person.
 *  
 */
int detectPresence(int fd)
{
		presenceD pData  = {0};
		while (1)
		{
			pData.proximity = getProximity(fd);
			pData.luminosity = getLuminosity(fd);
			
			fprintf(stdout,"Proximity:\t%d\t Luminosity:\t%d\n",pData.proximity,pData.luminosity);
			if (pData.proximity > 3100)
				return pData.proximity;
			
			usleep(300000);
		}
	
}

/**
 * \brief Function used get the proximity of the sensor.
 *  
 */
int getProximity(int fd)
{
	int prox1= 0;
	int prox2= 0;
	
	prox1 = wiringPiI2CReadReg8(fd,0x87) *256; //Upper Byte
	prox2 = wiringPiI2CReadReg8(fd,0x88); //Lower Byte
	
	return prox1+prox2;
}

/**
 * \brief Function used get the luminosity of the sensor.
 *  
 */
int getLuminosity(int fd)
{
	int lux1= 0; 
	int lux2= 0; 
	
	lux1 = wiringPiI2CReadReg8(fd,0x85) *256; //Upper Byte
	lux2 = wiringPiI2CReadReg8(fd,0x86); //Lower Byte
	
	return lux1 + lux2;
}
