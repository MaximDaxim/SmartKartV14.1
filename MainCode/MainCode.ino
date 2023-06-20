#include <PS2X_lib.h>  //for v1.6
#include <Servo.h>
#include <FastLED.h>
#include <Wire.h>
#include "Adafruit_TCS34725.h"

/******************************************************************
  set pins connected to PS2 controller:
    - 1e column: original
    - 2e colmun: Stef?
  replace pin numbers by the ones you use
******************************************************************/
Servo myservo;         // Servo wird initialisiert
#define PS2_DAT        22  //14 alter Anschluss   
#define PS2_CMD        A9  //15 alter Anschluss
#define PS2_SEL        A10  //16 alter Anschluss
#define PS2_CLK        12  //17 alter Anschluss
//define the L298n IO pin
#define ENA 5 //Hinten Links
#define ENB 6 //Hinten Rechts
#define IN1 7 //Hinten Links
#define IN2 8 //Hinten Links
#define IN3 9 //Hinten Rechts
#define IN4 11 //Hinten Rechts
#define ENC A15 //Hinten Rechts
#define END 10 //Vorne Rechts
#define IN5 13 //Vorne Links
#define IN6 4 //Vorne Links
#define IN7 50 //Vorne Rechts
#define IN8 2 //Vorne Rechts


Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_2_4MS, TCS34725_GAIN_1X);
int a = 1;
#define SCL   SCL21    
#define SDA   SDA20
#define Pin_1 49
#define Pin_2 51
#define Pin_3 53
#define NUM_LEDS_1_STRIP 29
#define NUM_LEDS_2_STRIP 14
#define NUM_LEDS_3_STRIP 8
String color = "";

CRGB leds[NUM_LEDS_1_STRIP + NUM_LEDS_2_STRIP + NUM_LEDS_3_STRIP];

/******************************************************************
  select modes of PS2 controller:
    - pressures = analog reading of push-butttons
    - rumble    = motor rumbling (Controller Vibrations)
  uncomment 1 of the lines for each mode selection
******************************************************************/
#define pressures   false
#define rumble      false
int stickVorne = PSS_LY;



PS2X ps2x; // create PS2 Controller Class



//right now, the library does NOT support hot pluggable controllers, meaning
//you must always either restart your Arduino after you connect the controller,
//or call config_gamepad(pins) again after connecting the controller.



int error = 0;
byte type = 0;
byte vibrate = 0;
int pos = 90; //Gibt dem Servo des Panzerturms die Richtung an
unsigned long myTime = 0;
unsigned long myTimePlus = 0;
String modus = "std";
int item = 0;

