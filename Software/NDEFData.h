/**
 * \file NDEFData.h
 * \brief Function prototypes for NDEFData
 * \author Robert Dinh
 * \date 01MAR2020
*/

#ifndef NDEFData_H
#define NDEFData_H


///\cond INTERNAL
// Function Prototypes
void stopFunction(int sig);
void initNFC(void);
int CardTransmit(nfc_device *pnd, uint8_t * capdu, size_t capdulen, uint8_t * rapdu, size_t * rapdulen);
void getNdef(void);
///\endcond



#endif