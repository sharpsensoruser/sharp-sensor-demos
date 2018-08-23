/////////////////////////////////////////////////////////////////////////////
// Sharp GP2Y1026AU0F Dust Sensor Demo
//
// Board Connection:
//   Arduino   GP2Y1026
//   GND       GND
//   5V        Vcc
//   10pin     TxD
//
// Serial monitor setting:
//   9600 baud
/////////////////////////////////////////////////////////////////////////////

#include <SoftwareSerial.h>

#define rxPin 10
#define txPin 11

// Use software serial port for communicating with GP2Y1026.
SoftwareSerial mySerial(rxPin, txPin);

// Helper function to read data from software serial port.
int getSerial() {
  while (!mySerial.available()) {}
  return mySerial.read();
}

void setup() {
  // Start the hardware serial port for the serial monitor.
  Serial.begin(9600);

  // Start the software serial port for receiving data from GP2Y1026.
  mySerial.begin(2400);

  // Wait a second for startup.
  delay(1000);
}

void loop() {
  static int frame[7];
  static int dataStart, VoutH, VoutL, VrefH, VrefL, checksum, dataEnd;

  // Look for end of frame.
  if ( getSerial() != 0xff )
    return;

  // Read in next frame.
  for(int i = 0; i < 7; i++) {
    frame[i] = getSerial();
  }

  // Assign values.
  dataStart = frame[0];
  VoutH = frame[1];
  VoutL = frame[2];
  VrefH = frame[3];
  VrefL = frame[4];
  checksum = frame[5];
  dataEnd = frame[6];
  
  // Check the start and end of frame.
  if ( dataStart != 0xaa || dataEnd != 0xff )
    return;
  
  // Verify the checksum.
  unsigned int testSum = VoutH + VoutL + VrefH + VrefL;
  if ( checksum != (testSum & 0xff) )
    return;

  // Print the data values.
  Serial.print("dataStart=");
  Serial.print(dataStart);
  Serial.print(", ");
  Serial.print("VoutH=");
  Serial.print(VoutH);
  Serial.print(", ");
  Serial.print("VoutL=");
  Serial.print(VoutL);
  Serial.print(", ");
  Serial.print("VrefH=");
  Serial.print(VrefH);
  Serial.print(", ");
  Serial.print("VrefL=");
  Serial.print(VrefL);
  Serial.print(", ");
  Serial.print("Checksum=");
  Serial.print(checksum);
  Serial.print(", ");
  Serial.print("dataEnd=");
  Serial.print(dataEnd);
  Serial.print(", ");
  
  // Calculate Vout.
  float Vout = (VoutH * 256 + VoutL) / 1024.0 * 5.0;
  Serial.print("Vout=");
  Serial.print(Vout * 1000.0);
  Serial.print("mV, ");

  // Determine "a" coefficient beforehand by plotting output voltage
  // on Y axis versus dust density on X axis which you measure using
  // a reference dust monitor. The coefficient will be different
  // depending on the reference dust monitor used and the type of
  // particulate matter you are testing with. By default, "a" is based
  // on sensitivity of 0.35V per change in dust density of 100ug/m3.
  float a = 100.0 / 0.35;
  
  // Calculate dust density.
  float dustDensity = a * Vout;
  Serial.print("dustDensity=");
  Serial.print(dustDensity);
  Serial.print("ug/m3");
  Serial.println(" ");  
}

// END PROGRAM

