/////////////////////////////////////////////////////////////////////////////
// Sharp GP2AP02VT00F Time-of-Flight Sensor Demo (I2C)
//
// Board Connection:
//   Arduino         GP2AP02VT00F
//   Not Connected   Pin 01 - NC
//   Not Connected   Pin 02 - NC
//   Not Connected   Pin 03 - NC
//                   Pin 04 - INT
//   SCL             Pin 05 - SCL   Use 3.3V logic level converter
//   SDA             Pin 06 - SDA   Use 3.3V logic level converter
//   3.3V            Pin 07 - VDD
//   GND             Pin 08 - GND
//                   Pin 09 - EN
//                   Pin 10 - GPIO
//   GND             Pin 11 - GNDV
//   3.3V            Pin 12 - VDDV
//
/////////////////////////////////////////////////////////////////////////////

#include <Wire.h>

// Sensor I2C slave address (7-bit) = 41 decimal.
#define I2C_SLAVE_ADDRESS 0x29

// Device/Chip ID = 47 decimal.
#define CHIP_ID 0x2f

// Sensor registers.
#define REGISTER_OPERATING_MODE     0x00
#define REGISTER_MEASUREMENT_STATUS 0x01
#define REGISTER_SOFTWARE_RESET     0x02
#define REGISTER_INT_TERMINAL       0x02
#define REGISTER_VCSEL_CURRENT      0x04
#define REGISTER_MEASUREMENT_TIME   0x2B
#define REGISTER_RANGE1_STATUS      0x2E
#define REGISTER_MEASUREMENT_DATA   0x38
#define REGISTER_DEVICE_ID          0x41

// Operating mode.
#define OPERATING_MODE_SHUTDOWN          0
#define OPERATING_MODE_START_MEASUREMENT 0x80

// Measurement status.
#define MEASUREMENT_STATUS_NOT_END       0
#define MEASUREMENT_STATUS_ENDED         1
#define BITMASK_MEASUREMENT_STATUS_FLAG  0x02
#define BITMASK_MEASUREMENT_STATUS_CLEAR 0x0d

// Software reset.
#define SOFTWARE_RESET_VALUE 0x01

// INT terminal.
#define INT_TERMINAL_VALUE 0x20

// VCSEL current.
#define VCSEL_CURRENT_SHORT_RANGE 0x07
#define VCSEL_CURRENT_LONG_RANGE  0x0F

// Measurement time.
#define MEASUREMENT_TIME_NORMAL 0x03

// Range1 status values.
#define RANGE1_STATUS_VALID_DATA      0x00
#define RANGE1_STATUS_VCSEL_SHORT     0x01
#define RANGE1_STATUS_LOW_SIGNAL      0x02
#define RANGE1_STATUS_LOW_SN          0x04
#define RANGE1_STATUS_TOO_MUCH_AMB    0x08
#define RANGE1_STATUS_WAF             0x10
#define RANGE1_STATUS_CAL_ERROR       0x20  
#define RANGE1_STATUS_CROSSTALK_ERROR 0x80

// Register values.
#define REGISTER_06H_BASE_VALUE  0x60
#define REGISTER_14H_SHORT_RANGE 0x28
#define REGISTER_14H_LONG_RANGE  0x14
#define REGISTER_43H_SHORT_RANGE 0xC0
#define REGISTER_43H_LONG_RANGE  0xD0
#define REGISTER_47H_SHORT_RANGE 0x20
#define REGISTER_47H_LONG_RANGE  0x00

/////////////////////////////////////////////////////////////////////////////

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

// Helper function to read from I2C.
void readi2c(uint8_t registerAddress, uint8_t* data, int dataLen) {
  Wire.beginTransmission(I2C_SLAVE_ADDRESS);
  Wire.write(registerAddress);
  Wire.endTransmission();

  Wire.requestFrom(I2C_SLAVE_ADDRESS, dataLen);
  for(int i = 0; i < dataLen; i++) {
    data[i] = Wire.read();
  }
}

// Helper function to write to I2C.
void writei2c(uint8_t registerAddress, uint8_t* data, int dataLen) {
  Wire.beginTransmission(I2C_SLAVE_ADDRESS);
  Wire.write(registerAddress);
  for(int i = 0; i < dataLen; i++) {
    Wire.write(data[i]);
  }
  Wire.endTransmission(); 
}

