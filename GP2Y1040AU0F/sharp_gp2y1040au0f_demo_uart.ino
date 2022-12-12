/////////////////////////////////////////////////////////////////////////////
// Sharp GP2Y1040AU0F Dust Sensor Demo (UART)
//
// Board Connection:
//   Arduino       GP2Y1040AU0F
//   10pin         Pin 1 - TXD      Use 3.3V logic level converter
//   11pin         Pin 2 - RXD      Use 3.3V logic level converter
//   Not Connected Pin 3 - SEL
//   GND           Pin 4 - GND
//   5V            Pin 5 - VCC
//   GND           Pin 6 - GND
//
// Serial monitor setting:
//   9600 baud 8-N-1
/////////////////////////////////////////////////////////////////////////////

#include <SoftwareSerial.h>

// Test sending some commands.
//#define TEST_SEND_CMDS
#ifdef TEST_SEND_CMDS
static int counter = 0;
#endif // TEST_SEND_CMDS

// Use software serial port for communicating with GP2Y1040AU0F.
// If you have Arduino Mega 2560, you can use hardware serial port
// instead like Serial1, etc.
#define rxPin 10
#define txPin 11
static SoftwareSerial mySerial(rxPin, txPin);

// Starting bytes of UART data record.
#define START_FRAME_1 0xFF
#define START_FRAME_2 0xFA

// Starting bytes of UART cmd record.
#define CMD_FRAME_1_OLD 0x42 // Old start byte for UART cmds
#define CMD_FRAME_1_NEW 0xA1 // New start byte (Sharp spec change from Sep 9, 2022)
#define CMD_FRAME_1     CMD_FRAME_1_NEW 
#define CMD_FRAME_2     0x4D
        
// Store UART raw data frame bytes and calculated field values.
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

/////////////////////////////////////////////////////////////////////////////

