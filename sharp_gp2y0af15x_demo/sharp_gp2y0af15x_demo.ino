/////////////////////////////////////////////////////////////////////////////
// Sharp GP2Y0AF15X Distance Measuring Sensor Demo
//
// Board Connection:
//   Arduino   GP2Y0AF15X
//   A0        Vo
//   GND       GND
//   5V        Vcc
//
// Serial monitor setting:
//   9600 baud
/////////////////////////////////////////////////////////////////////////////

// Choose program options.
//#define PRINT_RAW_DATA
#define USE_AVG

// Arduino pin numbers.
const int sharpVoPin = A0;   // Arduino analog pin 5 connect to sensor Vo.
  
/////////////////////////////////////////////////////////////////////////////

// Helper functions to print a data value to the serial monitor.
void printValue(String text, unsigned int value, bool isLast = false) {
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

/////////////////////////////////////////////////////////////////////////////

// Arduino setup function.
void setup() {  
  // Start the hardware serial port for the serial monitor.
  Serial.begin(9600);
  Serial.println("");
  Serial.println("GP2Y0AF15X Demo");
  Serial.println("===============");
  
  // Wait 25.2ms for stable measurements.
  delayMicroseconds(25200);
}

// Arduino main loop.
void loop() {  
  // Read the output voltage. This operation takes around 100 microseconds.
  int VoRaw = analogRead(sharpVoPin);

  // Compute the output voltage in Volts.
  float Vo = Vo / 1024.0 * 5.0;
  printFValue("Vo", Vo*1000.0, "mV");

  // Convert to Distance in units of mm.
  const float a = 54.375;
  const float b = 0.0275;
  float dist = 0;
  if ( Vo > b ) {
    dist = a / (Vo - b);
  }
  printFValue("Distance", dist, "mm", true);
  Serial.println("");
  
} // END PROGRAM

