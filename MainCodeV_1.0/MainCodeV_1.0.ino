#include <PS2X_lib.h>  //for v1.6

/******************************************************************
 * set pins connected to PS2 controller:
 *   - 1e column: original 
 *   - 2e colmun: Stef?
 * replace pin numbers by the ones you use
 ******************************************************************/
#define PS2_DAT        8  //14    
#define PS2_CMD        A15  //15
#define PS2_SEL        A14  //16
#define PS2_CLK        7  //17
//define the L298n IO pin
#define ENA 5
#define ENB 6
#define IN1 7
#define IN2 8
#define IN3 9
#define IN4 11
#define ENC 3
#define END 10
#define IN5 13
#define IN6 4
#define IN7 1
#define IN8 2
/******************************************************************
 * select modes of PS2 controller:
 *   - pressures = analog reading of push-butttons 
 *   - rumble    = motor rumbling
 * uncomment 1 of the lines for each mode selection
 ******************************************************************/
//#define pressures   true
#define pressures   false
//#define rumble      true
#define rumble      false
int stickVorne = PSS_LY;

PS2X ps2x; // create PS2 Controller Class

//right now, the library does NOT support hot pluggable controllers, meaning 
//you must always either restart your Arduino after you connect the controller, 
//or call config_gamepad(pins) again after connecting the controller.

int error = 0;
byte type = 0;
byte vibrate = 0;

void setup(){
  Serial.begin(9600);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(IN5, OUTPUT);
  pinMode(IN6, OUTPUT);
  pinMode(IN7, OUTPUT);
  pinMode(IN8, OUTPUT);
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(ENC, OUTPUT);
  pinMode(END, OUTPUT);
  digitalWrite(ENA, HIGH);  
  digitalWrite(ENB, HIGH);
  digitalWrite(ENC, HIGH);  
  digitalWrite(END, HIGH);
 
  Serial.begin(57600);
  
  delay(300);  //added delay to give wireless ps2 module some time to startup, before configuring it
   
  //CHANGES for v1.6 HERE!!! **************PAY ATTENTION*************
  
  //setup pins and settings: GamePad(clock, command, attention, data, Pressures?, Rumble?) check for error
  error = ps2x.config_gamepad(PS2_CLK, PS2_CMD, PS2_SEL, PS2_DAT, pressures, rumble);
  
  if(error == 0){
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
  else if(error == 1)
    Serial.println("No controller found, check wiring, see readme.txt to enable debug. visit www.billporter.info for troubleshooting tips");
   
  else if(error == 2)
    Serial.println("Controller found but not accepting commands. see readme.txt to enable debug. Visit www.billporter.info for troubleshooting tips");

  else if(error == 3)
    Serial.println("Controller refusing to enter Pressures mode, may not support it. ");
  
//  Serial.print(ps2x.Analog(1), HEX);
  
  type = ps2x.readType(); 
  switch(type) {
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

void hl(boolean Richtung, int Starke)
{
  analogWrite(ENA,Starke);
  if(Richtung == true)
  {
    digitalWrite(IN1,true);
    digitalWrite(IN2,false);
  }else{
    digitalWrite(IN1,false);
    digitalWrite(IN2,true);
  }
}

void hr(boolean Richtung, int Starke)
{
  analogWrite(ENB,Starke);
  if(Richtung == true)
  {
    digitalWrite(IN5,false);
    digitalWrite(IN6,true);
  }else{
    digitalWrite(IN5,true);
    digitalWrite(IN6,false);
  }
}

void vl(boolean Richtung, int Starke)
{
  analogWrite(ENC,Starke);
  if(Richtung == true)
  {
    digitalWrite(IN3,false);
    digitalWrite(IN4,true);
  }else{
    digitalWrite(IN3,true);
    digitalWrite(IN4,false);
  }
}

void vr(boolean Richtung, int Starke)
{
  analogWrite(END,Starke);
  if(Richtung == true)
  {
    digitalWrite(IN7,true);
    digitalWrite(IN8,false);
  }else{
    digitalWrite(IN7,false);
    digitalWrite(IN8,true);
  }
}

void ansteuern(float Y,float X)
{
  
  if (Y > 140)
  {
    float starke = (32.0/2601.0)*(Y*Y)-(160.0/51.0)*Y + 200.0;
    
    Serial.println(starke);
    vr(false, starke);
    vl(false, starke);
    hr(false, starke);
    hl(false, starke);
  }
  if (Y < 114)
  {
    float starke = (32.0/2601.0)*(Y*Y)-(160.0/51.0)*Y + 200.0;

    Serial.println(starke);
    vr(true, starke);
    vl(true, starke);
    hr(true, starke);
    hl(true, starke);
  }


  if(Y < 140 && Y > 114)
  {
   Serial.println(0);
    vr(false, 0);
    vl(false, 0);
    hr(false, 0);
    hl(false, 0); 
  }
}
void loop() {
  /* You must Read Gamepad to get new values and set vibration values
     ps2x.read_gamepad(small motor on/off, larger motor strenght from 0-255)
     if you don't enable the rumble, use ps2x.read_gamepad(); with no values
     You should call this at least once a second
   */  
  if(error == 1) //skip loop if no controller found
    return; 
  
  if(type == 2){ //Guitar Hero Controller
    ps2x.read_gamepad();          //read controller 
   
  }
  else { //DualShock Controller
    ps2x.read_gamepad(false, vibrate); //read controller and set large motor to spin at 'vibrate' speed

    ansteuern(ps2x.Analog(PSS_LY),0);

    if(ps2x.Button(PSB_L1) || ps2x.Button(PSB_R1)) { //print stick values if either is TRUE
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
