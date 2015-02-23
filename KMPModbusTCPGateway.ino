//#define Wireless

#include <KMPSoftwareSerial.h>
#include <Mudbus.h>
#include <SPI.h>

#ifndef Wireless
#include <Ethernet.h>
#endif

#ifdef Wireless
#include <WiFi.h>
#endif

// Modbus
Mudbus Mb;

#ifdef Wireless
char ssid[] = "Test";        // Network SSID (name) 
char pass[] = "Pwd1234!";    // Network password
int status = WL_IDLE_STATUS; // Status of the Wifi radio
#endif

// Kamstrup Multical 602
word const kregnums[] = { 0x003C,0x0050,0x0056,0x0057,0x0059,0x004a,0x004b,0x0044,0x0045,0x0048,0x0049,0x0054,0x0055,0x0063,0x0092,0x0093,0x0095,0x0096,0x007f,0x0080 };
//                            60,    80,    86,    87,    89,    74     75     68     69     72     73     84     85     99    146    147    149    150    127    128

// char* kregstrings[] = { "E1","EFFEKT1","T1","T2","T1-T2","FLOW1","FLOW2","V1","V2","M1","M2","VA","VB","INFO","AVG-T1-AAR","AVG-T2-AAR","AVG-T1-MND","AVG-T2-MND","MAX-EFKT1-AAR","MAX-EFKT1-AAR-DATO" };
#define NUMREGS 7            // Number of registers that are to be read from the Kamstrup
#define KAMBAUD 1200         // Baud rate of the Kamstrup KMP Data connection

// Registers
int iKMPAddr = 0;            // KMP address counter
int iMbAddr = 0;             // Modbus address counter

// Arduino Uno pin allocations
#define PIN_KAMSER_RX  5     // Kamstrup Data DAT/62 (Yellow wire)
#define PIN_KAMSER_TX  6     // Kamstrup Data REQ/63 (Green wire)
#define PIN_LED        13    // Standard Arduino LED

// Kamstrup KMP serial
#define KAMTIMEOUT 300       // Kamstrup timeout after transmit
KMPSoftwareSerial kamSer(PIN_KAMSER_RX, PIN_KAMSER_TX, true);  // Initialize serial

/******************************/
/**                          **/
/**          Setup           **/
/**                          **/
/******************************/
// Initialization
void setup()
{
  Serial.begin(9600);
  
  // Just for debug
  pinMode(PIN_LED, OUTPUT);
  digitalWrite(PIN_LED, 0);
  
  // Setup the pins for Kamstrup serial communication
  pinMode(PIN_KAMSER_RX,INPUT);
  pinMode(PIN_KAMSER_TX,OUTPUT);
  kamSer.begin(KAMBAUD);
  
  #ifdef Wireless
  // Check if the WiFi Shield is present 
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("Wifi Shield not present");
    while(true);
  }

  // Wait for the WiFi connection to be established
  while ( status !=WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    status = WiFi.begin(ssid, pass);
    delay(1000);
  }
  #endif
   
  #ifndef Wireless 
  // Initialize Ethernet
  uint8_t mac[]     = { 0x90, 0xA2, 0xDA, 0x00, 0x51, 0x06 };
  Ethernet.begin(mac);

  // Show Ethernet details
  printEthernetStatus();
  #endif
  
  // Open the Wifi card for communication
  Mb.Begin();
}

