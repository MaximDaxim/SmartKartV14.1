#include <PS2X_lib.h>  //for v1.6
#include <Servo.h>


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
void ansteuern(float RY, float LX, float LY) { //Die große Methode zum fahren
  float starkeY;
  float starkeX;
  starkeY = map(RY, 0, 255, 200, -200); //Mappe den Wert des rechten Controllsticks auf eine Stärke zwischen -200 und 200

  
  if(modus == "Hanf")
  {
    myTimePlus = millis() + 5000;
    item = 0;
  }
  else if (modus == "Pilz")
  {
    myTimePlus = millis() + 2000;
    item = 1;
  }
  else if (modus == "Rakete")
  {
    myTimePlus = millis() + 7000;
    item = 2; 
  }
  else if (modus == "Wasser")
  {
    myTimePlus = millis() + 1000;
    while(myTime < myTimePlus)
    {
     vl(200);
     hr(200);
     myTime = millis();
    }
  }
  else if (modus == "Banane")
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



      if (LX > 140 && LY > 140) //vorne links
      {

        vr(-200);
        hl(-200);
      }
      if (LX < 118 && LY > 140) // vorne rechts
      {
        vl(-200);
        hr(-200);

      }
      if (LX < 118 && LY < 118) // hinten rechts
      {

        vr(200);
        hl(200);
      }
      if (LX > 140 && LY < 118) // hinten links
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

  if (type == 2) { //Guitar Hero Controller
    ps2x.read_gamepad();          //read controller

  }
  else { //DualShock Controller
    ps2x.read_gamepad(false, vibrate); //read controller and set large motor to spin at 'vibrate' speed

    //Unsere Methoden

    //if(ps2x.ButtonPressed(RED_FRET))
    //{
    //}
    
    servo();
    ansteuern(ps2x.Analog(PSS_RY), ps2x.Analog(PSS_LX), ps2x.Analog(PSS_LY));





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
