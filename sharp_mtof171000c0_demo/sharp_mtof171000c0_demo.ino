/////////////////////////////////////////////////////////////////////////////
// Sharp MTOF171000C0 TOF Sensor Module Demo (UART)
//
// Board Connection:
//   Arduino   MTOF171000C0
//   3.3V      Pin 1 - VDD
//   GND       Pin 2 - GND
//   10pin     Pin 3 - TXD   Use 3.3V logic level converter
//   11pin     Pin 4 - RXD   Use 3.3V logic level converter
//   NC        Pin 5
//   NC        Pin 6
//
// Serial monitor setting:
//   9600 baud
/////////////////////////////////////////////////////////////////////////////

#include <SoftwareSerial.h>

// Use software serial port for communicating with MTOF171000C0.
// If you have Arduino Mega 2560, you can use hardware serial port
// instead like Serial1, etc.
#define rxPin 10
#define txPin 11
static SoftwareSerial mySerial(rxPin, txPin);

// Starting bytes to identify UART data packet.
#define START_BYTE_1  0x55
#define START_BYTE_2  0xAA

// UART command codes.
#define CMD_RD_SEN_REG 0xD1   // Read the ToF sensor register
#define CMD_WR_SEN_REG 0x51   // Write the ToF sensor register
#define CMD_RD_MM      0xD3   // Measure distance
#define CMD_CALI_XTALK 0xFA   // Crosstalk calibration
#define CMD_CALI_OFS   0xFB   // Offset calibration
#define CMD_WR_OFS     0x7C   // Write the offset value
#define CMD_WR_XTALK   0x7D   // Write the crosstalk value
#define CMD_RD_VAR     0xF0   // Get the module status information, for the engineering debug
#define CMD_RESET      0xF5   // Sensor reset

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

// Helper function to send a basic command that doesn't require command data.
void sendCommand(byte cmdCode, byte dataLength) {
  // Send 1st Identification Byte.
  sendSerial(START_BYTE_1);

  // Send 2nd Identification Byte.
  sendSerial(START_BYTE_2);

  // Send the Command Instruction.
  sendSerial(cmdCode);

  // Send the Command Data.
  sendSerial(0);

  // Send the Data Length for this command in bytes.
  sendSerial(dataLength);

  // Send the CheckSum (which does not include the identification bytes).
  byte checksum = cmdCode + dataLength;
  sendSerial(checksum);
}

/////////////////////////////////////////////////////////////////////////////

// Helper function to send a write calibration command.
void sendCommandWriteCali(byte cmdCode, byte hiByte, byte loByte) {
  // Send 1st Identification Byte.
  sendSerial(START_BYTE_1);

  // Send 2nd Identification Byte.
  sendSerial(START_BYTE_2);

  // Send the Command Instruction.
  sendSerial(cmdCode);

  // Send the Command Data.
  sendSerial(0);

  // Send the Data Length for this command in bytes.
  sendSerial(2);

  // Send the hi byte of the calibration value to set.
  sendSerial(hiByte);

  // Send the lo byte of the calibration value to set.
  sendSerial(loByte);
  
  // Send the CheckSum (which does not include the identification bytes).
  byte checksum = cmdCode + 2 + hiByte + loByte;
  sendSerial(checksum);

  delay(500);
}

/////////////////////////////////////////////////////////////////////////////

// Helper function to read and print out the bytes of the specified command.
// Note the start bytes and cmd code have already been read.
void readCommand(byte cmdCode, byte dataLength) {
  // For computing the checksum.
  byte testsum = cmdCode;

  // Print out the command bytes read so far.
  Serial.print("Read Command: ");
  Serial.print(START_BYTE_1, HEX);
  Serial.print(" ");
  Serial.print(START_BYTE_2, HEX);
  Serial.print(" ");
  Serial.print(cmdCode, HEX);
  
  // Read the cmd data.
  int cmdData = readSerial();
  Serial.print(" ");
  Serial.print(cmdData, HEX);
  testsum += (byte)cmdData;
  
  // Read the Data Length.
  int len = readSerial();
  Serial.print(" ");
  Serial.print(len, HEX);
  if ( len != dataLength ) {
    Serial.println("");
    Serial.println("Error: Unexpected data length");
    return;
  }
  testsum += dataLength;
  
  // Read the Data.
  for(int i = 0; i < dataLength; i++) {
    int data = readSerial();
    Serial.print(" ");
    Serial.print(data, HEX);
    testsum += (byte)data;
  }
  
  // Read and verify the CheckSum.
  int checksum = readSerial();
  Serial.print(" ");
  Serial.println(checksum, HEX);
  if ( checksum != testsum ) {
    Serial.println("Error: Checksum does not match");
  }
}

/////////////////////////////////////////////////////////////////////////////

// Read the measured distance from the data packet.
// Note the start bytes and cmd code have already been read.
void readDistance() {
  // Read the cmd data (unused).
  int cmdData = readSerial();
  if ( cmdData != 0 )
    return;
    
  // Read the Data Length.
  int dataLength = readSerial();
  if ( dataLength != 2 )
    return;

  // Read the Hi byte of the distance.
  int distHi = readSerial();

  // Read the Lo byte of the distance.
  int distLo = readSerial();

  // Read and verify the CheckSum.
  int checksum = readSerial();
  unsigned int testsum = CMD_RD_MM + cmdData + dataLength + distHi + distLo;
  if ( checksum != (testsum & 0xFF) )
    return;
    
  // Print the distance to the Serial Monitor.
  int distMM = distHi * 256 + distLo;
  Serial.print("Read Distance: ");
  Serial.print(distMM);
  Serial.println("mm");
}

/////////////////////////////////////////////////////////////////////////////

// Read the UART data packet. 
void readDataPacket() {
  // Look for 1st Identification Byte.
  if ( readSerial() != START_BYTE_1 )
    return;

  // Look for 2nd Identification Byte.
  if ( readSerial() != START_BYTE_2 )
    return;

  // Look for the Command Code and process accordingly.
  int cmd = readSerial();
  if ( cmd == CMD_RD_MM ) {
    readDistance();
  } else if ( cmd == CMD_CALI_XTALK || cmd == CMD_CALI_OFS ) {
    readCommand(cmd, 4);
  }
}

/////////////////////////////////////////////////////////////////////////////

// Arduino setup function.
void setup() {
  // Start the hardware serial port for the serial monitor.
  Serial.begin(9600);
  Serial.println("");
  Serial.println("MTOF171000C0 Demo");
  Serial.println("=================");

  // Start the software serial port for communicating with MTOF171000C0.
  mySerial.begin(9600);

  // Wait two seconds for startup.
  delay(2000);

  // Test sending some commands.
  Serial.println("Send CMD_CALI_XTALK");
  sendCommand(CMD_CALI_XTALK, 4);
  Serial.println("Send CMD_CALI_OFS");
  sendCommand(CMD_CALI_OFS, 4);
  //Serial.println("Send CMD_RESET");
  //sendCommand(CMD_RESET, 0);
}

// Arduino main loop.
void loop() {
  Serial.println("---");
  
  // Send the request for a distance measurement.
  sendCommand(CMD_RD_MM, 2);

  // Read the data packet.
  readDataPacket();
  
} // END PROGRAM