/////////////////////////////////////////////////////////////////////////////

// Get single register value.
uint8_t getRegisterSingle(uint8_t registerAddress) {
  uint8_t data[1];
  data[0] = 0;
  readi2c(registerAddress, data, 1);
  return data[0];
}

// Set single register value.
void setRegisterSingle(uint8_t registerAddress, uint8_t registerValue) {
  uint8_t data[1];
  data[0] = registerValue;
  writei2c(registerAddress, data, 1);
}

// Set and print register value.
void setRegisterAndPrint(uint8_t registerAddress, uint8_t registerValue, String registerText) {
  setRegisterSingle(registerAddress, registerValue);
  uint8_t data = getRegisterSingle(registerAddress);
  printValue(registerText, data, true);
  Serial.println("");
}

/////////////////////////////////////////////////////////////////////////////

// Enter shutdown mode.
void shutdownMode() {
  setRegisterSingle(REGISTER_OPERATING_MODE, OPERATING_MODE_SHUTDOWN);
}

// Start measurement mode.
void startMeasurement() {
  setRegisterSingle(REGISTER_OPERATING_MODE, OPERATING_MODE_START_MEASUREMENT);
  delay(500);
}

// Do a software reset of tof sensor.
void softwareReset() {
  setRegisterSingle(REGISTER_SOFTWARE_RESET, SOFTWARE_RESET_VALUE);
  setRegisterSingle(REGISTER_OPERATING_MODE, OPERATING_MODE_START_MEASUREMENT);
  delay(2);
  setRegisterSingle(REGISTER_OPERATING_MODE, OPERATING_MODE_SHUTDOWN);
  setRegisterSingle(REGISTER_RANGE1_STATUS, RANGE1_STATUS_VALID_DATA);
}

// Check if measurement has ended or not.
uint8_t getMeasurementEndFlag() {
  uint8_t data = getRegisterSingle(REGISTER_MEASUREMENT_STATUS);
  if ( data & BITMASK_MEASUREMENT_STATUS_FLAG == BITMASK_MEASUREMENT_STATUS_FLAG ) {
    return MEASUREMENT_STATUS_ENDED;
  }
  return MEASUREMENT_STATUS_NOT_END;
}

// Clear measurement status.
void clearMeasurementEndFlag() {
    setRegisterSingle(REGISTER_MEASUREMENT_STATUS, BITMASK_MEASUREMENT_STATUS_CLEAR);
}

// Get the Device ID.
uint8_t getDeviceID() {
  uint8_t data = getRegisterSingle(REGISTER_DEVICE_ID);
  return data;
}

