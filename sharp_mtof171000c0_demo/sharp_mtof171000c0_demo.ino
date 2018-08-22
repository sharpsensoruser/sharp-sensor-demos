/////////////////////////////////////////////////////////////////////////////
// Sharp MTOF171000C0 TOF Sensor Module Demo
//
// Board Connection:
//   Arduino   MTOF171000C0
//   3.3V      VDD
//   GND       GND
//   10pin     TXD
//   11pin     RXD
//
// Serial monitor setting:
//   9600 baud
/////////////////////////////////////////////////////////////////////////////

#include <SoftwareSerial.h>

// Flag to indicate whether to send a command to TOF module.
static bool doSend = true;

// Use software serial port for communicating with MTOF171000C0.
// If you have Arduino Mega 2560, you can use hardware serial port
// instead like Serial1, etc.
#define rxPin 10
#define txPin 11
static SoftwareSerial mySerial(rxPin, txPin);

// Helper function to send data through the software serial port.
void sendSerial(unsigned char data) {
  mySerial.write(data);
}

// Helper function to read data from software serial port.
int readSerial() {
  while (!mySerial.available()) {}
  return mySerial.read();
}

// Helper function to send the command CMD_RD_MM.
void send_CMD_RD_MM() {
  // Send 1st Identification Byte.
  sendSerial(0x55);

  // Send 2nd Identification Byte.
  sendSerial(0xAA);

  // Send the Command Instruction for CMD_RD_MM.
  sendSerial(0xD3);

  // Send Data Register 0.
  sendSerial(0);

  // Send the Data Length for this command in bytes.
  sendSerial(0x02);

  // Send the CheckSum (which does not include the identification bytes).
  sendSerial(0xD5);
}

/////////////////////////////////////////////////////////////////////////////

// Arduino setup function.
void setup() {
  // Start the hardware serial port for the serial monitor.
  Serial.begin(9600);

  // Start the software serial port for communicating with MTOF171000C0.
  mySerial.begin(9600);

  // Wait two seconds for startup.
  delay(2000);
}

// Arduino main loop.
void loop() {
  // Send the request for a distance measurement.
  if ( doSend ) {
    send_CMD_RD_MM();
    doSend = false;
  }
  
  // Look for 1st Identification Byte.
  if ( readSerial() != 0x55 )
    return;

  // Look for 2nd Identification Byte.
  if ( readSerial() != 0xAA )
    return;

  // Look for Command Instruction.
  int cmd = 0xD3;
  if ( readSerial() != cmd )
    return;

  // Read the data register (unused).
  int dataReg = readSerial();
  if ( dataReg != 0 )
    return;
    
  // Read the Data Length.
  int dataLength = readSerial();
  if ( dataLength != 0x02 )
    return;

  // Read the Hi byte of the distance.
  int distHi = readSerial();

  // Read the Lo byte of the distance.
  int distLo = readSerial();

  // Read and verify the CheckSum.
  int checksum = readSerial();
  unsigned int testsum = cmd + dataReg + dataLength + distHi + distLo;
  if ( checksum != (testsum & 0xff) )
    return;
    
  // Print the distance to the Serial Monitor.
  int distMM = distHi * 256 + distLo;
  Serial.print("Distance = ");
  Serial.print(distMM);
  Serial.println("mm");
  doSend = true;
  
} // END PROGRAM

