/////////////////////////////////////////////////////////////////////////////
// Sharp GP2Y1040AU0F Dust Sensor Demo (I2C)
//
// Board Connection:
//   Arduino       GP2Y1040AU0F
//   SDA           Pin 1 - SDA      Use 3.3V logic level converter
//   SCL           Pin 2 - SCL      Use 3.3V logic level converter
//   GND           Pin 3 - SEL
//   GND           Pin 4 - GND
//   5V            Pin 5 - VCC
//   GND           Pin 6 - GND
//
// NOTE:
// Arduino Wire library has fixed I2C buffer size of 32 bytes.
// But GP2Y1040AU0F data packet size in I2C is 72 bytes. 
// Need to make the following local changes to Arduino on Windows:
// 1. Open the file (C:\Program Files (x86)\Arduino\hardware\arduino\avr\libraries\Wire\src\utility\twi.h)　
// 2. Change from #define TWI_BUFFER_LENGTH 32 to #define TWI_BUFFER_LENGTH 128   
// 3. Open the file (C:\Program Files (x86)\Arduino\hardware\arduino\avr\libraries\Wire\src\Wire.h)
// 4. Change from #define BUFFER_LENGTH 32 to #define BUFFER_LENGTH 128　
/////////////////////////////////////////////////////////////////////////////

#include <Wire.h>

// Test sending some commands.
//#define TEST_SEND_CMDS
#ifdef TEST_SEND_CMDS
static int counter = 0;
#endif // TEST_SEND_CMDS

// GP2Y1040AU0F I2C slave address = 105 decimal.
#define SLAVE_ADDRESS 0x69

// I2C register addresses (commands).
#define REGISTER_READ     0x00
#define REGISTER_SLEEP    0x50
#define REGISTER_CLEAN    0x51
#define REGISTER_RESET    0x52
#define REGISTER_MAVE     0x53
#define REGISTER_TINT     0x58
#define REGISTER_TPREFAN  0x59
#define REGISTER_TINTC_H  0x5A
#define REGISTER_TINTC_L  0x5B
#define REGISTER_TCLEAN   0x5C
#define REGISTER_SPEEDFAN 0x63

// Store I2C raw data frame bytes and calculated field values.
static uint8_t frames[72];
static float NC_03;  // Number concentration of particles with size 0.3um to 10um
static float NC_05;  // Number concentration of particles with size 0.5um to 10um
static float NC_1;   // Number concentration of particles with size 1.0um to 10um 
static float NC_25;  // Number concentration of particles with size 2.5um to 10um
static float NC_4;   // Number concentration of particles with size 4.0um to 10um
static float PM1_1;  // Mass concentration of particles with size 0.3um to 1.0um (standard particle)
static float PM25_1; // Mass concentration of particles with size 0.3um to 2.5um (standard particle)
static float PM10_1; // Mass concentration of particles with size 0.3um to 10um (standard particle)
static float PM1_2;  // Mass concentration of particles with size 0.3um to 1.0um (cigarette smoke)
static float PM25_2; // Mass concentration of particles with size 0.3um to 2.5um (cigarette smoke)
static float PM10_2; // Mass concentration of particles with size 0.3um to 10um (cigarette smoke)

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

// Helper function to compute CRC-8 with polynomial 0x31 and initialization 0xFF.
uint8_t computeCRC(uint8_t dataHi, uint8_t dataLo)
{
  uint8_t crcPoly = 0x31;
  uint8_t crc = 0xFF;

  uint8_t buf[2];
  buf[0] = dataHi;
  buf[1] = dataLo;
  for(int j = 0; j < 2; j++)
  {
    crc ^= buf[j];
    for (int i = 0; i < 8; i++)
    {
      if ((crc & 0x80) != 0)
          crc = (uint8_t)((crc << 1) ^ crcPoly);
      else
          crc <<= 1;
    }
  }

  return crc;
}

