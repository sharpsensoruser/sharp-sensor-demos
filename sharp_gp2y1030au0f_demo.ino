/////////////////////////////////////////////////////////////////////////////
// Sharp GP2Y1030AU0F Dust Sensor Demo
//
// Board Connection:
//   Arduino   GP2Y1030
//   GND       GND
//   5V        Vcc
//   10pin     TxD
//
// Serial monitor setting:
//   9600 baud
/////////////////////////////////////////////////////////////////////////////

#include <SoftwareSerial.h>

// Choose program options.
#define PRINT_RAW_DATA
#define PRINT_RAW_COUNTS
#define PRINT_AVG_COUNTS
#define USE_MOVING_AVG

// Use software serial port for communicating with GP2Y1030.
// If you have Arduino Mega 2560, you can use hardware serial port
// instead like Serial1, etc.
#define rxPin 10
#define txPin 11
static SoftwareSerial mySerial(rxPin, txPin);

// Moving average buffers for storing and then averaging last N particle counts.
#define N 10
static unsigned int PM_A_Buf[N];
static unsigned int PM_B_Buf[N];
static unsigned int PM_C_Buf[N];
static int bufIndex = 0;
static bool isBufferFull = false;

// Enums to help with conversion of particle count to dust density/concentration.
enum TestParticleType {
  SILICA, // Silica particles
  SMOKE,  // Cigarette smoke
  ACFINE, // Test dust
  ATMOS   // Atmospheric dust
};
enum TestParticleSize {
  PM_SIZE_A,   // Less than 2.5 microns in diameter
  PM_SIZE_B,   // Greater than 2.5 microns in diameter (NOT USED)
  PM_SIZE_C    // All particle sizes (total).
};

/////////////////////////////////////////////////////////////////////////////

// Helper function to read data from software serial port.
int getSerial() {
  while (!mySerial.available()) {}
  return mySerial.read();
}

// Helper functions to print a data value to the serial monitor.
void printValue(String text, int value, bool isLast = false) {
  Serial.print(text);
  Serial.print("=");
  Serial.print(value);
  if (!isLast) {
    Serial.print(", ");
  }
}
void printFValue(String text, float value, String units, bool isLast = false) {
  Serial.print(text);
  Serial.print("=");
  Serial.print(value);
  Serial.print(units);
  if (!isLast) {
    Serial.print(", ");
  }
}

// Helper function to calculate dust density (dust or mass concentration)
// in units of ug/m3 based on particle count. The conversion formula from
// particle count to dust density is dependent on many factors such as the
// magnitude of the particle count, the type and size of particles being
// tested, and the type of reference dust monitor being used.
// Returns -1 if error.
float getDustDensity(TestParticleType pmType, TestParticleSize pmSize,
        unsigned int pmCount) {
  unsigned int x = pmCount;
  float y = -1.0;

  if ( pmType == SILICA ) {
    if ( pmSize == PM_SIZE_A || pmSize == PM_SIZE_C )
      y = (2.0e-09)*x*x*x - (9.0e-06)*x*x + 0.0777*x;
  } else if ( pmType == SMOKE ) {
    if ( pmSize == PM_SIZE_A || pmSize == PM_SIZE_C ) {
      if ( x < 60 )
        y = 0.8501*x;
      else if ( x > 60 && x < 600 )
        y = 0.3687*x + 28.218;
    }
  } else if ( pmType == ACFINE ) {
    if ( x < 6000 ) {
      if ( pmSize == PM_SIZE_A )
        y = (1.513e-09)*x*x*x -(1.111e-05)*x*x + (7.094e-02)*x;
      else if ( pmSize == PM_SIZE_C )
        y = (5.314e-10)*x*x*x -(6.520e-06)*x*x + (6.519e-02)*x;
    }
  } else if ( pmType == ATMOS ) {
    if ( pmSize == PM_SIZE_A ) {
      if ( x <= 2000 )
        y = (3.4219e-02)*x;
      else if ( x > 2000 && x <= 7000 )
        y = (1.952e-02)*x + 29.36;
    } else if ( pmSize == PM_SIZE_C ) {
      if ( x <= 2000 )
        y = (5.009e-02)*x;
      else if ( x > 2000 && x <= 8000 )
        y = (3.082e-02)*x + 38.54;
      else if ( x > 8000 )
        y = (1.634e-02)*x + 154.4;
    }
  }

  return y;
}

/////////////////////////////////////////////////////////////////////////////

// Arduino setup function.
void setup() {
  // Start the hardware serial port for the serial monitor.
  Serial.begin(9600);

  // Start the software serial port for receiving data from GP2Y1030.
  mySerial.begin(2400);

  // Wait two seconds for startup.
  delay(2000);
}

