#define control_pin_1 5
#define control_pin_2 6
#define ENC_A 2
#define ENC_B 3
#define PPR 385
#include "PIDController.h"


PIDController PID;

//int a;
//int b;
int counter=0;
String input_string="";
int user_input;
double myValue;
float degree;

void setup() {
  // put your setup code here, to run once:
pinMode(control_pin_1,OUTPUT);
pinMode(control_pin_2,OUTPUT);
pinMode(ENC_A,INPUT);
pinMode(ENC_B,INPUT);
Serial.begin(9600);
PID.begin();
PID.tune(16, 0.3, 1);
 PID.limit(-255, 255);
 digitalWrite(control_pin_1, LOW);
  digitalWrite(control_pin_2, LOW);
 attachInterrupt(digitalPinToInterrupt(ENC_A),position_control,RISING);
}
//PIDController PID
void loop() {
  
  while(Serial.available())
  {
    delay(3);
    char input_char=Serial.read();
    input_string+=input_char;
  }
  
  if (input_string.length() > 0) { 
    user_input = input_string.toInt();
  }
  PID.setpoint((user_input / 360.0) * PPR);

  myValue=PID.compute(counter);

  motor_driver(myValue > 0 ? 'C': 'A',myValue < 0 ? abs(myValue)  : myValue);

   input_string="";
  degree=user_input * 360.0 * PPR;
  //Serial.print("feed back :  " );Serial.println(myValue);
  Serial.print(user_input);
  Serial.print(" ");
  Serial.print(counter);
  Serial.println();

  
  // put your main code here, to run repeatedly:
  
}

void motor_driver(char dir,int mot_speed)
{
  if (dir=='C')
  {
    analogWrite(control_pin_1,mot_speed);
    digitalWrite(control_pin_2,LOW);
  }
  else if (dir=='A')
  {
    digitalWrite(control_pin_1,LOW);
    analogWrite(control_pin_2,mot_speed);
  }
  
}

void position_control()
{
 if (digitalRead(ENC_B)==LOW)
 counter++;
 else
 counter--; 
}
