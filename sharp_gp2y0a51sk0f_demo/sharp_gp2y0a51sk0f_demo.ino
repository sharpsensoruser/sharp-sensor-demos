/////////////////////////////////////////////////////////////////////////////
// Sharp GP2Y0A51SK0F Distance Measuring Sensor Demo
//
// Board Connection:
//   Arduino   GP2Y0A51SK0F
//   5V        Pin 1 - Vcc
//   GND       Pin 2 - GND
//   A0        Pin 3 - Vo
//
// Serial monitor setting:
//   9600 baud
/////////////////////////////////////////////////////////////////////////////

// Arduino pin numbers.
const int sharpVoPin = A0;   // Arduino analog pin 0 connect to sensor Vo.

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
  Serial.print(value, 3);
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
  Serial.println("GP2Y0A51SK0F Demo");
  Serial.println("=================");

  // Wait at least 25.2ms for the first measurement to complete.
  delay(26);
}

// Arduino main loop.
void loop() {
  // Wait 100ms between readings.
  delay(100);
  
  // Read the output voltage.
  int VoRaw = analogRead(sharpVoPin);
  printValue("VoRaw", VoRaw);

  // Compute the output voltage in Volts.
  float Vo = (5.0 * VoRaw) / 1024.0;
  printFValue("Vo", Vo, "V");

  // Convert to Distance in units of mm
  // by approximating datasheet graph
  // using equation of form: y = a/x + b
  // and two (x,y) points on the graph:
  // (30mm, 1.68V) and (150mm, 0.39V)
  const float a = 48.375;
  const float b = 0.0675;
  float dist = 0;
  if ( Vo > b ) {
    dist = a / (Vo - b);
  }
  printFValue("Distance", dist, "mm", true);
  Serial.println("");
  
} // END PROGRAM