/******************************/
/**                          **/
/**           Loop           **/
/**                          **/
/******************************/
// Main loop
void loop()
{
  int iHi, iLo;

  // Read a Kanstrup 602 register and place it the Modbus memory map
  long lKMPReg = kamReadReg(iKMPAddr);
  
  // Prepare Modbus registers
  switch (iKMPAddr) {
    // ID 60 - E1: Energiregister 1: Varmeenergi
    // Modbus Addr 0 and 1
    case 0:
      Long2doubleInt(&iLo, &iHi, lKMPReg);
      Mb.R[iMbAddr] = iHi;
      Mb.R[iMbAddr+1] = iLo;;
      iMbAddr++;
      break;
    // ID 80 - EFFEKT1: Aktuel effekt beregnet på baggrud af V1-T1-T2
    // Modbus Addr 2
    case 1:
      Mb.R[iMbAddr] = lKMPReg; 
      break;
    // ID 86 - T1: Aktuel fremløbstemperatur
    // Modbus Addr 3
    case 2:
      Mb.R[iMbAddr] = lKMPReg; 
      break;
    // ID 87 - T2: Aktuel returløbstemperatur
    // Modbus Addr 4
    case 3:
      Mb.R[iMbAddr] = lKMPReg; 
      break;
    // ID 89 - T1-T2: Aktuel temperaturdifferens
    // Modbus Addr 5
    case 4:
      Mb.R[iMbAddr] = lKMPReg; 
      break;
    // ID 74 - FLOW1: Aktuel flow i fremløb
    // Modbus Addr 6 og 7
    case 5:
    Long2doubleInt(&iLo, &iHi, lKMPReg);
      Mb.R[iMbAddr] = iHi;
      Mb.R[iMbAddr+1] = iLo;;
      iMbAddr++;
      break;
    // ID 75 - FLOW2: Aktuel flow i returløb
    // Modbus Addr 8 og 9
    case 6:
    Long2doubleInt(&iLo, &iHi, lKMPReg);
      Mb.R[iMbAddr] = iHi;
      Mb.R[iMbAddr+1] = iLo;;
      iMbAddr++;
      break;
    // ID 68 - V1: Volumenregister V1
    // Modbus Addr 10 and 11
    case 7:
      Long2doubleInt(&iLo, &iHi, lKMPReg);
      Mb.R[iMbAddr] = iHi;
      Mb.R[iMbAddr+1] = iLo;;
      iMbAddr++;
      break;
    // ID 69 - V2: Volumenregister V2
    // Modbus Addr 12 and 13
    case 8:
      Long2doubleInt(&iLo, &iHi, lKMPReg);
      Mb.R[iMbAddr] = iHi;
      Mb.R[iMbAddr+1] = iLo;;
      iMbAddr++;
      break;
    // ID 72 - M1: Masseregister V1
    // Modbus Addr 14 and 15
    case 9:
      Long2doubleInt(&iLo, &iHi, lKMPReg);
      Mb.R[iMbAddr] = iHi;
      Mb.R[iMbAddr+1] = iLo;;
      iMbAddr++;
      break;
    // ID 73 - M2: Masseregister V2
    // Modbus Addr 16 and 17
    case 10:
      Long2doubleInt(&iLo, &iHi, lKMPReg);
      Mb.R[iMbAddr] = iHi;
      Mb.R[iMbAddr+1] = iLo;;
      iMbAddr++;
      break;
    // ID 84 - VA: Inputregister VA
    // Modbus Addr 18 and 19
    case 11:
      Long2doubleInt(&iLo, &iHi, lKMPReg);
      Mb.R[iMbAddr] = iHi;
      Mb.R[iMbAddr+1] = iLo;;
      iMbAddr++;
      break;
    // ID 85 - VB: Inputregister VB
    // Modbus Addr 20 and 21
    case 12:
      Long2doubleInt(&iLo, &iHi, lKMPReg);
      Mb.R[iMbAddr] = iHi;
      Mb.R[iMbAddr+1] = iLo;;
      iMbAddr++;
      break;
    // ID 99 - INFO: Infokode register, aktuelt
    // Modbus Addr 22 and 23
    case 13:
     Long2doubleInt(&iLo, &iHi, lKMPReg);
      Mb.R[iMbAddr] = iHi;
      Mb.R[iMbAddr+1] = iLo;;
      iMbAddr++;
      break;
    // ID 146 - AVR T1/ÅR: År til dato gennemsnit for T1
    // Modbus Addr 24
    case 14:
      Mb.R[iMbAddr] = lKMPReg; 
      break;
    // ID 147 - AVR T2/ÅR: År til dato gennemsnit for T2
    // Modbus Addr 25
    case 15:
      Mb.R[iMbAddr] = lKMPReg; 
      break;
    // ID 149 - AVR T1/MND: Måned til dato gennemsnit for T1
    // Modbus Addr 26
    case 16:
      Mb.R[iMbAddr] = lKMPReg; 
      break;
    // ID 150 - AVR T2/MND: Måned til dato gennemsnit for T2
    // Modbus Addr 27
    case 17:
      Mb.R[iMbAddr] = lKMPReg; 
      break;
    // ID 127 - MAX EFFEKT1DATE/ÅR: Dato for max i indeværende år
    // Modbus Addr 28 and 29
    case 18:
      Long2doubleInt(&iLo, &iHi, lKMPReg);
      Mb.R[iMbAddr] = iHi;
      Mb.R[iMbAddr+1] = iLo;;
      iMbAddr++;
      break;
    // ID 128 - MAX EFFEKT1/ÅR: Max. værdi i indeværende år
    // Modbus Addr 30
    case 19:
      Mb.R[iMbAddr] = lKMPReg; 
      break;
  }
    
  // Move to the next registers
  iKMPAddr++;
  iMbAddr++;
  
  // Roll registers
  if (iKMPAddr > 19) {
    iKMPAddr = 0;
    iMbAddr = 0;
  }

  // Update Modbus memory map  
  Mb.Run();
}

