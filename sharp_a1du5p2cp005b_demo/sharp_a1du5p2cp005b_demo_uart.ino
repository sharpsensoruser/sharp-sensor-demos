/////////////////////////////////////////////////////////////////////////////
// Sharp A1DU5P2CP005B VOCs Sensor Module Demo (UART)
//
// Sensor IC: MNST iS-MM-VOCs-4
//
// Board Connection:
//   Arduino         A1DU5P2CP005B
//   3.3V            Pin 1 - VDD
//   Not Connected   Pin 2 - SCL
//   Not Connected   Pin 3 - SDA
//   Pin 11          Pin 4 - RX
//   Pin 10          Pin 5 - TX
//   GND             Pin 6 - GND
//   
// Reference: http://www.mnsenstech.com
/////////////////////////////////////////////////////////////////////////////

#include <SoftwareSerial.h>

// Use software serial port for communicating with sensor module.
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

// Request sensor data by sending UART command.
void requestSensorData() {
  sendSerial(0x61);
  sendSerial(0x02);
  sendSerial(0x01);
}

// Set the current measured value as the baseline.
void resetSensorBaseline() {
  sendSerial(0x52);
  sendSerial(0x01);
  sendSerial(0x01);
}

/////////////////////////////////////////////////////////////////////////////

// Decode sensor status.
String decodeStatus(uint32_t stat) {
  String statusText = "Unknown";
  if ( stat == 0x00 )
    statusText = "0(OK)";
  else if ( stat == 0x01 )
    statusText = "1(Heating)";
  else if ( stat == 0x02 )
    statusText = "2(Error)";
  return statusText;  
}

// Helper function to print a data value to the serial monitor.
void printValue(String text, uint32_t value, bool isLast = false) {
  Serial.print(text);
  Serial.print("=");
  Serial.print(value);
  if (!isLast) {
    Serial.print(", ");
  }
}
void printFValue(String text, float value, bool isLast = false) {
  Serial.print(text);
  Serial.print("=");
  Serial.print(value);
  if (!isLast) {
    Serial.print(", ");
  }
}

/////////////////////////////////////////////////////////////////////////////

// Arduino setup function.
void setup() {
  // Start the hardware serial port for the serial monitor.
  Serial.begin(9600);
  Serial.println("");
  Serial.println("A1DU5P2CP005B Demo");
  Serial.println("==================");

  // Start the software serial port for communicating with the module.
  mySerial.begin(9600);

  // Wait for startup.
  delay(1000);
}

// Arduino main loop.
void loop() {
  // Make request for sensor data.
  requestSensorData();
  
  // Read 9 bytes.
  int data[9];
  for(int i = 0; i < 9; i++) {
    int val = readSerial();
    if ( val < 0 )
      return;
    data[i] = val; 
  }

  // Print out raw data.
  Serial.print("RAW DATA: ");
  for(int j = 0; j < 9; j++) {
    Serial.print(data[j]);
    Serial.print(" ");
  }
  Serial.println("");

  // Calculate concentration values (ppb - parts per billion).
  uint32_t hcho = data[0]*256UL + data[1];
  uint32_t stat = data[2];
  uint32_t sens = data[3]*16777216UL + data[4]*65536UL + data[5]*256UL + data[6];
  uint32_t tvoc = data[7]*256UL + data[8];
  printValue("HCHO(ppb)", hcho);
  Serial.print("Status=" + decodeStatus(stat) + ", ");
  printValue("Sensing Resistor", sens);
  printValue("TVOC(ppb)", tvoc, true);
  Serial.println("");

  // Convert to different concentration units (ug/m3).
  // For this sensor model, 1 ppb is equal to 1.8 ug/m3.
  printFValue("HCHO(ug/m3)", hcho*1.8f);
  printFValue("TVOC(ug/m3)", tvoc*1.8f, true);
  Serial.println("");
  Serial.println("---");

  delay(1000);
}

// END PROGRAM