// Helper function to convert 6 bytes of data to 32-bit IEEE754 float.
// Return -1 if CRC check failed.
float convertToFloat(uint8_t* data)
{
  // Check CRCs.
  uint8_t crc = computeCRC(data[0], data[1]);
  if ( crc != data[2] )
    return -1;
  crc = computeCRC(data[3], data[4]);
  if ( crc != data[5] )
    return -1;
        
  // Form the 32-bit hex representation.
  uint32_t val = ((uint32_t)data[0])<<24 | ((uint32_t)data[1])<<16 | ((uint32_t)data[3])<<8 | data[4];
  float fval = *((float*)&val);

  return fval;
}

// Helper function to print a data value to the serial monitor.
void printValue(String text, float value, bool isLast = false) {
  Serial.print(text);
  Serial.print("=");
  Serial.print(value);
  if (!isLast) {
    Serial.print(", ");
  }
}

/////////////////////////////////////////////////////////////////////////////

// Read the 72-byte air quality data record.
void readAirQualityData() {
  Serial.println("---");
    
  // Specify the register address to read from.
  Wire.beginTransmission(SLAVE_ADDRESS);
  Wire.write(REGISTER_READ);
  Wire.endTransmission();

  // Request to read 72 bytes at a time.
  Wire.requestFrom(SLAVE_ADDRESS, 72);
  int numRead = 0;
  while( numRead < 72 )
  {    
    if ( !Wire.available() )
      return;
    frames[numRead++] = Wire.read();
  }
  
  // Print raw air quality data frames.  
  for(int j = 0; j < 6; j++)
  {
    Serial.print("Read Air Data (HEX):");
    for(int i = j*12; i < (j*12+12); i++) {
      Serial.print(" ");
      Serial.print(frames[i], HEX);
    }
    Serial.println("");
  }

  // Calculate the number and mass concentrations from the raw frame data.
  NC_03  = convertToFloat(&frames[0]);
  NC_05  = convertToFloat(&frames[6]);
  NC_1   = convertToFloat(&frames[12]);
  NC_25  = convertToFloat(&frames[18]);
  NC_4   = convertToFloat(&frames[24]);
  PM1_1  = convertToFloat(&frames[36]);
  PM25_1 = convertToFloat(&frames[42]);
  PM10_1 = convertToFloat(&frames[48]);
  PM1_2  = convertToFloat(&frames[54]);
  PM25_2 = convertToFloat(&frames[60]);
  PM10_2 = convertToFloat(&frames[66]);

  // Print concentration values.
  printValue("NC_03",  NC_03);
  printValue("NC_05",  NC_05);
  printValue("NC_1",   NC_1);
  printValue("NC_25",  NC_25);
  printValue("NC_4",   NC_4, true);
  Serial.println("");
  printValue("PM1_1",  PM1_1);
  printValue("PM25_1", PM25_1);
  printValue("PM10_1", PM10_1, true);
  Serial.println("");
  printValue("PM1_2",  PM1_2);
  printValue("PM25_2", PM25_2);
  printValue("PM10_2", PM10_2, true);
  Serial.println("");
}

// Read the command data value. Applicable to commands
// MAVE, TINT, TPREFAN, TINTC_H/L, TCLEAN, SPEEDFAN
void readCommandData(String cmdText, int cmdRegister)
{
  Serial.println("---");
  Serial.print("Read Cmd Data: " + cmdText + "=");
    
  // Specify the register address to read from.
  Wire.beginTransmission(SLAVE_ADDRESS);
  Wire.write(cmdRegister);
  Wire.endTransmission();

  // Request to read 1 byte.
  Wire.requestFrom(cmdRegister, 1);

  // Read and print the data value.
  if ( Wire.available() )
  {
    uint8_t data = Wire.read();    
    Serial.print(data);
  }
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

  // Start the Wire library.
  Wire.begin();
  
   // Wait a second for startup.
  delay(1000);
}

// Arduino main loop.
void loop() {
  
  // Test sending some cmds.
  #ifdef TEST_SEND_CMDS
  counter++;
  if ( counter == 10 ) {
    readCommandData("MAVE", REGISTER_MAVE);   // Read MAVE value
    writeRegister(REGISTER_CLEAN, 1);         // Start cleaning
    return;  
  }
  #endif // TEST_SEND_CMDS
  
  // Read air quality data once per second.
  readAirQualityData();
  delay(1000);
  
} // END PROGRAM