// Initialize registers.
void initRegisters() {
  setRegisterAndPrint(REGISTER_MEASUREMENT_TIME, MEASUREMENT_TIME_NORMAL, "Measurement Time");
  setRegisterAndPrint(REGISTER_INT_TERMINAL, INT_TERMINAL_VALUE, "INT Terminal"); // 52 decimal.
  setRegisterAndPrint(0x03, 0x00, "REGISTER 0x03");
  setRegisterAndPrint(REGISTER_VCSEL_CURRENT, VCSEL_CURRENT_LONG_RANGE, "VCSEL Current"); // 15 decimal.
  setRegisterAndPrint(0x05, 0x0C, "REGISTER 0x05"); // 12 decimal.
  setRegisterAndPrint(0x06, REGISTER_06H_BASE_VALUE | VCSEL_CURRENT_LONG_RANGE, "REGISTER 0x06"); // 111 decimal.
  setRegisterAndPrint(0x07, 0xDF, "REGISTER 0x07"); // 223 decimal.
  setRegisterAndPrint(0x0D, 0x82, "REGISTER 0x0D"); // 130 decimal.
  setRegisterAndPrint(0x0E, 0x02, "REGISTER 0x0E");
  setRegisterAndPrint(0x11, 0xAD, "REGISTER 0x11"); // 173 decimal.
  //setRegisterAndPrint(0x11, 0x98, "REGISTER 0x11"); // 152 decimal.
  setRegisterAndPrint(0x12, 0x00, "REGISTER 0x12");
  setRegisterAndPrint(0x13, 0x15, "REGISTER 0x13"); // 21 decimal.
  setRegisterAndPrint(0x14, REGISTER_14H_LONG_RANGE, "REGISTER 0x14"); // 20 decimal.
  setRegisterAndPrint(0x15, 0x01, "REGISTER 0x15");
  //setRegisterAndPrint(0x1A, 0x75, "REGISTER 0x1A"); // 117 decimal.
  //setRegisterAndPrint(0x1B, 0x57, "REGISTER 0x1B"); // 87 decimal.
  //setRegisterAndPrint(0x1C, 0x75, "REGISTER 0x1C");
  //setRegisterAndPrint(0x1D, 0x57, "REGISTER 0x1D");
  setRegisterAndPrint(0x1A, 0xAA, "REGISTER 0x1A"); // 170 decimal.
  setRegisterAndPrint(0x1B, 0xAA, "REGISTER 0x1B");
  setRegisterAndPrint(0x1C, 0xAA, "REGISTER 0x1C");
  setRegisterAndPrint(0x1D, 0xAA, "REGISTER 0x1D");
  setRegisterAndPrint(0x1E, 0xF4, "REGISTER 0x1E"); // 244 decimal.
  setRegisterAndPrint(0x1F, 0xF7, "REGISTER 0x1F"); // 247 decimal.
  setRegisterAndPrint(0x28, 0x84, "REGISTER 0x28"); // 132 decimal.
  setRegisterAndPrint(0x2A, 0x0B, "REGISTER 0x2A"); // 11 decimal.
  setRegisterAndPrint(0x43, REGISTER_43H_LONG_RANGE, "REGISTER 0x43"); // 208 decimal.
  setRegisterAndPrint(0x45, 0x04, "REGISTER 0x45");
  setRegisterAndPrint(0x47, REGISTER_47H_LONG_RANGE, "REGISTER 0x47"); // 0 decimal.
  setRegisterAndPrint(0x4D, 0x06, "REGISTER 0x4D");
  setRegisterAndPrint(0x4E, 0x3E, "REGISTER 0x4E"); // 62 decimal.
}

// Read measurement register.
void readMeasurementRegister(uint8_t registerAddress, uint8_t dataAddress, uint8_t* rdata) {
  setRegisterSingle(registerAddress, dataAddress);;
  rdata[0] = 0;
  rdata[1] = 0;
  rdata[2] = 0;
  readi2c(registerAddress, rdata, 3);
  printValue("rdata0", rdata[0], false);
  printValue("rdata1", rdata[1], false);
  printValue("rdata2", rdata[2], true);
  Serial.println("");
}

// Read measurement data.
void readMeasurementData() {
  uint8_t rdata[3];
  
  uint8_t range1_status = getRegisterSingle(REGISTER_RANGE1_STATUS);
  printValue("range1_status", range1_status, true);
  Serial.println("");

  readMeasurementRegister(REGISTER_MEASUREMENT_DATA, 0x64, rdata);
  uint8_t maxbin_ret = rdata[1];
  printValue("maxbin_ret", maxbin_ret, true);
  Serial.println("");
  
  readMeasurementRegister(REGISTER_MEASUREMENT_DATA, 0x66, rdata);
  int32_t fine_ret_fx16p = (int32_t) rdata[2] << 8 | (int32_t) rdata[1];
  printValue("fine_ret_fx16p", fine_ret_fx16p, true);
  Serial.println("");
}

/////////////////////////////////////////////////////////////////////////////

// Arduino setup function.
void setup() {
  // Start the hardware serial port for the serial monitor.
  Serial.begin(9600);
  Serial.println("");
  Serial.println("GP2AP02VT00F Demo");
  Serial.println("=================");

  // Start the Wire library.
  Wire.begin();
  delay(200);

  // Initialize sensor.
  softwareReset();
  
  // Check for valid device iD.
  int devID = getDeviceID();
  printValue("Device ID", devID, true);
  Serial.println("");

  // Init registers.
  initRegisters();

  // First measurement.
  startMeasurement();
}

// Arduino main loop.
void loop() {
  // Check if measurement has ended or not.
  if ( getMeasurementEndFlag() == MEASUREMENT_STATUS_NOT_END ) {
    Serial.println("Measurement Not End");
    delay(100);
    return;
  }
  
  // Read measurement data.
  Serial.println("---");
  readMeasurementData();

  // Clear measurement status end flag.
  clearMeasurementEndFlag();
  
  // Start next measurement.
  startMeasurement();
}

// END PROGRAM
