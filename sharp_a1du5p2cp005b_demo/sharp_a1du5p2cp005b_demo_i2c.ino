/////////////////////////////////////////////////////////////////////////////
// Sharp A1DU5P2CP005B VOCs Sensor Module Demo (I2C)
//
// Sensor IC: MNST iS-MM-VOCs-4
//
// Board Connection:
//   Arduino         A1DU5P2CP005B
//   3.3V            Pin 1 - VDD
//   SCL             Pin 2 - SCL   Use 3.3V logic level converter
//   SDA             Pin 3 - SDA   Use 3.3V logic level converter
//   Not Connected   Pin 4 - RX
//   Not Connected   Pin 5 - TX
//   GND             Pin 6 - GND
//   
// Reference: http://www.mnsenstech.com
/////////////////////////////////////////////////////////////////////////////

#include <Wire.h>

// Molecular weights for units conversion.
#define MOLECULAR_WEIGHT_HCHO 30
#define MOLECULAR_WEIGHT_TVOC 44

// Sensor I2C slave address (7-bit) = 81 decimal.
#define I2C_SLAVE_ADDRESS 0x51

// Sensor registers.
#define REGISTER_READ_DATA      0x61
#define REGISTER_RESET_BASELINE 0x52

// Counter used to decide when to process the data.
#define MAX_COUNTER 1000
static uint32_t counter = 0;

/////////////////////////////////////////////////////////////////////////////

// Decode status.
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

// Request sensor data.
void requestSensorData() {
  Wire.beginTransmission(I2C_SLAVE_ADDRESS);
  Wire.write(REGISTER_READ_DATA);
  Wire.write(0x02);
  Wire.write(0x01);
  Wire.endTransmission(); 
}

// Set the current measured value as the baseline.
void resetSensorBaseline() {
  Wire.beginTransmission(I2C_SLAVE_ADDRESS);
  Wire.write(REGISTER_RESET_BASELINE);
  Wire.write(0x01);
  Wire.write(0x01);
  Wire.endTransmission();  
}

// Convert ppb to ug/m3 units.
float convertUnits(uint32_t ppb, float molecularWeight) {
  return 0.0409 * ppb * molecularWeight;
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

  // Start the Wire library.
  Wire.begin();
  delay(500);

  // Request sensor data.
  requestSensorData();
}

// Arduino main loop.
void loop() {
  // Read 9 bytes of data from slave.
  int numRead = Wire.requestFrom(I2C_SLAVE_ADDRESS, 9);
  if (numRead != 9)
    return;
  uint8_t data[9];
  for(int i = 0; i < 9; i++) {
    data[i] = Wire.read();
  }

  // Check counter to see if we should process the data.
  if ( counter >= MAX_COUNTER ) {
    counter = 0;
  } else {
    counter++;
    return;
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
  printFValue("HCHO(ug/m3)", convertUnits(hcho, MOLECULAR_WEIGHT_HCHO));
  printFValue("TVOC(ug/m3)", convertUnits(tvoc, MOLECULAR_WEIGHT_TVOC), true);
  Serial.println("");
  Serial.println("---");
}

// END PROGRAM
