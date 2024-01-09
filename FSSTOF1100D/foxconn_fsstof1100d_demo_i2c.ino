/////////////////////////////////////////////////////////////////////////////
// Foxconn FSSTOF1100D TOF Module Demo (I2C) - range 1cm to 10cm
//
// Board Connection:
//   Arduino   FSSTOF1100D
//   3.3V      Pin 1 - VCC
//   GND       Pin 2 - GND
//   SCL       Pin 3 - SCL   Use 3.3V logic level converter
//   SDA       Pin 4 - SDA   Use 3.3V logic level converter
//
// This demo code is also applicable to:
//   FSSTOF52000D - TOF Module with range 5cm to 200cm
//   FSSTOF54500D - TOF Module with range 5cm to 450cm
/////////////////////////////////////////////////////////////////////////////

#include <Wire.h>

// TOF Module I2C slave address = 16 decimal.
#define SLAVE_ADDRESS 0x10 

// I2C register IDs/command codes.
#define REGISTER_ID_DISTANCE 0x00
#define REGISTER_ID_STATUS   0x01
#define REGISTER_ID_RANGING  0x02
#define REGISTER_ID_VERSION  0x18

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

// Helper function to write a value to an 8-bit register.
void writeRegister(uint8_t regAddress, uint8_t value) {
  Wire.beginTransmission(SLAVE_ADDRESS);
  Wire.write(regAddress);
  Wire.write(value);
  Wire.endTransmission();

  delay(200);
}

/////////////////////////////////////////////////////////////////////////////

// Helper function to read main and minor software version.
void readVersion()
{
  // Specify the register address to read from.
  Wire.beginTransmission(SLAVE_ADDRESS);
  Wire.write(REGISTER_ID_VERSION);
  Wire.endTransmission();

  // Request to read 4 bytes.
  Wire.requestFrom(SLAVE_ADDRESS, 4);
  uint8_t mainVerLo  = Wire.read();
  uint8_t mainVerHi  = Wire.read();
  uint8_t minorVerLo = Wire.read();
  uint8_t minorVerHi = Wire.read();

  // Print the main and minor version.
  Serial.print("Main version=");
  Serial.print((long)mainVerHi*256 + mainVerLo);
  Serial.print(", Minor version=");
  Serial.print((long)minorVerHi*256 + minorVerLo);
  Serial.println("");
}

/////////////////////////////////////////////////////////////////////////////

// Helper function to start ranging.
void startRanging()
{
  uint8_t valueHi = 0x00;
  uint8_t valueLo = 0x01;

  // Write the 16-bit value of "1" to start ranging.
  Serial.println("---\nStart ranging");
  Wire.beginTransmission(SLAVE_ADDRESS);
  Wire.write(REGISTER_ID_RANGING);
  Wire.write(valueLo);
  Wire.write(valueHi);
  Wire.endTransmission();
}

// Helper function to stop ranging.
void stopRanging()
{
  uint8_t valueHi = 0x00;
  uint8_t valueLo = 0x00;

  // Write the 16-bit value of "0" to stop ranging.
  Serial.println("---\nStop ranging");
  Wire.beginTransmission(SLAVE_ADDRESS);
  Wire.write(REGISTER_ID_RANGING);
  Wire.write(valueLo);
  Wire.write(valueHi);
  Wire.endTransmission();
}

/////////////////////////////////////////////////////////////////////////////

// Helper function to read the distance and ranging status.
void readDistance()
{
  // Specify the register address to read from.
  Wire.beginTransmission(SLAVE_ADDRESS);
  Wire.write(REGISTER_ID_DISTANCE);
  Wire.endTransmission();

  // Request to read 4 bytes.
  Wire.requestFrom(SLAVE_ADDRESS, 4);
  uint8_t distLo   = Wire.read();
  uint8_t distHi   = Wire.read();
  uint8_t statusLo = Wire.read();
  uint8_t statusHi = Wire.read();

  // Print the distance and ranging status.
  Serial.print("---\nDistance=");
  Serial.print((long)distHi*256 + distLo);
  Serial.print("mm, Status=0x");
  Serial.print((long)statusHi*256 + statusLo, HEX);
  Serial.println("");
}

/////////////////////////////////////////////////////////////////////////////

// Arduino setup function.
void setup()
{
  // Start the hardware serial port for the serial monitor.
  Serial.begin(9600);
  Serial.println("");
  Serial.println("FSSTOF Module Demo");
  Serial.println("==================");
  
   // Start the Wire library.
  Wire.begin();
  
  // Wait one second for startup.
  delay(1000);

  // Read version info.
  readVersion();
  
  // Start ranging.
  startRanging();
}

// Arduino main loop.
void loop()
{  
  // Read distance and ranging status.
  readDistance();
  delay(1000);

} // END PROGRAM
