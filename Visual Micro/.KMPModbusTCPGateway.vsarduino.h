/* 
	Editor: http://www.visualmicro.com
	        visual micro and the arduino ide ignore this code during compilation. this code is automatically maintained by visualmicro, manual changes to this file will be overwritten
	        the contents of the Visual Micro sketch sub folder can be deleted prior to publishing a project
	        all non-arduino files created by visual micro and all visual studio project or solution files can be freely deleted and are not required to compile a sketch (do not delete your own code!).
	        note: debugger breakpoints are stored in '.sln' or '.asln' files, knowledge of last uploaded breakpoints is stored in the upload.vmps.xml file. Both files are required to continue a previous debug session without needing to compile and upload again
	
	Hardware: Arduino Ethernet, Platform=avr, Package=arduino
*/

#define __AVR_ATmega328p__
#define __AVR_ATmega328P__
#define ARDUINO 160
#define ARDUINO_MAIN
#define F_CPU 16000000L
#define __AVR__
extern "C" void __cxa_pure_virtual() {;}

//
//
void Long2doubleInt(int *lo, int *hi, long lValue);
long kamReadReg(unsigned short kreg);
void kamSend(byte const *msg, int msgsize);
unsigned short kamReceive(byte recvmsg[]);
long kamDecode(unsigned short const kreg, byte const *msg);
long crc_1021(byte const *inmsg, unsigned int len);
void printEthernetStatus();
void printWiFiStatus();

#include "C:\Program Files (x86)\Arduino\hardware\arduino\avr\variants\ethernet\pins_arduino.h" 
#include "C:\Program Files (x86)\Arduino\hardware\arduino\avr\cores\arduino\arduino.h"
#include <KMPModbusTCPGateway.ino>
