/////////////////////////////////////////////////////////////////////////////
// Foxconn FSTOF2003C0I TOF Module Demo (I2C)
//
// Board Connection:
//   Arduino   FSTOF2003C0I
//   3.3V      Pin 1 - VDD
//   GND       Pin 2 - GND
//   SDA       Pin 3 - SDA
//   SCL       Pin 4 - SCL
/////////////////////////////////////////////////////////////////////////////

#include <Wire.h>

// TOF Module I2C slave address = 54 decimal.
#define SLAVE_ADDRESS 0x36

// I2C command codes.
#define TOFM_CMD_NONE            0x00
#define TOFM_CMD_START_FLAG      0x80
#define TOFM_CMD_ST_MM           0x81
#define TOFM_CMD_CALI_XTALK      0x82
#define TOFM_CMD_CALI_OFS        0x83
#define TOFM_CMD_RESET           0x84
#define TOFM_CMD_RD_FACTORY_DATA 0x85
#define TOFM_CMD_RD_VERSION_INFO 0x86
#define TOFM_CMD_RD_DEBUG_PARA1  0x8A
#define TOFM_CMD_RD_DEBUG_PARA2  0x8B
#define TOFM_CMD_RD_DEBUG_PARA3  0x8C

// Range1 status values (same as Sharp GP2AP03VT00F TOF Sensor).
#define RANGE1_STATUS_VALID_DATA      0x00
#define RANGE1_STATUS_VCSEL_SHORT     0x01
#define RANGE1_STATUS_LOW_SIGNAL      0x02
#define RANGE1_STATUS_LOW_SN          0x04
#define RANGE1_STATUS_TOO_MUCH_AMB    0x08
#define RANGE1_STATUS_WAF             0x10
#define RANGE1_STATUS_CAL_ERROR       0x20  
#define RANGE1_STATUS_CROSSTALK_ERROR 0x80

/////////////////////////////////////////////////////////////////////////////

// Helper function to write a value to an 8-bit register.
void writeRegister(uint8_t regAddress, uint8_t value)
{
  Wire.beginTransmission(SLAVE_ADDRESS);
  Wire.write(regAddress);
  Wire.write(value);
  Wire.endTransmission();

  delay(200);
}

/////////////////////////////////////////////////////////////////////////////

// Helper function to request distance measurement.
void send_CMD_ST_MM() {
  uint8_t data = 0;
  writeRegister(TOFM_CMD_ST_MM, data);
}

// Read the measured distance from the data packet (range1).
void readDistance() {
  // Specify the register address to read from.
  Wire.beginTransmission(SLAVE_ADDRESS);
  Wire.write(TOFM_CMD_ST_MM);
  Wire.endTransmission();

  // Request to read 5 bytes.
  Wire.requestFrom(SLAVE_ADDRESS, 5);
  uint8_t cmdCode      = Wire.read();
  uint8_t dataLen      = Wire.read();
  uint8_t distHi       = Wire.read();
  uint8_t distLo       = Wire.read();
  uint8_t range1Status = Wire.read();

  // Print the raw data to the Serial Monitor.
  Serial.print("Raw Data: 0x");
  Serial.print(cmdCode, HEX);
  Serial.print(" 0x");
  Serial.print(dataLen, HEX);
  Serial.print(" 0x");
  Serial.print(distHi, HEX);
  Serial.print(" 0x");
  Serial.print(distLo, HEX);
  Serial.print(" 0x");
  Serial.print(range1Status, HEX);
  Serial.print(", ");
  
  // Print the distance to the Serial Monitor.
  int distMM = distHi * 256 + distLo;
  Serial.print("Distance = ");
  Serial.print(distMM);
  Serial.println("mm");
}

/////////////////////////////////////////////////////////////////////////////

// Helper function to request version info.
void send_CMD_RD_VERSION_INFO() {
  uint8_t data = 0;
  writeRegister(TOFM_CMD_RD_VERSION_INFO, data);
}

// Read the version info from the data packet.
void readVersionInfo() {
  // Specify the register address to read from.
  Wire.beginTransmission(SLAVE_ADDRESS);
  Wire.write(TOFM_CMD_RD_VERSION_INFO);
  Wire.endTransmission();

  // Request to read 5 bytes.
  Wire.requestFrom(SLAVE_ADDRESS, 5);
  uint8_t cmdCode   = Wire.read();
  uint8_t dataLen   = Wire.read();
  uint8_t sensor_ic = Wire.read();
  uint8_t port      = Wire.read();
  uint8_t vers      = Wire.read();

  // Print the raw data to the Serial Monitor.
  Serial.print("Raw Data: 0x");
  Serial.print(cmdCode, HEX);
  Serial.print(" 0x");
  Serial.print(dataLen, HEX);
  Serial.print(" 0x");
  Serial.print(sensor_ic, HEX);
  Serial.print(" 0x");
  Serial.print(port, HEX);
  Serial.print(" 0x");
  Serial.println(vers, HEX);

  // Decode the sensor_ic type.
  if ( sensor_ic == 0x02 ) {
    Serial.println("sensor_ic = 0x02 (GP2AP02VT00F)");
  } else if ( sensor_ic == 0x03 ) {
    Serial.println("sensor_ic = 0x03 (GP2AP03VT00F)");
  } else {
    Serial.print("sensor_ic = 0x");
    Serial.println(sensor_ic, HEX);    
  }
  
  // Decode the port type.
  if ( port == 0x41 ) {
    Serial.println("port = 0x41('A') : Firmware supports UART and I2C");
  } else if ( port == 0x49 ) {
    Serial.println("port = 0x49('I') : Firmware supports I2C only");
  } else if ( port == 0x55 ) {
    Serial.println("port = 0x55('U') : Firmware supports UART only");    
  } else {
    Serial.print("port = 0x");
    Serial.println(port, HEX);    
  }

  // Output the version number.
  Serial.print("version = ");
  Serial.println(vers);
}

/////////////////////////////////////////////////////////////////////////////

// Arduino setup function.
void setup() {
  // Start the hardware serial port for the serial monitor.
  Serial.begin(9600);
  Serial.println("");
  Serial.println("FSTOF2003C0I Demo");
  Serial.println("=================");
  
  // Wait one second for startup.
  delay(1000);

  // Get version info.
  send_CMD_RD_VERSION_INFO();
  readVersionInfo();
}

// Arduino main loop.
void loop() {
  // Send the request for a distance measurement.
  send_CMD_ST_MM();
  
  // Read the distance.
  readDistance();

} // END PROGRAM