// Arduino main loop.
void loop() {  
  // Look for StartFrame1 byte.
  if ( getSerial() != 0xff )
    return;

  // Look for StartFrame2 byte.
  if ( getSerial() != 0xfa )
    return;

  // Read in the next 8 frames (bytes).
  // PM_A refers to count of particles < 2.5 microns in size.
  // PM_B refers to count of particles > 2.5 microns in size.
  // PM_C refers to count of all particle sizes (total count).
  int PM_A_High = getSerial();
  int PM_A_Low  = getSerial();
  int PM_B_High = getSerial();
  int PM_B_Low  = getSerial();
  int PM_C_High = getSerial();
  int PM_C_Low  = getSerial();
  int Reserved  = getSerial();
  int CheckSum  = getSerial();
  
  // Reserved byte should be zero.
  if ( Reserved != 0 )
    return;
    
  // Verify checksum by adding the raw data values and taking the lower 8 bits.
  unsigned int sum = PM_A_High + PM_A_Low + PM_B_High + PM_B_Low + PM_C_High + PM_C_Low;
  sum = sum & 0xff;
  if ( sum != CheckSum )
    return;
  Serial.println("");
  
  // Print raw data values.
  #ifdef PRINT_RAW_DATA
  Serial.print("RAW DATA......");
  printValue("PM_A_High", PM_A_High);
  printValue("PM_A_Low", PM_A_Low);
  printValue("PM_B_High", PM_B_High);
  printValue("PM_B_Low", PM_B_Low);
  printValue("PM_C_High", PM_C_High);
  printValue("PM_C_Low", PM_C_Low);
  printValue("Reserved", Reserved);
  printValue("CheckSum", CheckSum, true);
  Serial.println("");
  #endif // PRINT_RAW_DATA

  // Calculate particle counts.
  unsigned int PM_A_Count = PM_A_High * 256 + PM_A_Low;
  unsigned int PM_B_Count = PM_B_High * 256 + PM_B_Low;
  unsigned int PM_C_Count = PM_C_High * 256 + PM_C_Low;
  #ifdef PRINT_RAW_COUNTS
  Serial.print("RAW COUNTS....");
  printValue("PM_A_Count", PM_A_Count);
  printValue("PM_B_Count", PM_B_Count);
  printValue("PM_C_Count", PM_C_Count, true);
  Serial.println("");
  #endif // PRINT_RAW_COUNTS

  // Calculate moving average counts. We calculate the average
  // if the buffer is full or has more than 1 item.
  #ifdef USE_MOVING_AVG
  PM_A_Buf[bufIndex] = PM_A_Count;
  PM_B_Buf[bufIndex] = PM_B_Count;
  PM_C_Buf[bufIndex] = PM_C_Count;
  bufIndex++;
  if ( bufIndex >= N ) {
    bufIndex = 0;
    if ( !isBufferFull )
      isBufferFull = true;
  }
  if ( isBufferFull || bufIndex > 1 ) {
    float sumA = 0;
    float sumB = 0;
    float sumC = 0;
    int numItems = N; // Number of items to average.
    if ( !isBufferFull )
      numItems = bufIndex;
    for(int i = 0; i < numItems; i++) {
      sumA += PM_A_Buf[i];
      sumB += PM_B_Buf[i];
      sumC += PM_C_Buf[i];
    }
    PM_A_Count = (unsigned int)(sumA / numItems);
    PM_B_Count = (unsigned int)(sumB / numItems);
    PM_C_Count = (unsigned int)(sumC / numItems);

    // Print avg particle counts.
    #ifdef PRINT_AVG_COUNTS
    Serial.print("AVG COUNTS....");
    printValue("PM_A_Avg", PM_A_Count);
    printValue("PM_B_Avg", PM_B_Count);
    printValue("PM_C_Avg", PM_C_Count, true);
    Serial.println("");
    #endif // PRINT_AVG_COUNTS
  }
  #endif // USE_MOVING_AVG

  // Calculate dust density (dust or mass concentration) in units of ug/m3.
  float PM_A_Density = getDustDensity(ATMOS, PM_SIZE_A, PM_A_Count);
  float PM_C_Density = getDustDensity(ATMOS, PM_SIZE_C, PM_C_Count);
  float PM_B_Density = PM_C_Density - PM_A_Density;
  Serial.print("DUST DENSITY..");
  printFValue("PM_A_Density", PM_A_Density, "ug/m3");
  printFValue("PM_B_Density", PM_B_Density, "ug/m3");
  printFValue("PM_C_Density", PM_C_Density, "ug/m3", true);
  Serial.println("");

} // END PROGRAM
