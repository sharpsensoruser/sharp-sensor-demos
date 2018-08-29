/////////////////////////////////////////////////////////////////////////////
// Sharp DC6S4ZN3101 Microwave Sensor Module Demo
//
// Board Connection:
//   Arduino   DC6S4ZN3101
//   3.3V      VCC
//   GND       GND
//   10pin     TXD
//   11pin     RXD
//
// Serial monitor setting:
//   115200 baud
/////////////////////////////////////////////////////////////////////////////

#include <SoftwareSerial.h>

// Use software serial port for communicating with DC6S4ZN3101.
// If you have Arduino Mega 2560, you can use hardware serial port
// instead like Serial1, etc.
#define PIN_RX 10
#define PIN_TX 11
static SoftwareSerial mySerial(PIN_RX, PIN_TX);

// Payload types.
#define PAYLOAD_TYPE_WAVEFORMIQDATA 1
#define PAYLOAD_TYPE_SIGNALMEANVALUE 5

// Demo program options.
#define PRINT_READ_SERIAL 1

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

/////////////////////////////////////////////////////////////////////////////

// Read data from software serial port.
int readSerial() {
  while (!mySerial.available()) {}
  int data = mySerial.read();

  #ifdef PRINT_READ_SERIAL
  Serial.print("Read: ");
  Serial.print(data);
  Serial.print(" (0x");
  Serial.print(data, HEX);
  Serial.println(")");
  #endif // PRINT_READ_SERIAL

  return data;
}

// Read the Payload Value into a buffer.
bool readPayloadValue(unsigned char* buf, int len) {
  for(int i = 0; i < len; i++) {
    int data = readSerial();
    if ( data < 0 )
      return false;
    buf[i] = (unsigned char)data;
  }
  return true;
}

// Read Payload Type 5 - Signal Mean Value.
void readPayloadSignalMeanValue() {
  // Read Payload Length. Sometimes the length is read as 1 which
  // contradicts the specification.
  int payloadLength = readSerial();
  if ( payloadLength != 2 )
    return;
    
  // Read Payload Value.
  unsigned char buf[2];
  if ( !readPayloadValue(buf, 2) )
    return false;
  short payloadValue = ((short)buf[0])<<8 | (short)buf[1];

  // Read Payload Sequence
  int payloadSequence = readSerial();
  if ( payloadSequence != 0 )
    return;
    
  // Read Payload Checksum and check it.
  int payloadChecksum = readSerial();
  int calcChecksum = calculateChecksum(buf, 2);
  if ( payloadChecksum != calcChecksum )
    return;
    
  // OK to print now that data packet is validated.
  printValue("Type", PAYLOAD_TYPE_SIGNALMEANVALUE);
  printValue("Length", payloadLength);
  printValue("SignalMeanValue", payloadValue);
  printValue("Sequence", payloadSequence);
  printValue("Checksum", payloadChecksum, true); 
  Serial.println("");
}

/////////////////////////////////////////////////////////////////////////////

// Helper function to send data through the software serial port.
void sendSerial(unsigned char data) {
  mySerial.write(data);
}

/////////////////////////////////////////////////////////////////////////////

// Arduino setup function.
void setup() {
  // Start the hardware serial port for the serial monitor.
  Serial.begin(115200);

  // Start the software serial port for communicating with DC6S4ZN3101.
  mySerial.begin(115200);

  // Wait for startup.
  delay(500);
}

// Arduino main loop.
void loop() {
  // Read Payload Type.
  int payloadType = readSerial();    
  if ( payloadType == PAYLOAD_TYPE_SIGNALMEANVALUE ) {
    readPayloadSignalMeanValue();
  }
  
} // END PROGRAM