// Read the remainder of the 28-byte UART air quality data record.
void readAirQualityData() {
  // Set the 1st start frame since we already read it.
  frames[0] = START_FRAME_1;
  
  // Look for the 2nd start frame.
  int data = readSerial();
  if ( data != START_FRAME_2 )
    return;
  frames[1] = (uint8_t)data;
  
  // Read the next 26 frames/bytes.
  for(int i = 2; i < 28; i++) {
    data = readSerial();
    if (data < 0 || data > 255)
      return;
    frames[i] = (uint8_t)data;    
  }

  // Print raw air quality data frames.
  Serial.println("---");
  Serial.print("Read Air Data (HEX):");
  for(int i = 0; i < 14; i++) {
    Serial.print(" ");
    Serial.print(frames[i], HEX);
  }
  Serial.println("");
  Serial.print("Read Air Data (HEX):");
  for(int i = 14; i < 28; i++) {
    Serial.print(" ");
    Serial.print(frames[i], HEX);
  }
  Serial.println("");
  
  // Compare the checksum.
  uint8_t checksum = 0;
  for(int i = 0; i < 27; i++) {
    checksum += frames[i];
  }
  if ( checksum != frames[27] ) {
    Serial.print("Error: Checksum doesn't match: calculated sum is: 0x");
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
  printValue("PM10_1", PM10_1, true);
  Serial.println("");
  printValue("PM1_2",  PM1_2);
  printValue("PM25_2", PM25_2);
  printValue("PM10_2", PM10_2, true);
  Serial.println("");
}

// Read the rest of command record following the first cmd frame.
void readCommandData(uint8_t firstFrame)
{
  // Set the 1st cmd frame since we already read it.
  frames[0] = firstFrame;

  // Look for the 2nd cmd frame.
  int data = readSerial();
  if ( data != CMD_FRAME_2 )
    return;
  frames[1] = (uint8_t)data;
  
  // Read in 5 remaining bytes from serial.
  for(int i = 2; i < 7; i++)
  {
    data = readSerial();
    if (data < 0 || data > 255)
      return;
    frames[i] = (uint8_t)data;
  }

  // Print out the 7 bytes of cmd data.
  Serial.println("---");
  Serial.print("Read Cmd Data (HEX):");
  for(int i = 0; i < 7; i++)
  {
    Serial.print(" ");
    Serial.print(frames[i], HEX);
  }
  Serial.println("");
}

// Send UART command data to sensor.
//
// Examples of the 7-bytes of commmand data:
//   READ: Read data in passive mode: A1 4D E2 00 00 01 71
//   MODE: Switch to passive mode: A1 4D E1 00 00 01 70
//   MODE: Switch to active mode: A1 4D E1 00 01 01 71
//   SLEEP: Go to sleep: A1 4D E4 00 00 01 73
//   SLEEP: Wake up: A1 4D E4 00 01 01 74
//   CLEAN: Start cleaning: A1 4D 01 00 00 00 90
//   RESET: Software reset: A1 4D 02 81 00 01 12
//   MAVE: Set number of moving averages to 10: A1 4D 03 00 0A 00 9C
//   TINT: Set interval time to 0 seconds: A1 4D 04 00 00 00 93
//   TPREFAN: Set pre-rotation time of fan to 3 seconds: A1 4D 05 00 03 00 97
//   TINTC: Set interval time for auto-cleaning to 60480[10s]: A1 4D 06 EC 40 01 C1
//   TCLEAN: Set cleaning time to 10 seconds: A1 4D 07 00 0A 00 A0      
void sendCommandData(uint8_t cmd, uint16_t dataValue)
{
  uint8_t buf[7];
  buf[0] = CMD_FRAME_1;
  buf[1] = CMD_FRAME_2;
  buf[2] = cmd;
  buf[3] = (uint8_t)((dataValue & 0xFF00) >> 8);
  buf[4] = (uint8_t)(dataValue & 0x00FF);
  uint16_t checksum = buf[0] + buf[1] + buf[2] + buf[3] + buf[4];
  if ( CMD_FRAME_1 == CMD_FRAME_1_NEW )
  {
    checksum -= 0x5F; // According to Sharp Application Note Rev 1.1 dated May 17, 2022
  }
  buf[5] = (uint8_t)((checksum & 0xFF00) >> 8);
  buf[6] = (uint8_t)(checksum & 0x00FF);

  // Print out the cmd data before sending.
  Serial.println("---");
  Serial.print("Send Cmd Data (HEX):");
  for(int i = 0; i < 7; i++)
  {
    Serial.print(" ");
    Serial.print(buf[i], HEX);
  }
  Serial.println("");

  // Send cmd data to sensor.
  mySerial.write(buf, 7);
}

/////////////////////////////////////////////////////////////////////////////

// Arduino setup function.
void setup() {
  // Start the hardware serial port for the serial monitor.
  Serial.begin(9600);
  Serial.println("");
  Serial.println("GP2Y1040AU0F Demo");
  Serial.println("=================");

  // Start the software serial port for sending and receiving data from GP2Y1040AU0F.
  mySerial.begin(9600);

  // Wait a second for startup.
  delay(1000);
}

// Arduino main loop.
void loop() {
  // Test sending some cmds.
  #ifdef TEST_SEND_CMDS
  counter++;
  if ( counter == 10 )
  {
    sendCommandData(0xE1, 0);    // Switch to passive mode
    sendCommandData(0x03, 10);   // Set number of moving averages to 10    
    sendCommandData(0xE2, 0);    // Read air data in passive mode    
    sendCommandData(0x01, 0);    // Start cleaning
    sendCommandData(0xE1, 1);    // Switch to active mode
    return;  
  }
  #endif // TEST_SEND_CMDS
  
  // Read frame and handle accordingly.
  int frame = readSerial();
  if (frame == START_FRAME_1)
  {
    readAirQualityData();
  }
  else if (frame == CMD_FRAME_1_OLD || frame == CMD_FRAME_1_NEW)
  {
    readCommandData(frame);
  }
  
  delay(100);
  
} // END PROGRAM