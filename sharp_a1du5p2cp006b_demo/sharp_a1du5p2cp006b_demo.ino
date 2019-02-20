/////////////////////////////////////////////////////////////////////////////
// Sharp A1DU5P2CP006B Humidity Sensor Module
//
// Sensor IC: AMS ENS210
//
// Board Connection:
//   Arduino   A1DU5P2CP006B
//   3.3V      Pin 1 - VDD
//   SCL       Pin 2 - SCL
//   SDA       Pin 3 - SDA
//   GND       Pin 4 - GND
//   
// Reference: https://ams.com/ens210
/////////////////////////////////////////////////////////////////////////////

#include <Wire.h>

// Sensor I2C slave address = 67 decimal.
#define SLAVE_ADDRESS 0x43

// Sensor I2C register addresses as defined in ENS210 datasheet.
#define REGISTER_ADDRESS_SYS_CTRL   0x10
#define REGISTER_ADDRESS_SENS_RUN   0x21
#define REGISTER_ADDRESS_SENS_START 0x22
#define REGISTER_ADDRESS_T_VAL      0x30
#define REGISTER_ADDRESS_H_VAL      0x33

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

// Helper function to read a 3 byte measurement value from register.
uint32_t readMeasurement(uint8_t regAddress)
{
  // Specify the register address to read from.
  Wire.beginTransmission(SLAVE_ADDRESS);
  Wire.write(regAddress);
  Wire.endTransmission();

  // Request to read 3 bytes.
  Wire.requestFrom(SLAVE_ADDRESS, 3);
  uint8_t b0 = Wire.read();
  uint8_t b1 = Wire.read();
  uint8_t b2 = Wire.read();
  uint32_t measurement = (uint32_t)b0 + (uint32_t)b1*256 + (uint32_t)b2*65536;

  // Only take the last 16-bits.
  measurement = measurement & 0xffff;
  
  return measurement;
}

/////////////////////////////////////////////////////////////////////////////

void setup() {
  // Start the hardware serial port for the serial monitor.
  Serial.begin(9600);
  Serial.println("");
  Serial.println("A1DU5P2CP006B Demo");
  Serial.println("==================");

  // Start the Wire library.
  Wire.begin();

  // Reset the sensor and put it into standby mode.
  writeRegister(REGISTER_ADDRESS_SYS_CTRL, 0x81);

  // Set the run mode of the sensor to be Single Shot for both humidity and temperature.
  writeRegister(REGISTER_ADDRESS_SENS_RUN, 0x00);
}

/////////////////////////////////////////////////////////////////////////////

void loop() {  
  // Start a measurement for both humidity and temperature.
  writeRegister(REGISTER_ADDRESS_SENS_START, 0x03);

  // Read temperature value.
  uint32_t rawTemperature = readMeasurement(REGISTER_ADDRESS_T_VAL);
  Serial.print("Temperature = ");
  float tempKelvin = rawTemperature/64.0;
  float tempC = tempKelvin - 273.15;
  Serial.print((tempC + 0.05), 1);
  Serial.println(" degrees Celsius");
  
  // Read relative humidity value.
  uint32_t rawHumidity = readMeasurement(REGISTER_ADDRESS_H_VAL);
  Serial.print("Humidity = ");
  Serial.print((rawHumidity/512.0 + 0.05), 1);
  Serial.println("%");

  Serial.println("");
  delay(500);
}