void setup() {
  Serial.begin(9600);
  myservo.attach(3);         //Dem Arduino wird gesagt wo der Servo (Pin 3) ist
  pinMode(IN1, OUTPUT); //Hinten Links
  pinMode(IN2, OUTPUT); //Hinten Links
  pinMode(IN3, OUTPUT); //Hinten Rechts
  pinMode(IN4, OUTPUT); //Hinten Rechts
  pinMode(IN5, OUTPUT); //Vorne Links
  pinMode(IN6, OUTPUT); //Vorne Links
  pinMode(IN7, OUTPUT); //Vorne Rechts
  pinMode(IN8, OUTPUT); //Vorne Rechts
  pinMode(ENA, OUTPUT); //Hinten Links
  pinMode(ENB, OUTPUT); //Hinten Rechts
  pinMode(ENC, OUTPUT); //Vorne Links
  pinMode(END, OUTPUT);
  digitalWrite(ENA, HIGH); //Hinten Links
  digitalWrite(ENB, HIGH); //Hinten Rechts
  digitalWrite(ENC, HIGH); //Vorne Links
  digitalWrite(END, HIGH); //Vorne Rechts

  Serial.begin(57600);

  delay(300);  //added delay to give wireless ps2 module some time to startup, before configuring it

  //CHANGES for v1.6 HERE!!! **************PAY ATTENTION*************

  //setup pins and settings: GamePad(clock, command, attention, data, Pressures?, Rumble?) check for error
  error = ps2x.config_gamepad(PS2_CLK, PS2_CMD, PS2_SEL, PS2_DAT, pressures, rumble);

  if (error == 0) {
    Serial.print("Found Controller, configured successful ");
    Serial.print("pressures = ");
    if (pressures)
      Serial.println("true ");
    else
      Serial.println("false");
    Serial.print("rumble = ");
    if (rumble)
      Serial.println("true)");
    else
      Serial.println("false");
    Serial.println("Try out all the buttons, X will vibrate the controller, faster as you press harder;");
    Serial.println("holding L1 or R1 will print out the analog stick values.");
    Serial.println("Note: Go to www.billporter.info for updates and to report bugs.");
  }
  else if (error == 1)
    Serial.println("No controller found, check wiring, see readme.txt to enable debug. visit www.billporter.info for troubleshooting tips");

  else if (error == 2)
    Serial.println("Controller found but not accepting commands. see readme.txt to enable debug. Visit www.billporter.info for troubleshooting tips");



  else if (error == 3)
    Serial.println("Controller refusing to enter Pressures mode, may not support it. ");

  //  Serial.print(ps2x.Analog(1), HEX);

  type = ps2x.readType();
  switch (type) {
    case 0:
      Serial.print("Unknown Controller type found ");
      break;
    case 1:
      Serial.print("DualShock Controller found ");
      break;
    case 2:
      Serial.print("GuitarHero Controller found ");
      break;
    case 3:
      Serial.print("Wireless Sony DualShock Controller found ");
      break;



      if (tcs.begin()) {
    Serial.println("Found sensor");
  } else {
    Serial.println("No TCS34725 found ... check your connections");
    while (1);
}
  // tell FastLED there's 60 NEOPIXEL leds on pin 2, starting at index 0 in the led array
  FastLED.addLeds<NEOPIXEL, Pin_1>(leds, 0, NUM_LEDS_1_STRIP);

  // tell FastLED there's 60 NEOPIXEL leds on pin 3, starting at index 60 in the led array
  FastLED.addLeds<NEOPIXEL, Pin_2>(leds, NUM_LEDS_1_STRIP, NUM_LEDS_2_STRIP);
  
  // tell FastLED there's 60 NEOPIXEL leds on pin 3, starting at index 60 in the led array
  FastLED.addLeds<NEOPIXEL, Pin_3>(leds, NUM_LEDS_1_STRIP + NUM_LEDS_2_STRIP, NUM_LEDS_3_STRIP);

int UnterbodenHinten[] = {0, 13};
int Ruecklicht[] = {14, 28};
int UnterbodenVorne[] = {29, 42};
int AugeRechts[] = {43, 46};
int AugeLinks[] = {47, 50};
  }
}

void schnell()
{
  for(int dot = 0; dot < 14; dot++)//UnterbodenHinten
  {
    leds[dot] =  0xFF0000;
    FastLED.setBrightness(50);
    FastLED.show(); 
  }
  for(int dot = 14; dot < 29; dot++)//Ruecklicht
  {
    leds[dot] =  0xFFD700;
    FastLED.setBrightness(50);
    FastLED.show(); 
  }
  for(int dot = 29; dot < 43; dot++)//Ruecklicht
  {
    leds[dot] =  CRGB::Black;
    FastLED.setBrightness(50);
    FastLED.show(); 
  }  
  leds[43] = 0xFF0000;
  leds[44] = CRGB::Black;
  leds[45] = CRGB::Black;
  leds[46] = 0xFF0000;
  leds[47] = 0xFF0000;
  leds[48] = CRGB::Black;
  leds[49] = CRGB::Black;
  leds[50] = 0xFF0000; 
}

