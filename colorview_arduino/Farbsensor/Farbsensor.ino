
#include <Wire.h>
#include "Adafruit_TCS34725.h"

  // Connect VDD    to 3.3V DC
   //Connect GROUND to common ground */
/* Initialise with default values (int time = 2.4ms, gain = 1x) */
// Adafruit_TCS34725 tcs = Adafruit_TCS34725();
#define SCL   SCL21    
#define SDA   SDA20
String color = ""; //reading 

/* Initialise with specific int time and gain values */
Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_614MS, TCS34725_GAIN_1X);

void setup(void) {
  Serial.begin(9600);

  if (tcs.begin()) {
    Serial.println("Found sensor");
  } else {
    Serial.println("No TCS34725 found ... check your connections");
    while (1);

    
}
  // Now we're ready to get readings!
}

void loop(void) {
   float red, green, blue;
  tcs.getRGB(&red, &green, &blue);

  Serial.print("R:\t"); Serial.print(int(red)); 
  Serial.print("\tG:\t"); Serial.print(int(green)); 
  Serial.print("\tB:\t"); Serial.print(int(blue));
  Serial.println(color);
  
  if ((red > 170) && (green < 55) && (blue < 45)) {
color = "red";
}
else if ((red < green) && (green > 100) && (blue < 34)) {
color = "green";
}
else if ((red < blue) && (green < blue) && (blue > 100)) {
color = "blue";
}
else if ((red < 110 ) && (green < 100) && (blue < 70 )) {
color = "yellow";
}
else if ((red < 170 ) && (green > 45) && (blue < 40  )) {
color = "orange";
}
else if ((red < 140) && (green > 50) && (blue < 35)) {
color = "purple";
}
 Serial.print("\n");

}
