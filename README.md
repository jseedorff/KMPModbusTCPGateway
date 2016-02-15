# ModbusTCPClient
Arduino Modbus TCP / Kamstrup 602 KMP gateway. The gateway operates as a Modbus TCP client that continously reads a number of select registers from the Kamstrup 602 using the KMP protocol. These registered are then made available according to the included Modbus memory map.
A switch (#define Wireless) exist in the code to compile for the Arduino Uno+Arduino WiFi shield or the Arduino Ethernet.
This project makes use of the KMPSoftwareSerial and Mudbus libraries (present in the Library repository).
The physical interface between the Kamstrup KMP and the Arduino I/Os is secribed in the file "interface.pdf". 

