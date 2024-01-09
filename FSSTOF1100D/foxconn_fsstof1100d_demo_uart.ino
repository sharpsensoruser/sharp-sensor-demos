/////////////////////////////////////////////////////////////////////////////
// Foxconn FSSTOF1100D TOF Module Demo (UART) - range 1 to 10cm
//
// Board Connection:
//   Arduino   FSSTOF1100D
//   3.3V      Pin 1 - VCC
//   GND       Pin 2 - GND
//   10pin     Pin 5 - RXD   Use 3.3V logic level converter
//   11pin     Pin 6 - TXD   Use 3.3V logic level converter
//
// Serial monitor setting:
//   9600 baud 8-N-1
//
// References:
//   Online CRC-16 Modbus calculator: https://www.tahapaksu.com/crc/
//
// This demo code is also applicable to:
//   FSSTOF52000D - TOF Module with range 5cm to 200cm
//   FSSTOF54500D - TOF Module with range 5cm to 450cm
/////////////////////////////////////////////////////////////////////////////

#include <SoftwareSerial.h>

// Use software serial port for communicating with FSSTOF1100D.
// If you have Arduino Mega 2560, you can use hardware serial port
// instead like Serial1, etc.
#define rxPin 11
#define txPin 10
static SoftwareSerial mySerial(rxPin, txPin);

// UART data packet format:
// 1. Address bits (1 byte)
// 2. Function/command code (1 byte)
// 3. Data (e.g. 2 bytes for register id/address followed by 2 bytes for register data/value)
// 4. Checksum (CRC-16 Modbus, 2 bytes, low-order byte first)
#define ADDRESS_BITS 0x00

#define FUNCTION_CODE_READ_REGISTER  0x04
#define FUNCTION_CODE_WRITE_REGISTER 0x06

#define REGISTER_ID_DISTANCE 0x00
#define REGISTER_ID_STATUS   0x01
#define REGISTER_ID_RANGING  0x02
#define REGISTER_ID_VERSION  0x18

#define RANGING_START            0x01
#define RANGING_START_CRC16_LOW  0xE8
#define RANGING_START_CRC16_HIGH 0x1B

#define RANGING_STOP            0x00
#define RANGING_STOP_CRC16_LOW  0x29
#define RANGING_STOP_CRC16_HIGH 0xDB

#define DISTANCE_READ_NUMREGISTERS 0x02
#define DISTANCE_READ_CRC16_LOW    0x70
#define DISTANCE_READ_CRC16_HIGH   0x1A

#define VERSION_READ_NUMREGISTERS 0x02
#define VERSION_READ_CRC16_LOW    0xF0
#define VERSION_READ_CRC16_HIGH   0x1D

// Ranging status values (same as Sharp GP2AP03VT00F TOF Sensor).
#define RANGING_STATUS_VALID_DATA      0x00
#define RANGING_STATUS_VCSEL_SHORT     0x01
#define RANGING_STATUS_LOW_SIGNAL      0x02
#define RANGING_STATUS_LOW_SN          0x04
#define RANGING_STATUS_TOO_MUCH_AMB    0x08
#define RANGING_STATUS_WAF             0x10
#define RANGING_STATUS_CAL_ERROR       0x20  
#define RANGING_STATUS_CROSSTALK_ERROR 0x80

/////////////////////////////////////////////////////////////////////////////

// Helper function to send data through the software serial port.
void sendSerial(unsigned char data) {
  mySerial.write(data);
}

// Helper function to read data from software serial port.
int readSerial() {
  while (!mySerial.available()) {}
  return mySerial.read();
}

/////////////////////////////////////////////////////////////////////////////

// Helper function to read a UART data packet.
void readDataPacket(int* dataToFill, int numToRead) {
  Serial.print("Receive:");
  for(int i = 0; i < numToRead; i++) {
    int data = readSerial();
    Serial.print(" 0x");
    if ( data < 16 )
      Serial.print("0");
    Serial.print(data, HEX);
    dataToFill[i] = data;
  }
  Serial.println("");
}

/////////////////////////////////////////////////////////////////////////////

// Helper function to start ranging.
void startRanging() {
  Serial.println("---\nSend: Ranging start");
  
  // Send address bits (1 byte).
  sendSerial(ADDRESS_BITS);

  // Send function code (1 byte).
  sendSerial(FUNCTION_CODE_WRITE_REGISTER);

  // Send data (2 bytes for register id/address, high order byte is first).
  sendSerial(0x00);
  sendSerial(REGISTER_ID_RANGING);

  // Send data (2 bytes for register data/value, high order byte is first).
  sendSerial(0x00);
  sendSerial(RANGING_START);

  // Send checksum (2 bytes, CRC-16 Modbus, low order byte is first).
  sendSerial(RANGING_START_CRC16_LOW);
  sendSerial(RANGING_START_CRC16_HIGH);

  // Wait for processing.
  delay(100);
}

