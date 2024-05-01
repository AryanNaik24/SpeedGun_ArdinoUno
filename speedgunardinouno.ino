#include<LiquidCrystal.h>
#include<math.h> // Include math library for ceil() function
LiquidCrystal lcd(2,3,4,5,6,7); // change and ass custom ports here for the lcd

#define echo A0
#define trigger A1

#define Recheck A5 

#define G_LED 8
#define R_LED 9
#define buz 10

float spd=0,dist=0,Time=0;
int flag1=0,flag2=0;
int time1=0,time2=0;

const unsigned int BEEP_FREQUENCY=3000;

void setup() {
  // put your setup code here, to run once:
  pinMode(Recheck,INPUT_PULLUP);
  pinMode(echo,INPUT);
  pinMode(trigger,OUTPUT);
  pinMode(R_LED,OUTPUT);
  pinMode(G_LED,OUTPUT);
  pinMode(buz,OUTPUT);
  lcd.begin(16,2);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Vroom ");
  lcd.setCursor(0,1);
  lcd.print("Measurement ");
  delay(2000);
  lcd.clear();

  Serial.begin(9600); // Initialize serial communication
}

void loop() {
  // put your main code here, to run repeatedly:
  usRead();

  if(dist<=30 && flag1==0){time1=millis(); flag1=1;}
  if(dist<=20 && flag2==0){time2=millis(); flag2=1;}

  if(flag1==1 && flag2==1){
    flag1=2; flag2=2;
    if(time1>time2){
      Time=time1-time2;
    }
    else if(time2>time1){
      Time=time2-time1;
    }
    Time=Time/1000; //convert to seconds
    spd=(10/Time);
    spd=spd*3600; //sec pr hr 
    spd=spd/1000;
    spd = ceil(spd); // Round up the speed to the nearest integer

    digitalWrite(R_LED,0);
    digitalWrite(G_LED,1);
    tone(buz,BEEP_FREQUENCY,200);
  }
  if(digitalRead(Recheck)==0){
    spd=0;
    flag1=0;
    flag2=0;
    digitalWrite(R_LED,1);
    digitalWrite(G_LED,0);
    tone(buz,BEEP_FREQUENCY,400);
  }

  lcd.setCursor(0,0);
  lcd.print("Distance : ");
  lcd.print(dist);
  lcd.print("   ");

  lcd.setCursor(0,1);
  lcd.print("Speed :");
  lcd.print(spd);
  delay(10);

  // Print speed and distance on serial monitor
  Serial.print("Distance: ");
  Serial.print(dist);
  Serial.print(" cm\t");
  Serial.print("Speed: ");
  Serial.print(spd);
  Serial.println(" km/h");
}

long usRead(){
  digitalWrite(trigger,LOW);
  delayMicroseconds(2);
  digitalWrite(trigger,HIGH);
  delayMicroseconds(10);
  digitalWrite(trigger, LOW);
  long time = pulseIn(echo,HIGH);

  dist = time*0.034/2;
  return time;
}