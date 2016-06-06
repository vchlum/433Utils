/*
  RFSniffer

  Usage: ./RFSniffer [<pulseLength>]
  [] = optional

  Hacked from http://code.google.com/p/rc-switch/
  by @justy to provide a handy RF code sniffer
*/

#include "RCSwitch.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
     
     
RCSwitch mySwitch;
 
void getBin(int num, char *str) {
     *(str+5) = '\0';
     int mask = 0x10 << 1;
     while(mask >>= 1)
     *str++ = !!(mask & num) + '0';
  }


int main(int argc, char *argv[]) {
  
     // This pin is not the first pin on the RPi GPIO header!
     // Consult https://projects.drogon.net/raspberry-pi/wiringpi/pins/
     // for more information.
     int PIN = 2;
     char bincode[300];
     
     if(wiringPiSetup() == -1) {
       printf("wiringPiSetup failed, exiting...");
       return 0;
     }

     int pulseLength = 0;
     if (argv[1] != NULL) pulseLength = atoi(argv[1]);

     mySwitch = RCSwitch();
	 if (pulseLength != 0) mySwitch.setPulseLength(pulseLength);
     mySwitch.enableReceive(PIN);  // Receiver on interrupt 0 => that is pin #2
     
     while(1) {
  
      if (mySwitch.available()) {
    
        unsigned int value = mySwitch.getReceivedValue();
    
        if (value == 0) {
          printf("Unknown encoding\n");
        } else {    
   
          int n, c, k;
          for (c = 100; c >= 0; c--)  {
            k = value >> c;
            if (k & 1)
              printf("1");
            else
              printf("0");
            }
          printf("\n");
          printf("Received %lu\n", mySwitch.getReceivedValue() );
        }
    
        mySwitch.resetAvailable();
    
      }
      
  
  }

  exit(0);


}

