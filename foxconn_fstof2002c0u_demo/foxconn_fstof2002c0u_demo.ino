/////////////////////////////////////////////////////////////////////////////
// Foxconn FSTOF2002C0U TOF Module Demo (UART)
//
// Board Connection:
//   Arduino   FSTOF2002C0U
//   3.3V      VCC
//   GND       GND
//   10pin     TX
//   11pin     RX
//
// Serial monitor setting:
//   9600 baud 8-N-1
/////////////////////////////////////////////////////////////////////////////

#include <SoftwareSerial.h>

// Use software serial port for communicating with FSTOF2002C0U.
// If you have Arduino Mega 2560, you can use hardware serial port
// instead like Serial1, etc.
#define rxPin 10
#define txPin 11
static SoftwareSerial mySerial(rxPin, txPin);

// UART command codes.
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

// Range1 status values (same as Sharp GP2AP02VT00F TOF Sensor).
#define RANGE1_STATUS_VALID_DATA      0x00
#define RANGE1_STATUS_VCSEL_SHORT     0x01
#define RANGE1_STATUS_LOW_SIGNAL      0x02
#define RANGE1_STATUS_LOW_SN          0x04
#define RANGE1_STATUS_TOO_MUCH_AMB    0x08
#define RANGE1_STATUS_WAF             0x10
#define RANGE1_STATUS_CAL_ERROR       0x20  
#define RANGE1_STATUS_CROSSTALK_ERROR 0x80

/////////////////////////////////////////////////////////////////////////////

// Helper function to send data through the software serial port.
void sendSerial(unsigned char data) {
  mySerial.write(data);
}

// Helper function to read data from software serial port.
int readSerial() {
  while (!mySerial.available()) {}
  return mySerial.read();
}

// Helper function to request distance measurement.
void send_CMD_ST_MM() {
  // Send 1st Packet Header Byte.
  sendSerial(0x55);

  // Send 2nd Packet Header Byte.
  sendSerial(0xAA);

  // Send the Command Byte for measuring distance.
  sendSerial(TOFM_CMD_ST_MM);

  // Send the Data Length for this command in bytes.
  sendSerial(0);

  // Send the Ending Packet Byte.
  sendSerial(0xFA);
}

// Read the measured distance from the data packet (range1).
void readDistance() {
  // Read the Data Length.
  int dataLength = readSerial();
  if ( dataLength != 0x03 )
    return;

  // Read the Hi byte of the distance.
  int distHi = readSerial();

  // Read the Lo byte of the distance.
  int distLo = readSerial();

  // Read the Range1 Status value.
  int range1Status = readSerial();
  
  // Read the End Byte.
  if ( readSerial() != 0xFA )
    return;
    
  // Print the distance to the Serial Monitor.
  int distMM = distHi * 256 + distLo;
  Serial.print("Distance = ");
  Serial.print(distMM);
  Serial.print("mm, ");
  Serial.print("Range1 Status = 0x");
  Serial.println(range1Status, HEX);
}

/////////////////////////////////////////////////////////////////////////////

// Arduino setup function.
void setup() {
  // Start the hardware serial port for the serial monitor.
  Serial.begin(9600);
  Serial.println("");
  Serial.println("FSTOF2002C0U Demo");
  Serial.println("=================");
  
  // Start the software serial port for communicating with FSTOF2002C0U.
  mySerial.begin(9600);

  // Wait two seconds for startup.
  delay(2000);
}

// Arduino main loop.
void loop() {
  // Send the request for a distance measurement.
  send_CMD_ST_MM();
  
  // Look for 1st Packet Header Byte.
  if ( readSerial() != 0x55 )
    return;

  // Look for 2nd Packet Header Byte.
  if ( readSerial() != 0xAA )
    return;

  // Look for the Command for measuring distance.
  int cmd = readSerial();
  if ( cmd == TOFM_CMD_ST_MM ) {
    readDistance();
  }

} // END PROGRAM
