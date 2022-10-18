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
// Port 15

void setup() {
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
  vl(true, 0);
  vr(true, 0);
  hl(true, 0);
  hr(true, 0);
}

void forward(){ 
  vl(true, 200);
  hl(true, 200);
  vr(true, 200);
  hr(true, 200);
} 

//Räder einzeln ansteuern:

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

void seitlichLinks(int starke)
{
  vr(true, starke);
  vl(false, starke);
  hr(false, starke);
  hl(true, starke);
}

void ansteuern(float Y,float X)
{
  
  
    float starke = ((80/51*(-1))*Y+200); // Funktion gibt bei Y = 127.5 Null zurück, bei Y = 255 -200 und bei Y = 0 200
    
    Serial.println(starke);
    vr(false, starke);
    vl(false, starke);
    hr(false, starke);
    hl(false, starke);
  }
  
  

void loop() 
{
  ansteuern(255,0);
 
}