void goldPilz()
{
  for(int dot = 0; dot < 14; dot++)//UnterbodenHinten
  {
    leds[dot] =  0xFF0000;
    FastLED.setBrightness(50);
    FastLED.show(); 
  }
  for(int dot = 14; dot < 29; dot++)//Ruecklicht
  {
    leds[dot] =  0xFFD700;
    FastLED.setBrightness(50);
    FastLED.show(); 
  }
  for(int dot = 29; dot < 43; dot++)//UnterbodenVorne
  {
    leds[dot] =  0xFFD700;
    FastLED.setBrightness(50);
    FastLED.show(); 
  }  
  leds[43] = 0xFF0000;
  leds[44] = 0xFFD700;
  leds[45] = 0xFFD700;
  leds[46] = 0xFF0000;
  leds[47] = 0xFF0000;
  leds[48] = 0xFFD700;
  leds[49] = 0xFFD700;
  leds[50] = 0xFF0000; 
}

void pfuetze()
{
  for(int dot = 0; dot < 51; dot++)
  {
    leds[dot] =  CRGB::Blue;
    FastLED.setBrightness(25);
    FastLED.show(); 
  }  
}

void banane()
{
  if(color == "Yellow"){
  for(int dot = 0; dot < 51; dot++)
  {
    leds[dot] = 0x000000;
    FastLED.show();
  }  
  }
}

void hanfBlatt()
{
  for(int dot = 0; dot <= 42; dot++)
  {
    leds[dot] = 0xADFF2F;
    FastLED.setBrightness(50);
    FastLED.show();
  } 
  for(int dot = 43; dot < 51; dot++)
  {
    leds[dot] = 0xFF0000;
    FastLED.setBrightness(50);
    FastLED.show();
  }   
}

void stern()
{ 
  if(color == "Orange"){  
   for(int i = 0; i < 51; i++)
   {
            int dot = i; 
            leds[dot] = CRGB(random(255), random(255), random(255));
            FastLED.setBrightness(50);
            FastLED.show();
            if(dot == 51)
            {
              break;  
            }
   }
  }
        
}

void rakete()
{
   for(int dot = 14; dot < 28; dot++)
   {
      leds[dot] =  0xFF4500;
      FastLED.setBrightness(100);
      FastLED.show();
   } 
}

void standard()
{
  for(int dot = 0; dot < 51; dot++)
  {
    leds[dot] = CRGB::White;
    FastLED.setBrightness(50);
    FastLED.show();  
  }  
}







//Methoden um die Räder einzelnd anzusteuern
//Jeder Motor benötigt zwei Pins, sie bestimmen die Drehrichtung

void hl(int starke) //Hinten Links
{
  analogWrite(ENA, abs(starke)); //Stell die Stärke ein. Der Bereich liegt zwischen -255 bis 255

  if (starke > 0) //Wenn die Stärke positiv ist dann werden die Pins so angesteuert, sodass sie vorwärts fahren
  {
    digitalWrite(IN1, true);
    digitalWrite(IN2, false);
  }
  else if (starke < 0) //Wenn die Stärke negativ ist dann werden die Pins so angesteuert, sodass sie rückwärts fahren
  {
    digitalWrite(IN1, false);
    digitalWrite(IN2, true);
  }
}





void vl(int starke) // Vorne Links
{
  analogWrite(ENC, abs(starke));

  if (starke > 0)
  {
    digitalWrite(IN5, false);
    digitalWrite(IN6, true);
  }
  else if (starke < 0)
  {
    digitalWrite(IN5, true);
    digitalWrite(IN6, false);
  }
}





void hr(int starke) //Hinten Rechts
{
  analogWrite(ENB, abs(starke));

  if (starke > 0)
  {
    digitalWrite(IN3, false);
    digitalWrite(IN4, true);
  }
  else if (starke < 0)
  {
    digitalWrite(IN3, true);
    digitalWrite(IN4, false);
  }
}





void vr(int starke) //Vorne Rechts
{
  analogWrite(END, abs(starke));

  if (starke > 0)
  {
    digitalWrite(IN7, true);
    digitalWrite(IN8, false);
  }
  else if (starke < 0)
  {
    digitalWrite(IN7, false);
    digitalWrite(IN8, true);
  }
}


