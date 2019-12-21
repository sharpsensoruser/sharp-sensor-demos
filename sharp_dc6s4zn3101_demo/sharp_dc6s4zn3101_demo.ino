/////////////////////////////////////////////////////////////////////////////
// Sharp DC6S4ZN3101 Microwave Sensor Module Demo
//
// Board Connection:
//   Arduino Mega   DC6S4ZN3101   Arduino Uno
//   3.3V           VCC           3.3V          
//   GND            GND           GND
//   RX1-pin19      TXD           pin10
//   TX1-pin18      RXD           pin11
//
// Serial monitor setting:
//   115200 baud
/////////////////////////////////////////////////////////////////////////////

#include <SoftwareSerial.h>

// Demo program options.
#define PRINT_READ_SERIAL
#define PRINT_SEND_SERIAL
#define USE_SOFTWARE_SERIAL
#define VERIFY_CHECKSUM

// Payload types.
#define PAYLOAD_TYPE_WAVEFORMDATA 1
#define PAYLOAD_TYPE_SIGNALMEANVALUE 5
#define PAYLOAD_TYPE_DEBUGINFO 7
#define PAYLOAD_TYPE_ALARMS 11

// Use software serial port for communicating with DC6S4ZN3101.
// If you have Arduino Mega 2560, you can use hardware serial port
// instead like Serial1, etc.
#ifdef USE_SOFTWARE_SERIAL
#define PIN_RX 10
#define PIN_TX 11
static SoftwareSerial mySerial(PIN_RX, PIN_TX);
#endif // USE_SOFTWARE_SERIAL

/////////////////////////////////////////////////////////////////////////////

