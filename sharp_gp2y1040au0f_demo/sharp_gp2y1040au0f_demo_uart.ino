/////////////////////////////////////////////////////////////////////////////
// Sharp GP2Y1040AU0F Dust Sensor Demo (UART)
//
// Board Connection:
//   Arduino       GP2Y1040AU0F
//   10pin         Pin 1 - TXD
//   Not Connected Pin 2 - RXD
//   Not Connected Pin 3 - SEL
//   GND           Pin 4 - GND
//   5V            Pin 5 - VCC
//   GND           Pin 6 - GND
//
// Serial monitor setting:
//   9600 baud 8-N-1
/////////////////////////////////////////////////////////////////////////////

#include <SoftwareSerial.h>

// Use software serial port for communicating with GP2Y1040AU0F.
// If you have Arduino Mega 2560, you can use hardware serial port
// instead like Serial1, etc.
#define rxPin 10
#define txPin 11
static SoftwareSerial mySerial(rxPin, txPin);

// Starting bytes of UART data record.
#define StartFrame1 0xFF
#define StartFrame2 0xFA

// Store UART data record raw bytes and calculated field values.
static uint8_t frames[28];
static int NC_03;  // Number concentration of particles with size 0.3um to 10um
static int NC_05;  // Number concentration of particles with size 0.5um to 10um
static int NC_1;   // Number concentration of particles with size 1.0um to 10um 
static int NC_25;  // Number concentration of particles with size 2.5um to 10um
static int NC_4;   // Number concentration of particles with size 4.0um to 10um
static int PM1_1;  // Mass concentration of particles with size 0.3um to 1.0um (standard particle)
static int PM25_1; // Mass concentration of particles with size 0.3um to 2.5um (standard particle)
static int PM10_1; // Mass concentration of particles with size 0.3um to 10um (standard particle)
static int PM1_2;  // Mass concentration of particles with size 0.3um to 1.0um (cigarette smoke)
static int PM25_2; // Mass concentration of particles with size 0.3um to 2.5um (cigarette smoke)
static int PM10_2; // Mass concentration of particles with size 0.3um to 10um (cigarette smoke)

/////////////////////////////////////////////////////////////////////////////

// Helper function to read data from software serial port.
int readSerial() {
  while (!mySerial.available()) {}
  return mySerial.read();
}

// Helper function to print a data value to the serial monitor.
void printValue(String text, int value, bool isLast = false) {
  Serial.print(text);
  Serial.print("=");
  Serial.print(value);
  if (!isLast) {
    Serial.print(", ");
  }
}

// Read the 28-byte UART data record.
void readDataRecord() {
  // Look for the 1st start frame.  
  frames[0] = readSerial();
  if ( frames[0] != StartFrame1 )
    return;
  
  // Look for the 2nd start frame.
  frames[1] = readSerial();
  if ( frames[1] != StartFrame2 )
    return;
    
  // Read the next 26 frames/bytes.
  for(int i = 2; i < 28; i++) {
    frames[i] = readSerial();    
  }

  // Print raw data frames.
  Serial.print("Raw data 00 to 13: 0x");
  Serial.print(frames[0], HEX);
  for(int i = 1; i < 14; i++) {
    Serial.print(", 0x");
    Serial.print(frames[i], HEX);
  }
  Serial.println("");
  Serial.print("Raw data 14 to 27: 0x");
  Serial.print(frames[14], HEX);
  for(int i = 15; i < 28; i++) {
    Serial.print(", 0x");
    Serial.print(frames[i], HEX);
  }
  Serial.println("");
  
  // Compare the checksum.
  uint8_t checksum = 0;
  for(int i = 0; i < 27; i++) {
    checksum += frames[i];
  }
  if ( checksum != frames[27] ) {
    Serial.print("Checksum doesn't match: calculated sum is: 0x");
    Serial.println(checksum, HEX);
    return;
  }
  
  // Calculate the number and mass concentrations from the raw frame data.
  NC_03  = frames[2]  * 256 + frames[3];
  NC_05  = frames[4]  * 256 + frames[5];
  NC_1   = frames[6]  * 256 + frames[7];
  NC_25  = frames[8]  * 256 + frames[9];
  NC_4   = frames[10] * 256 + frames[11];
  PM1_1  = frames[14] * 256 + frames[15];
  PM25_1 = frames[16] * 256 + frames[17];
  PM10_1 = frames[18] * 256 + frames[19];
  PM1_2  = frames[20] * 256 + frames[21];
  PM25_2 = frames[22] * 256 + frames[23];
  PM10_2 = frames[24] * 256 + frames[25];  

  // Print concentration values.
  printValue("NC_03",  NC_03);
  printValue("NC_05",  NC_05);
  printValue("NC_1",   NC_1);
  printValue("NC_25",  NC_25);
  printValue("NC_4",   NC_4, true);
  Serial.println("");
  printValue("PM1_1",  PM1_1);
  printValue("PM25_1", PM25_1);
  printValue("PM10_1", PM10_1);
  printValue("PM1_2",  PM1_2);
  printValue("PM25_2", PM25_2);
  printValue("PM10_2", PM10_2, true);
  Serial.println("");
}

/////////////////////////////////////////////////////////////////////////////

// Arduino setup function.
void setup() {
  // Start the hardware serial port for the serial monitor.
  Serial.begin(9600);
  Serial.println("");
  Serial.println("GP2Y1040AU0F Demo");
  Serial.println("=================");

  // Start the software serial port for receiving data from GP2Y1040AU0F.
  mySerial.begin(9600);

  // Wait a second for startup.
  delay(1000);
}

// Arduino main loop.
void loop() {
  // Read data record.
  readDataRecord();
  Serial.println("---");
  delay(100);
  
} // END PROGRAM