void servo() //Servo Ansteuerung
{
  //Die Schultertasten sind die Tasten hinter den Steuerungskreuzen bezeichnet mit L und R
  if (ps2x.Button(PSB_L2)) //Wenn die linke hinter Schultertaste gedrückt wird (2)
  {
    pos++;
    if (pos == 181) {
      pos = 180; //Der Berich liegt bei 0 bis 180
    }
    myservo.write(pos);   //Ändert die Position um + 1, falls der Wert auf 181 ist dann setzte ihn auf 180
  }
  else if (ps2x.Button(PSB_R2)) //Wenn die rechte hintere Schultertaste gedrückt wird (2)
  {
    pos--;
    if (pos <= -1) {
      pos = 0; //Der Berich liegt bei 0 bis 180
    }
    myservo.write(pos); //Änder die Position um - 1, falls der Wert auf -1 ist dann setzte ihn auf 0
  }
}


//RY = Rechter Joystick Y-Achse (Geschwindigkeit); LX = Linker Joystick X-Achse; LY = Linker Joystick Y-Achse
void ansteuern(float RY, float LX, float LY) { //Die große Methode zum Fahren
  float starkeY;
  float starkeX;
  starkeY = map(RY, 0, 255, 200, -200); //Mappe den Wert des rechten Controllsticks auf eine Stärke zwischen -200 und 200

  //Items werden gecheckt
  if(modus == "Hanf")//Invertierung fünf Sekunden
  {
    myTimePlus = millis() + 5000;
    item = 0;
    modus = "std";
  }
  else if (modus == "Pilz")//Fährt für zwei Sekunden sehr viel schneller
  {
    myTimePlus = millis() + 2000;
    item = 1;
    modus = "std";
  }
  else if (modus == "Rakete")//Fährt für sieben Sekunden schneller
  {
    myTimePlus = millis() + 7000;
    item = 2;
    modus = "std";
  }
  else if (modus == "Wasser")//Lässt den Wagen schräg fahren für eine Sekunde
  {
    myTimePlus = millis() + 1000;
    while(myTime < myTimePlus)
    {
     vl(200);
     hr(200);
     myTime = millis();
     modus = "std";
    }
  }
  else if (modus == "Banane")//Dreht das Auto um 360 Grad
  {
    vl(250);  //Dreht sich einmal um die eigene Achse
    hl(250);
    vr(-250);
    hr(-250);
    delay(1050);
    
    vl(200);    //Fährt für eine Sekunde nach vorne
    hl(200);
    vr(200);
    hr(200);
    delay(1000);
    modus = "std";
  }

  if(myTimePlus > millis() && item == 0)//Hanf
  {
    starkeY = starkeY * -1; 
  }
  if(myTimePlus > millis() && item == 1)//Pilz
  {
    starkeY = starkeY + 50;
  }
  if(myTimePlus > millis() && item == 2)//Rakete 
  {
    starkeY = starkeY + 20;
  }
  if(millis() > myTimePlus)
  {
    modus = "std";
  }
  Serial.print("Aktuell: ");
  Serial.print(millis());
  Serial.print("       Plus: ");
  Serial.println(myTimePlus);


  if (RY > 118 && RY < 140) //Es wird geguckt ob das Auto hovert oder schräg fahren soll
  {
    if (LY < 140 && LY > 118) //Es wird geguckt ob es hovert
    {

      starkeX = map(LX, 0, 255, 200, -200); //Der linke Controllstick wird gemappt um zu hovern
      vl(-starkeX);
      vr(starkeX);
      hl(starkeX);
      hr(-starkeX);
    }
    if (LY > 140 || LY < 118) //Es wird geguckt ob er schräg fährt
    {



      if (LX > 140 && LY > 140) //Fährt nach vorne links
      {

        vr(-200);
        hl(-200);
      }
      if (LX < 118 && LY > 140) //Fährt nach vorne rechts
      {
        vl(-200);
        hr(-200);

      }
      if (LX < 118 && LY < 118) //Fährt nach hinten rechts
      {

        vr(200);
        hl(200);
      }
      if (LX > 140 && LY < 118) //Fährt nach hinten links
      {
        vl(200);
        hr(200);
      }


    }


  }


  else if (140 > LX)                 //Linkskurve
  {
    starkeX = starkeY * (map(LX, 0, 127, 0, 100) / 100.00);
    vr(starkeY);
    hr(starkeY);
    vl(starkeX);
    hl(starkeX);
    //Serial.println(starkeX);
    //Serial.println(starkeY);
  }
  else if (118 < LX)                 //Rechtskurve
  {
    starkeX = starkeY * (map(LX, 255, 127, 0, 100) / 100.00);
    vr(starkeX);
    hr(starkeX);
    vl(starkeY);
    hl(starkeY);
    //Serial.println(starkeX);
    //Serial.println(starkeY);
    //Serial.println("UWE");
  }
  else if (118 < LX && 140 > LX)           // Nach vorne/hinten
  {
    vr(starkeY);
    hr(starkeY);
    vl(starkeY);
    hl(starkeY);
    //Serial.println(starkeY);
  }
}






