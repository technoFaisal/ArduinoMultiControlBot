s// Libraries
#include <SoftwareSerial.h>
#include <Servo.h>
#include "ultrasonic.h"

//constants
SoftwareSerial BT(1,0); //TX, RX respetively
String readvoice;
String readvoice1;
const int button = 2;     //Button pin to pin 2
const int led    = 3;     //Led pin (throught resistor) to pin 3
const int ledblue    = 5;     //Led pin (throught resistor) to pin 3
const int buzzer = 4;     //Buzzer pin to pin 4
const int motorA1= 6;     //motor A positive (+) pin to pin 6 (PWM) (from L298 module!)
const int motorA2= 9;     //motor A negative (-) pin to pin 9 (PWM)
const int motorB1=10;     //motor B positive (+) pin to pin 10 (PWM)
const int motorB2=11;     //motor B negative (-) pin to pin 11 (PWM)

//Variables
int distance;         //Variable to store distance from an object
int checkRight;
int checkLeft;
int function=0;         //Variable to store function of robot: '1' running or '0' stoped. By default is stoped
int buttonState=0;            //Variable to store the state of the button. By default '0'
int pos=90;             //Variable to store the servo position. By default 90 degrees - sensor will 'look' forward
int flag=0;           //Useful flag to keep the button state when button is released 



Ultrasonic ultrasonic(A4 ,A5); //Create Ultrsonic object ultrasonic(trig pin,echo pin)
Servo myservo;          //Create Servo object to control a servo

int exitmanual=0;

void setup() 
{
  BT.begin(9600);
  Serial.begin(9600);
    myservo.attach(5);      //Servo pin connected to pin 5
    myservo.write(pos);        // tell servo to go to position in variable 'pos' 
  pinMode(button, INPUT_PULLUP);
  pinMode(led,  OUTPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(motorA1,OUTPUT);
  pinMode(motorA2,OUTPUT);
  pinMode(motorB1,OUTPUT);
  pinMode(motorB2,OUTPUT);
  
  // put your setup code here, to run once:

}

void loop() 
{
  // put your main code here, to run repeatedly:
  while (BT.available())
  {
    delay(10);
    char c=BT.read();
    readvoice +=c;
   }
  if(readvoice.length() > 0)
  { Serial.println(readvoice);

  switch(readvoice)

  case "go manual":
  digitalWrite(ledblue, HIGH);
  while(exitmanual!=1)
  { 
  
  char c1=BT.read();
   readvoice2 += c2;
if(readvoice2.length() > 0)
  { Serial.println(readvoice2)
   if(readvoice2 == "move forward")
   {
    forward();
    }

   if(readvoice2 == "reverse")
   {
    backward();
    }
   
   if(readvoice2 == "turn left")
   {
    left();
    }
    
   if(readvoice2 == "turn right")
    {
    right();
    }
    if(readvoice2 == "stop")
    {
      stop()
      exitmanual=1
     }  
     
    }
   }
   break;
   
   case "autocontrol":
   while(delay(100000))
   {
   digitalWrite(led, HIGH);
    //Read distance...
    distance = ultrasonic.Ranging(CM);
        //Check for objects...
    if (distance > 10)
    {
      forward(); //All clear, move forward!
      noTone(buzzer);
    }
  else if (distance <=10){
      stop(); //Object detected! Stop the robot and check left and right for the better way out!
      tone(buzzer,500); // play a tone
     
      //Start scanning... 
      for(pos = 0; pos < 180; pos += 1){  //goes from 0 degrees to 180 degrees 
                myservo.write(pos);             //tell servo to go to position in variable 'pos' 
                delay(10);                      //waits 10ms for the servo to reach the position 
            } 
            
            checkLeft = ultrasonic.Ranging(CM);   //Take distance from the left side
            
            for(pos = 180; pos>=0; pos-=1){     //goes from 180 degrees to 0 degrees                           
                myservo.write(pos);             //tell servo to go to position in variable 'pos' 
                delay(10);                      //waits 10ms for the servo to reach the position   
            }
            
            checkRight= ultrasonic.Ranging(CM);
            
            myservo.write(90);                   // Sensor "look" forward again
            
            //Finally, take the right decision, turn left or right?
            if (checkLeft > checkRight){
              left();
              delay(400); // delay, change value if necessary to make robot turn.            
              }
            else if (checkLeft < checkRight){
              right();
              delay(400); // delay, change value if necessary to make robot turn.
            }
            else if (checkLeft <=10 && checkRight <=10){
              backward(); //The road is closed... go back and then left ;)
              left();
            }

  }


break;


   
 }
}

    
  void forward()
  {
  digitalWrite(motorA1, HIGH);
  digitalWrite(motorA2, LOW);
  digitalWrite(motorB1, HIGH);
  digitalWrite(motorB2, LOW); 
}

void backward()
{
  digitalWrite(motorA1, LOW);
  digitalWrite(motorA2, HIGH);
  digitalWrite(motorB1, LOW);
  digitalWrite(motorB2, HIGH);
}

void left()
{
  digitalWrite(motorA1, HIGH);
  digitalWrite(motorA2, LOW);
  digitalWrite(motorB1, LOW);
  digitalWrite(motorB2, HIGH);
}

void right()
{
  digitalWrite(motorA1, LOW);
  digitalWrite(motorA2, HIGH);
  digitalWrite(motorB1, HIGH);
  digitalWrite(motorB2, LOW); 
}

void stop()
{
  digitalWrite(motorA1, LOW);
  digitalWrite(motorA2, LOW);
  digitalWrite(motorB1, LOW);
  digitalWrite(motorB2, LOW);
}
