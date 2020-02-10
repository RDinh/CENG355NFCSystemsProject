#include <stdlib.h>
#include <string.h>
#include <nfc/nfc.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <wiringPiI2C.h>


//NFC Variables
 nfc_device *pnd;
 nfc_context *context;


int CardTransmit(nfc_device *pnd, uint8_t * capdu, size_t capdulen, uint8_t * rapdu, size_t * rapdulen)
{
  int res;
  size_t  szPos;
  printf("=> ");
  for (szPos = 0; szPos < capdulen; szPos++) {
    printf("%02x ", capdu[szPos]);
  }
  printf("\n");
  if ((res = nfc_initiator_transceive_bytes(pnd, capdu, capdulen, rapdu, *rapdulen, 500)) < 0) {
    return -1;
  } else {
    *rapdulen = (size_t) res;
    printf("<= ");
    for (szPos = 0; szPos < *rapdulen; szPos++) {
      printf("%02x ", rapdu[szPos]);
    }
    printf("\n");
    return 0;
  }
}


void getNdef(nfc_device *pnd, nfc_context *context)
{
	nfc_target nt;
/*
   nfc_device *pnd;
  nfc_target nt;
  nfc_context *context;
  nfc_init(&context);

  if (context == NULL) {
    printf("Unable to init libnfc (malloc)\n");
    exit(EXIT_FAILURE);
  }
 
  pnd = nfc_open(context, NULL);

  if (pnd == NULL) {
    printf("ERROR: %s", "Unable to open NFC device.");
    exit(EXIT_FAILURE);
  }
  if (nfc_initiator_init(pnd) < 0) {
    nfc_perror(pnd, "nfc_initiator_init");
    exit(EXIT_FAILURE);
  }

  printf("NFC reader: %s opened\n", nfc_device_get_name(pnd));
*/

 const nfc_modulation nmMifare = {
    .nmt = NMT_ISO14443A,
    .nbr = NBR_106,
  };
  // nfc_set_property_bool(pnd, NP_AUTO_ISO14443_4, true);
  printf("Polling for target...\n");
  while (nfc_initiator_select_passive_target(pnd, nmMifare, NULL, 0, &nt) <= 0);
  printf("Target detected!\n");
  uint8_t capdu[264];
  size_t capdulen;
  uint8_t rapdu[264];
  size_t rapdulen;
  // Select application
  //memcpy(capdu, "\x00\xA4\x04\x00\xF2\x22\x22\x22\x22", 9); 
  memcpy(capdu, "\x00\xA4\x04\x00\x07\xF0\x39\x41\x48\x14\x81\x00\x00", 13);
  capdulen=13;
  rapdulen=sizeof(rapdu);
  if (CardTransmit(pnd, capdu, capdulen, rapdu, &rapdulen) < 0)
    exit(EXIT_FAILURE);
  if (rapdulen < 2 || rapdu[rapdulen-2] != 0x90 || rapdu[rapdulen-1] != 0x00)
    exit(EXIT_FAILURE);
  printf("Application selected!\n");

  // Select Capability Container
  memcpy(capdu, "\x00\xa4\x00\x0c\x02\xe1\x03", 7);  
  capdulen=7;
  rapdulen=sizeof(rapdu);
  if (CardTransmit(pnd, capdu, capdulen, rapdu, &rapdulen) < 0)
    exit(EXIT_FAILURE);
  if (rapdulen < 2 || rapdu[rapdulen-2] != 0x90 || rapdu[rapdulen-1] != 0x00) {
    capdu[3]='\x00'; // Maybe an older Tag4 ?
    if (CardTransmit(pnd, capdu, capdulen, rapdu, &rapdulen) < 0)
      exit(EXIT_FAILURE);
  }
  printf("Capability Container selected!\n");

  // Read Capability Container
  memcpy(capdu, "\x00\xb0\x00\x00\x0f", 5);  
  capdulen=5;
  rapdulen=sizeof(rapdu);
  if (CardTransmit(pnd, capdu, capdulen, rapdu, &rapdulen) < 0)
    exit(EXIT_FAILURE);
  if (rapdulen < 2 || rapdu[rapdulen-2] != 0x90 || rapdu[rapdulen-1] != 0x00)
    exit(EXIT_FAILURE);
  printf("Capability Container header:\n");
  size_t  szPos;
  for (szPos = 0; szPos < rapdulen-2; szPos++) {
    printf("%02x ", rapdu[szPos]);
  }
  printf("\n");

 // NDEF SELECT
  memcpy(capdu, "\x00\xA4\x00\x0C\x02\xE1\x04", 7);  
  capdulen=7;
  rapdulen=sizeof(rapdu);
  if (CardTransmit(pnd, capdu, capdulen, rapdu, &rapdulen) < 0)
    exit(EXIT_FAILURE);
  if (rapdulen < 2 || rapdu[rapdulen-2] != 0x90 || rapdu[rapdulen-1] != 0x00)
    exit(EXIT_FAILURE);
  printf("NDEF SELECTED!\n");


  // NDEF Read Binary
  memcpy(capdu, "\x00\xb0\x00\x00\x02", 5);  
  capdulen=5;
  rapdulen=sizeof(rapdu);
  if (CardTransmit(pnd, capdu, capdulen, rapdu, &rapdulen) < 0)
    exit(EXIT_FAILURE);
  if (rapdulen < 2 || rapdu[rapdulen-2] != 0x90 || rapdu[rapdulen-1] != 0x00)
    exit(EXIT_FAILURE);
  printf("NDEF Read Binary NLEN!\n");

//READING NDEF DATA  
 memcpy(capdu, "\x00\xb0\x00\x00\x0f", 5);  
  capdulen=5;
  rapdulen=sizeof(rapdu);
  if (CardTransmit(pnd, capdu, capdulen, rapdu, &rapdulen) < 0)
    exit(EXIT_FAILURE);
  if (rapdulen < 2 || rapdu[rapdulen-2] != 0x90 || rapdu[rapdulen-1] != 0x00)
    exit(EXIT_FAILURE);
  printf("NDEF DATA ! \n\n");
  size_t  szPos2;
  char ndefMsg[100] = "";
  char chr[1] = "\0";
  int counter = 0;
  for (szPos2 = 8; szPos2 < rapdulen-2; szPos2++) {
    //chr = (char)rapdu[szPos2];
   // printf("%c", (char)rapdu[szPos2]);
    ndefMsg[counter]=(char)rapdu[szPos2];
    counter++;
   // printf("%s",ndefMsg);

    //printf("%c", chr);
   //sprintf(chr,"%c",(char)rapdu[szPos2]);
   //strncat(ndefMsg,chr,1);
   //printf("%s",ndefMsg);
  }
 // ndefMsg[counter]='\0';
  printf("%s",ndefMsg);
 
  char cmdS[100] = "python lcdDisplay.py Phone Scanned\n";
 // strcat(cmdS,ndefMsg);
 

 system(cmdS);
	sleep(1);
  printf("\nDone...\n");
}