/******************************/
/**                          **/
/**     Long2dooubleInt      **/
/**                          **/
/******************************/
// Convert long to two ints
void Long2doubleInt(int *lo, int *hi, long lValue) {
   *lo = (int)(lValue & 0xffffffff);
   *hi = (int)(lValue >> 16);
}

/******************************/
/**                          **/
/**       KamReadReg         **/
/**                          **/
/******************************/
// kamReadReg - read a Kamstrup register
// float kamReadReg(unsigned short kreg) {
long kamReadReg(unsigned short kreg) {
  
  byte recvmsg[40];     // buffer of bytes to hold the received data
  // float rval;        // this will hold the final value
  long rval;            // this will hold the final value

  // Prepare the message frame and send it to the Kamstrup
  byte sendmsg[] = { 0x3f, 0x10, 0x01, (kregnums[kreg] >> 8), (kregnums[kreg] & 0xff) };
  kamSend(sendmsg, 5);

  // Listen for an answer
  unsigned short rxnum = kamReceive(recvmsg);

  // Check if number of received bytes > 0 
  if(rxnum != 0){
    
    // Decode the received message
    rval = kamDecode(kreg, recvmsg);
  }
  else
  {
    // if INFO register, return 2 (bit for communication error)
    if(kreg == 13) {
      rval = 2;
    }
    else {
      // If no answer and normal register, return 0
      rval = 0;
    }
  }
  
  return rval;
}

/******************************/
/**                          **/
/**         KamSend          **/
/**                          **/
/******************************/
// kamSend - send data to Kamstrup meter
void kamSend(byte const *msg, int msgsize) {

  // Append checksum bytes to message
  byte newmsg[msgsize+2];
  for (int i = 0; i < msgsize; i++) { newmsg[i] = msg[i]; }
  newmsg[msgsize++] = 0x00;
  newmsg[msgsize++] = 0x00;
  int c = crc_1021(newmsg, msgsize);
  newmsg[msgsize-2] = (c >> 8);
  newmsg[msgsize-1] = c & 0xff;

  // Build the final transmit message - escape various bytes
  byte txmsg[20] = { 0x80 };   // Prefix
  int txsize = 1;
  for (int i = 0; i < msgsize; i++) {
    if (newmsg[i] == 0x06 or newmsg[i] == 0x0d or newmsg[i] == 0x1b or newmsg[i] == 0x40 or newmsg[i] == 0x80) {
      txmsg[txsize++] = 0x1b;
      txmsg[txsize++] = newmsg[i] ^ 0xff;
    } else {
      txmsg[txsize++] = newmsg[i];
    }
  }
  txmsg[txsize++] = 0x0d;  // EOF

  // send to serial interface
  for (int x = 0; x < txsize; x++) {
    kamSer.write(txmsg[x]);
  }

}