/////////////////////////////////////////////////////////////////////////////

// Helper function to stop ranging.
void stopRanging() {
  Serial.println("---\nSend: Ranging stop");
  
  // Send address bits (1 byte).
  sendSerial(ADDRESS_BITS);

  // Send function code (1 byte).
  sendSerial(FUNCTION_CODE_WRITE_REGISTER);

  // Send data (2 bytes for register id/address, high order byte is first).
  sendSerial(0x00);
  sendSerial(REGISTER_ID_RANGING);

  // Send data (2 bytes for register data/value, high order byte is first).
  sendSerial(0x00);
  sendSerial(RANGING_STOP);

  // Send checksum (2 bytes, CRC-16 Modbus, low order byte is first).
  sendSerial(RANGING_STOP_CRC16_LOW);
  sendSerial(RANGING_STOP_CRC16_HIGH);

  // Wait for processing.
  delay(100);
}

/////////////////////////////////////////////////////////////////////////////

// Helper function to read the distance result and the status.
void readDistance() {
  Serial.println("---\nSend: Distance read");
  
  // Send address bits (1 byte).
  sendSerial(ADDRESS_BITS);

  // Send function code (1 byte).
  sendSerial(FUNCTION_CODE_READ_REGISTER);

  // Send data (2 bytes for register id/address, high order byte is first).
  sendSerial(0x00);
  sendSerial(REGISTER_ID_DISTANCE);

  // Send data (2 bytes for number of register values to read, high order byte is first).
  sendSerial(0x00);
  sendSerial(DISTANCE_READ_NUMREGISTERS);

  // Send checksum (2 bytes, CRC-16 Modbus, low order byte is first).
  sendSerial(DISTANCE_READ_CRC16_LOW);
  sendSerial(DISTANCE_READ_CRC16_HIGH);

  // Wait for processing.
  delay(100);

  // Read response from module (9 bytes).
  int responseData[9];
  readDataPacket(responseData, 9);
  Serial.print("Distance=");
  Serial.print((long)(responseData[3])*256+responseData[4]);
  Serial.print("mm");
  Serial.print(", Status=0x");
  Serial.print((long)(responseData[5])*256+responseData[6], HEX);
  Serial.print(", CRC=0x");
  Serial.print((long)(responseData[8])*256+responseData[7], HEX);
  Serial.println("");
}

/////////////////////////////////////////////////////////////////////////////

// Helper function to read main and minor software version.
void readVersion() {
  Serial.println("---\nSend: Software version read");
  
  // Send address bits (1 byte).
  sendSerial(ADDRESS_BITS);

  // Send function code (1 byte).
  sendSerial(FUNCTION_CODE_READ_REGISTER);

  // Send data (2 bytes for register id/address, high order byte is first).
  sendSerial(0x00);
  sendSerial(REGISTER_ID_VERSION);

  // Send data (2 bytes for number of register values to read, high order byte is first).
  sendSerial(0x00);
  sendSerial(VERSION_READ_NUMREGISTERS);

  // Send checksum (2 bytes, CRC-16 Modbus, low order byte is first).
  sendSerial(VERSION_READ_CRC16_LOW);
  sendSerial(VERSION_READ_CRC16_HIGH);

  // Wait for processing.
  delay(100);

  // Read response from module (9 bytes).
  int responseData[9];
  readDataPacket(responseData, 9);
  Serial.print("Main version=");
  Serial.print((long)(responseData[3])*256+responseData[4]);
  Serial.print(", Minor version=");
  Serial.print((long)(responseData[5])*256+responseData[6]);
  Serial.print(", CRC=0x");
  Serial.print((long)(responseData[8])*256+responseData[7], HEX);
  Serial.println("");
}

/////////////////////////////////////////////////////////////////////////////

// Arduino setup function.
void setup() {
  // Start the hardware serial port for the serial monitor.
  Serial.begin(9600);
  Serial.println("");
  Serial.println("FSSTOF Module Demo");
  Serial.println("==================");
  
  // Start the software serial port for communicating with FSSTOF Module.
  mySerial.begin(9600);
  
  // Wait one second for startup.
  delay(1000);

  // Read version info.
  readVersion();
  
  // Start ranging.
  startRanging();
}

// Arduino main loop.
void loop() {

  // Request distance read and process response.
  readDistance();
  delay(1000);

} // END PROGRAM