void stopFunction(int sig)
{

	printf("Stopping now.\n");
	nfc_close(pnd);
	nfc_exit(context);
	exit(EXIT_SUCCESS);

}
int main(void)

{
	signal(SIGINT,stopFunction);	
 	nfc_init(&context);
 	 if (context == NULL) {
  	  printf("Unable to init libnfc (malloc)\n");
  	  exit(EXIT_FAILURE);
 	 }
 
 	 pnd = nfc_open(context, NULL);

 	 if (pnd == NULL) {
 	   printf("ERROR: %s", "Unable to open NFC device.");
 	   exit(EXIT_FAILURE);
 	 }
 	 if (nfc_initiator_init(pnd) < 0) {
  	  nfc_perror(pnd, "nfc_initiator_init");
  	  exit(EXIT_FAILURE);
  	}

  	printf("NFC reader: %s opened\n", nfc_device_get_name(pnd));

	int fd = wiringPiI2CSetup(0x13);
	
	//Select Command Register
	wiringPiI2CWriteReg8(fd,0x80,0xFF);
	
	//Select Prox Register
	wiringPiI2CWriteReg8(fd,0x82,0x03);

	//Select Current for IR LEDto 200ma
	wiringPiI2CWriteReg8(fd,0x83,0x14);

	//Select ALS Register
	wiringPiI2CWriteReg8(fd,0x84,0x9D);

	int lux1= 0;
	int lux2= 0;

	int prox1= 0;
	int prox2= 0;
	int proxT= 0;

	system("python lcdDisplay.py Waiting...");

	while (1)
	{
		
		lux1 = wiringPiI2CReadReg8(fd,0x85) *256;
		lux2 = wiringPiI2CReadReg8(fd,0x86);


		prox1 = wiringPiI2CReadReg8(fd,0x87) *256;
		prox2 = wiringPiI2CReadReg8(fd,0x88);

		printf("Luminosity: %d lux\n",lux1+lux2);
		printf("Proximity: %d \n",prox1+prox2);
		
		proxT= prox1+prox2;

		if (proxT > 2300)
		{
			getNdef(pnd,context);
			sleep(1);
			system("python lcdDisplay.py Waiting...");
		}

		usleep(300000);
	}

	return 0;


}