/******************************/
/**                          **/
/**       KamReceive         **/
/**                          **/
/******************************/
// kamReceive - receive bytes from Kamstrup meter
unsigned short kamReceive(byte recvmsg[]) {

  byte rxdata[50];  // Buffer to hold received data
  unsigned long rxindex = 0;
  unsigned long starttime = millis();
  
  kamSer.flush();  // Flush serial buffer - might contain noise

  byte r;
  
  // Loop until EOL received or timeout
  while(r != 0x0d){
    
    // Handle rx timeout
    if(millis()-starttime > KAMTIMEOUT) {
      Serial.println("No response from Kamstrup unit");
      return 0;
    }

    // Handle incoming data
    if (kamSer.available()) {

      // Receive byte
      r = kamSer.read();
      if(r != 0x40) {  // Don't append if it's the start marker
        
        // Append data
        rxdata[rxindex] = r;
        rxindex++; 
      }

    }
  }

  // Remove escape markers from received data
  unsigned short j = 0;
  for (unsigned short i = 0; i < rxindex -1; i++) {
    if (rxdata[i] == 0x1b) {
      byte v = rxdata[i+1] ^ 0xff;
      if (v != 0x06 and v != 0x0d and v != 0x1b and v != 0x40 and v != 0x80){
        Serial.print("Missing escape ");
        Serial.println(v, HEX);
      }
      recvmsg[j] = v;
      i++; // skip
    } else {
      recvmsg[j] = rxdata[i];
    }
    j++;
  }
  
  // Check CRC
  if (crc_1021(recvmsg,j)) {
    Serial.println("CRC error: ");
    return 0;
  }

  return j;
}

/******************************/
/**                          **/
/**       KamDecode          **/
/**                          **/
/******************************/
// kamDecode - decodes received data
// float kamDecode(unsigned short const kreg, byte const *msg) {
long kamDecode(unsigned short const kreg, byte const *msg) {
  
  // Skip if message is not valid
  if (msg[0] != 0x3f or msg[1] != 0x10) {
    return false;
  }
  if (msg[2] != (kregnums[kreg] >> 8) or msg[3] != (kregnums[kreg] & 0xff)) {
    return false;
  }
    
  // Decode the significant
  long x = 0;
  for (int i = 0; i < msg[5]; i++) {
    x <<= 8;
    x |= msg[i + 7];
  }
  
  // Decode the exponent
  int i = msg[6] & 0x3f;
  if (msg[6] & 0x40) {
    i = -i;
  };
  float ifl = pow(10,i);
  if (msg[6] & 0x80) {
    ifl = -ifl;
  }
  
  // Return final value
  // return (float ) (x * ifl);
  return (long ) (x * ifl * 100);

}

/******************************/
/**                          **/
/**       crc_1021           **/
/**                          **/
/******************************/
// crc_1021 - calculate crc16
long crc_1021(byte const *inmsg, unsigned int len){
  long creg = 0x0000;
  for(unsigned int i = 0; i < len; i++) {
    int mask = 0x80;
    while(mask > 0) {
      creg <<= 1;
      if (inmsg[i] & mask){
        creg |= 1;
      }
      mask>>=1;
      if (creg & 0x10000) {
        creg &= 0xffff;
        creg ^= 0x1021;
      }
    }
  }
  return creg;
}

#ifndef Wireless
/**********************************/
/**                              **/
/**     printEthernetStatus      **/
/**                              **/
/**********************************/
void printEthernetStatus() {
   // Print the WiFi shield's IP address
   IPAddress ip = Ethernet.localIP();
   Serial.print("IP Address: ");
   Serial.println(ip);
}
#endif

#ifdef Wireless
/******************************/
/**                          **/
/**     printWifiStatus      **/
/**                          **/
/******************************/
void printWifiStatus() {
   // Print the SSID of the network
   Serial.print("SSID: ");
   Serial.println(WiFi.SSID());

   // Print the WiFi shield's IP address
   IPAddress ip = WiFi.localIP();
   Serial.print("IP Address: ");
   Serial.println(ip);

   // Print the received signal strength
   long rssi = WiFi.RSSI();
   Serial.print("Signal strength (RSSI):");
   Serial.print(rssi);
   Serial.println(" dBm");
}
#endif