// Calculate checksum.
unsigned char calculateChecksum(const unsigned char* buf, int len) {
  unsigned char checksum = 0xff;
  for(int i = 0; i < len; i++) {
    checksum ^= buf[i];
  }
  return checksum;
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
void printTextValue(String text, String value, bool isLast = false) {
  Serial.print(text);
  Serial.print("=");
  Serial.print(value);
  if (!isLast) {
    Serial.print(", ");
  }
}

/////////////////////////////////////////////////////////////////////////////

// Read data from serial port.
int readSerial() {
  #ifdef USE_SOFTWARE_SERIAL
  while (!mySerial.available()) {}
  int data = mySerial.read();
  #else
  while (!Serial1.available()) {}
  int data = Serial1.read();
  #endif // USE_SOFTWARE_SERIAL
  
  #ifdef PRINT_READ_SERIAL
  Serial.print("Read: ");
  Serial.print(data);
  Serial.print(" (0x");
  Serial.print(data, HEX);
  Serial.println(")");
  #endif // PRINT_READ_SERIAL

  return data;
}

// Read the payload value into a buffer.
// Return false if error.
bool readPayloadValue(unsigned char* buf, int len) {
  for(int i = 0; i < len; i++) {
    int data = readSerial();
    if ( data < 0 )
      return false;
    buf[i] = (unsigned char)data;
  }
  return true;
}

// Read waveform channel I/Q data.
void readWaveformData() {
  // Read payload length. Length should be 4 bytes.
  int payloadLength = readSerial();
  if ( payloadLength != 4 )
    return;
    
  // Read payload value.
  unsigned char buf[4];
  if ( !readPayloadValue(buf, 4) )
    return;
  short waveI = ((short)buf[0])<<8 | (short)buf[1];
  short waveQ = ((short)buf[2])<<8 | (short)buf[3];

  // Read sequence number.
  int sequence = readSerial();
  if ( sequence < 0 || sequence > 127 )
    return;
    
  // Read checksum and verify it.
  #ifdef VERIFY_CHECKSUM
  int checksum = readSerial();
  int calcChecksum = calculateChecksum(buf, 4);
  if ( checksum != calcChecksum )
    return;
  #endif // VERIFY_CHECKSUM
  
  // Print info now that data packet is validated.
  printValue("Type", PAYLOAD_TYPE_WAVEFORMDATA);
  printValue("Length", payloadLength);
  printValue("WaveI", waveI);
  printValue("WaveQ", waveQ);
  #ifdef VERIFY_CHECKSUM
  printValue("Sequence", sequence);
  printValue("Checksum", checksum, true);
  #else
  printValue("Sequence", sequence, true);
  #endif // VERIFY_CHECKSUM
  Serial.println("");
}

// Read signal mean value.
void readSignalMeanValue() {
  // Read payload length. Length should be 2 bytes.
  int payloadLength = readSerial();
  if ( payloadLength != 2 )
    return;
    
  // Read payload value.
  unsigned char buf[2];
  if ( !readPayloadValue(buf, 2) )
    return;
  short payloadValue = ((short)buf[0])<<8 | (short)buf[1];

  // Read sequence number.
  int sequence = readSerial();
  if ( sequence != 0 )
    return;
    
  // Read checksum and verify it.
  #ifdef VERIFY_CHECKSUM
  int checksum = readSerial();
  int calcChecksum = calculateChecksum(buf, 2);
  if ( checksum != calcChecksum )
    return;
  #endif // VERIFY_CHECKSUM
  
  // Print info now that data packet is validated.
  printValue("Type", PAYLOAD_TYPE_SIGNALMEANVALUE);
  printValue("Length", payloadLength);
  printValue("SignalMeanValue", payloadValue);
  #ifdef VERIFY_CHECKSUM
  printValue("Sequence", sequence);
  printValue("Checksum", checksum, true);
  #else
  printValue("Sequence", sequence, true);
  #endif // VERIFY_CHECKSUM 
  Serial.println("");
}

// Read debug information.
void readDebugInfo() {
  // Read payload length which should be from 1 to 32 bytes.
  int payloadLength = readSerial();
  if ( payloadLength < 1 || payloadLength > 32 )
    return;
    
  // Read payload value.
  unsigned char buf[33];
  memset(buf, 0, 33);
  if ( !readPayloadValue(buf, payloadLength) )
    return;
  
  // Print info.
  printValue("Type", PAYLOAD_TYPE_DEBUGINFO);
  printValue("Length", payloadLength);
  printTextValue("DebugInfo", buf, true);
  Serial.println("");
}

// Read alarms.
void readAlarms() {
  // Read payload length. Length should be 2 bytes.
  int payloadLength = readSerial();
  if ( payloadLength != 2 )
    return;
    
  // Read payload value.
  unsigned char buf[2];
  if ( !readPayloadValue(buf, 2) )
    return;
  short alarm0 = (buf[0] & 0xf0)>>4;
  short alarm1 = (buf[0] & 0x0f);
  short alarm2 = (buf[1] & 0xf0)>>4;
  short alarm3 = (buf[1] & 0x0f);
  if ( alarm0 > 1 || alarm1 > 1 || alarm2 > 1 || alarm3 > 1 )
    return;
    
  // Read sequence number.
  int sequence = readSerial();
  if ( sequence != 0 )
    return;
    
  // Read checksum and verify it.
  #ifdef VERIFY_CHECKSUM
  int checksum = readSerial();
  int calcChecksum = calculateChecksum(buf, 2);
  if ( checksum != calcChecksum )
    return;
  #endif // VERIFY_CHECKSUM
  
  // Print info now that data packet is validated.
  printValue("Type", PAYLOAD_TYPE_ALARMS);
  printValue("Length", payloadLength);
  printValue("Alarm0", alarm0);
  printValue("Alarm1", alarm1);
  printValue("Alarm2", alarm2);
  printValue("Alarm3", alarm3);
  #ifdef VERIFY_CHECKSUM
  printValue("Sequence", sequence);
  printValue("Checksum", checksum, true);
  #else
  printValue("Sequence", sequence, true);
  #endif // VERIFY_CHECKSUM
  Serial.println("");
}

/////////////////////////////////////////////////////////////////////////////

// Helper function to send data through the serial port.
void sendSerial(const char* data) {
  #ifdef USE_SOFTWARE_SERIAL
  mySerial.write(data, strlen(data));
  mySerial.write(0x0d);
  #else
  Serial1.write(data, strlen(data));
  Serial1.write(0x0d);
  #endif // USE_SOFTWARE_SERIAL
  
  #ifdef PRINT_SEND_SERIAL
  Serial.print("Send: ");
  Serial.println(data);
  #endif // PRINT_SEND_SERIAL

  delay(10);
}

/////////////////////////////////////////////////////////////////////////////

// Arduino setup function.
void setup() {
  // Start the hardware serial port for the serial monitor.
  Serial.begin(115200);

  // Start the serial port for communicating with DC6S4ZN3101.
  #ifdef USE_SOFTWARE_SERIAL
  mySerial.begin(115200);
  #else
  Serial1.begin(115200);
  #endif // USE_SOFTWARE_SERIAL
  
  // Set waveform mode off.
  sendSerial("wave off");
  
  // Set alarm0 threshold to 400.
  sendSerial("th0 400");

  // Set alarm1 threshold to 950.
  sendSerial("th1 950");

  // Set alarm2 ON timer duration to 0.5 seconds.
  sendSerial("on2tm 5");
  
  // Set alarm3 OFF timer duration to 1 second.
  sendSerial("off3tm 10");

  // Send request for firmware version.
  sendSerial("ver");

  // Set waveform mode on (wave 100 / wave 500).
  //sendSerial("wave 100");
}

// Arduino main loop.
void loop() {
  // Read payload type and handle accordingly.
  int payloadType = readSerial();
  if ( payloadType == PAYLOAD_TYPE_WAVEFORMDATA ) {
    readWaveformData(); 
  } else if ( payloadType == PAYLOAD_TYPE_SIGNALMEANVALUE ) {
    readSignalMeanValue();
  } else if ( payloadType == PAYLOAD_TYPE_DEBUGINFO ) {
    readDebugInfo();    
  } else if ( payloadType == PAYLOAD_TYPE_ALARMS ) {
    readAlarms();
  }
  
} // END PROGRAM