void loop() {

  /* You must Read Gamepad to get new values and set vibration values
     ps2x.read_gamepad(small motor on/off, larger motor strenght from 0-255)
     if you don't enable the rumble, use ps2x.read_gamepad(); with no values
     You should call this at least once a second
  */
  if (error == 1) //skip loop if no controller found
    return;

  if (type == 2) { //Guitar Hero Controller (unötig, da wir keinen haben)
    ps2x.read_gamepad();          //read controller

  }
  else { //DualShock Controller
    ps2x.read_gamepad(false, vibrate); //read controller and set large motor to spin at 'vibrate' speed

    //Unsere Methoden

    //if(ps2x.ButtonPressed(RED_FRET))
    //{
    //}
    
    servo();//Bestimmmt die Servo Steuerung
    ansteuern(ps2x.Analog(PSS_RY), ps2x.Analog(PSS_LX), ps2x.Analog(PSS_LY));//Gib die Controllerwerte an die ansteuern Methode weiter, damit die Werte gemappt werden können





float red, green, blue;
  tcs.getRGB(&red, &green, &blue);
 
  
 if(color != "NULL"){
  Serial.print("R:\t"); Serial.print(int(red)); 
  Serial.print("\tG:\t"); Serial.print(int(green)); 
  Serial.print("\tB:\t"); Serial.print(int(blue)); Serial.print(color);
  Serial.print("\n");
 }
 if ((red > 120 && red < 160) && (green > 35&& green < 65) && (blue > 35 && blue < 55)) {
color = "Red";
//Serial.println(color);
schnell();
modus = "Pilz";
}
else if ((red > 65 && red < 80) && (green > 90 && green < 110) && (blue > 50 && blue < 80)) {
color = "Green";
//Serial.println(color);
hanfBlatt();
modus = "Hanf";
}
else if ((red > 35  && red < 50) && (green > 75 && green < 95) && (blue > 100 && blue < 120)) {
color = "Blue";
//Serial.println(color);
pfuetze();
modus = "Wasser";
}
else if ((red > 100 && red < 110 ) && (green > 90 && green < 100) && (blue > 35 && blue < 45)) {
color = "Yellow";
//Serial.println(color);
banane();
modus = "Banane";
}
else if ((red > 90 && red < 130 ) && (green > 79 && green < 95) && (blue > 35 && blue < 70)) {
  
color = "Orange";
//Serial.println(color);
stern();
modus = "Rakete";
}
 
else {
  color = "NULL";
  //Serial.println(color);
  standard();
  
  
}










    if (ps2x.Button(PSB_L1) || ps2x.Button(PSB_R1)) { //print stick values if either is TRUE
      Serial.print("Stick Values:");
      Serial.print(ps2x.Analog(PSS_LY), DEC); //Left stick, Y axis. Other options: LX, RY, RX
      Serial.print(",");
      Serial.print(ps2x.Analog(PSS_LX), DEC);
      Serial.print(",");
      Serial.print(ps2x.Analog(PSS_RY), DEC);
      Serial.print(",");
      Serial.println(ps2x.Analog(PSS_RX), DEC);
    }

  }
  delay(50);
}



/*
   starkeX = map(X, 0, 255, 200, -200);
   if(118>X || 140<X)
   {
   vl(starkeX);
   vr(starkeX*(-1));
   hl(starkeX*(-1));
   hr(starkeX);
   }
*/
