/////////////////////////////////////////////////////////////////////////////
// Sharp GP2Y1023AU0F Dust Sensor Demo
//
// Board Connection:
//   GP2Y1023AU0F      Arduino
//   Pin 1 - Vo        Pin 7
//   Pin 2 - LED-GND   C1(-) and GND
//   Pin 3 - GND       GND
//   Pin 4 - VLED      C1(+) and R1
//   Pin 5 - Vcc       5V
//
//   C1 = 220 uF capacitor
//   R1 = 150 ohm resistor
//
// Serial monitor setting:
//   9600 baud
//
// References:
//   https://www.mouser.com/datasheet/2/365/GP2Y1023AU0F_spec_ED14G011A-1288370.pdf
//   https://github.com/sharpsensoruser/sharp-sensor-demos/blob/master/docs/Sharp%20GP2Y1010AU0F%20-%20Dust%20Density%20Conversion.pdf
/////////////////////////////////////////////////////////////////////////////

// Arduino pin numbers.
const int pwmInputPin = 7;   // Arduino digital pin 7 connect to sensor Vo.

// Set the typical output voltage in microseconds when there is zero dust. 
unsigned long Voc = 1400;

// Use the typical sensitivity in units of microseconds per 100ug/m3.
const float K = 1400;
  
/////////////////////////////////////////////////////////////////////////////

// Helper functions to print a data value to the serial monitor.
void printValue(String text, unsigned long value, bool isLast = false) {
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
  // Set pin for input.
  pinMode(pwmInputPin, INPUT);
  
  // Start the hardware serial port for the serial monitor.
  Serial.begin(9600);
  
  // Wait two seconds for startup.
  delay(2000);
  Serial.println("");
  Serial.println("GP2Y1023AU0F Demo");
  Serial.println("=================");
}

// Arduino main loop.
void loop() {  
  // Get the pulse width in microseconds. This is the Vo reading.
  unsigned long timeoutUS = 15000;
  unsigned long Vo = pulseIn(pwmInputPin, LOW, timeoutUS);
  if ( Vo == 0 )
    return;
  printValue("Vo", Vo);

  // Offset adjustment.  
  if ( Vo < Voc )
    Voc = Vo;
  printValue("Voc", Voc);
  
  // Convert to Dust Density in units of ug/m3. Note that GP2Y1023 model
  // is high sensitivity device and densities beyond 250 ug/m3 are not
  // within the usable range.
  float dustDensity = (Vo - Voc) / K * 100.0;
  printFValue("DustDensity", dustDensity, "ug/m3", true);
  Serial.println("");

  delay(500);
  
} // END PROGRAM
