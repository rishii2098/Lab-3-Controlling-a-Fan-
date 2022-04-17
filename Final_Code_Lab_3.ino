/*
This lab is to design a controller to control a fan driven by a DC motor. Your design needs to
display the clock and fan rotation direction and speed.

(1) use the Arduino to control the motor that turns a fan. The fan can run at different speeds
    and in either direction (clockwise or counterclockwise)
(2) set up a Real Time Clock (RTC) and retrieve time information
(3) display the real-time clock and the fan’s rotation direction (“C” for clockwise, or “CC” or
    counterclockwise) and speed (“Full”, “3/4”, “1/2”, or “0”) on a LCD
(4) update the information display every second
(5) allow pressing a push button to change the rotation direction of the fan
    
    Additional requirements for EECE.5520 students 
    Allow an IR remote control to change the fan’s rotation speed and direction.
    
(6) use the IR receiver to receive and decode button press on an IR remote control.
(7) use the decoded information to switch the rotation direction and/or change the speed.
*/

/******************** Codes for IR Remote Buttons***********************************
 0 = FF6897, 1 = FF30CF, 2 = FF18E7, 3 = FF7A85 4 = FF10EF, 5 = FF38C7, 6 = FF5AA5
************************************************************************************/

 /*************************************************************
  *                          Libraries                        *
  *************************************************************/
#include <IRremote.h>
#include <Wire.h>
#include <LiquidCrystal.h>
#include  "RTClib.h"
#include <SoftwareSerial.h>

 /*************************************************************
  *             Variables/Assignment Declerations             *
  *************************************************************/
LiquidCrystal lcd(12,11,5,4,3,2);
char daysOfTheWeek[7][12] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
RTC_DS1307 rtc;
#define RTCAddress 0x68
const int RECV_PIN = 6; //Data pin for IR Sensor
int Rdirection;
int Flow_Control;
char x;
const int Enable12 = 10;  // PWM pin to L293D's EN12 (pin 1) 
const int Driver1A = 9;  // To L293D's 1A (pin 2)
const int Driver2A = 8;  // To L293D's 2A (pin 7)

// Define IR Receiver and Results Objects
IRrecv irrecv(RECV_PIN);
decode_results results;


 /*************************************************************
  *                       Void Setup                          *
  *************************************************************/
void setup(){
  //---set pin direction
    pinMode(Enable12,OUTPUT);
    pinMode(Driver1A,OUTPUT);
    pinMode(Driver2A,OUTPUT);
  // Serial Monitor @ 9600 baud
    Serial.begin(9600);

  // Enable the IR Receiver
    irrecv.enableIRIn();
  // set up the LCD's number of columns and rows:
    lcd.begin(16,2);
  //lcd.print("hello world");
    if (! rtc.begin()) {
      lcd.print("Couldn't find RTC");
      while (1);
      }
    if (! rtc.isrunning()) {
      lcd.print("RTC is NOT running!");
      }
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
   }

  /*************************************************************
  *                        Void Loop                           *
  **************************************************************/  
void loop(){
   if (irrecv.decode(&results)){
    // Print Code in HEX
       (results.value, HEX);
       irrecv.resume();
        }
        Flow_Control = 1;
        while (Flow_Control = 1){
          if (irrecv.decode(&results)){
          (results.value, HEX);
          irrecv.resume();
          }

          DateTime now = rtc.now();
          lcd.setCursor(0, 1);
          lcd.print(now.hour());
          lcd.print(':');
          lcd.print(now.minute());
          lcd.print(':');
          lcd.print(now.second());
          lcd.print("   ");
      
          lcd.setCursor(0, 0);
          lcd.print(daysOfTheWeek[now.dayOfTheWeek()]);
          lcd.print(" ,");
          lcd.print(now.month());
          lcd.print('/');
          lcd.print(now.day());
          lcd.print('/');
          lcd.print(now.year());
          
          if ((results.value)== 0xFF6897){  //Button 0 Motor Off
            goto Motor_Off;
          }
          if ((results.value)== 0xFF30CF){  //Button 1 Full Speed Clocwise
            goto FullSpeed_C;
          }
          if ((results.value)== 0xFF10EF){  //Button 4  Full Speed Counter Clockwise
            goto FullSpeed_CC;
          }
          if ((results.value)== 0xFF18E7){  //Button 2 Medium Speed Clocwise
            goto MedSpeed_C;
          }
          if ((results.value)== 0xFF38C7){  //Button 5 Medium Speed Counter Clockwise 
            goto MedSpeed_CC;
          }
          if ((results.value)== 0xFF7A85){  //Button 3 Slow Speed Clocwise
            goto SlowSpeed_C;
          } 
          if ((results.value)== 0xFF5AA5){  //Button 6 Slow Speed Counter Clockwise
            goto SlowSpeed_CC;
          }  
        }

 /*************************************************************
  *                          Motor Off                        *
  *************************************************************/
          Motor_Off:
          analogWrite(Enable12,0);  // PWM
          digitalWrite(Driver1A,LOW);   // Boolean
          digitalWrite(Driver2A,LOW);   // Boolean
          
          while (Flow_Control = 1){
          x = ' ';
          lcd.setCursor(9, 1);
          lcd.print(x);
          lcd.print(x);
          lcd.print(x);
          lcd.print(x);
          lcd.print(x);
          lcd.print(x);
          lcd.print(x);
          
          if (irrecv.decode(&results)){
          (results.value, HEX);
          irrecv.resume();
          }
          if ((results.value)== 0xFF6897){  //Button 0 Motor Off
            goto Motor_Off;
          }
          if ((results.value)== 0xFF30CF){  //Button 1 Full Speed Clocwise
            goto FullSpeed_C;
          }
          if ((results.value)== 0xFF10EF){  //Button 4  Full Speed Counter Clockwise
            goto FullSpeed_CC;
          }
          if ((results.value)== 0xFF18E7){  //Button 2 Medium Speed Clocwise
            goto MedSpeed_C;
          }
          if ((results.value)== 0xFF38C7){  //Button 5 Medium Speed Counter Clockwise 
            goto MedSpeed_CC;
          }
          if ((results.value)== 0xFF7A85){  //Button 3 Slow Speed Clocwise
            goto SlowSpeed_C;
          } 
          if ((results.value)== 0xFF5AA5){  //Button 6 Slow Speed Counter Clockwise
            goto SlowSpeed_CC;
          }  
         }
          
 /*************************************************************
  *              Full Speed Motor Control Clockwise           *
  *************************************************************/ 
          FullSpeed_C:      
          analogWrite(Enable12,255);  // PWM
          digitalWrite(Driver1A,LOW);   // Boolean
          digitalWrite(Driver2A,HIGH);   // Boolean

          while(Flow_Control = 1){
          DateTime now = rtc.now();
          lcd.setCursor(0, 1);
          lcd.print(now.hour());
          lcd.print(':');
          lcd.print(now.minute());
          lcd.print(':');
          lcd.print(now.second());
          lcd.print("   ");
      
          lcd.setCursor(0, 0);
          lcd.print(daysOfTheWeek[now.dayOfTheWeek()]);
          lcd.print(" ,");
          lcd.print(now.month());
          lcd.print('/');
          lcd.print(now.day());
          lcd.print('/');
          lcd.print(now.year());

          lcd.setCursor(9, 1);
          lcd.print('C');
          lcd.print(' ');
          lcd.print(' ');
          lcd.print('F');
          lcd.print('U');
          lcd.print('L');
          lcd.print('L');

          if (irrecv.decode(&results)){
          (results.value, HEX);
          irrecv.resume();
          }

          if ((results.value)== 0xFF6897){  //Button 0 Motor Off
            goto Motor_Off;
          }
          if ((results.value)== 0xFF30CF){  //Button 1 Full Speed Clocwise
            goto FullSpeed_C;
          }
          if ((results.value)== 0xFF10EF){  //Button 4  Full Speed Counter Clockwise
            goto FullSpeed_CC;
          }
          if ((results.value)== 0xFF18E7){  //Button 2 Medium Speed Clocwise
            goto MedSpeed_C;
          }
          if ((results.value)== 0xFF38C7){  //Button 5 Medium Speed Counter Clockwise 
            goto MedSpeed_CC;
          }
          if ((results.value)== 0xFF7A85){  //Button 3 Slow Speed Clocwise
            goto SlowSpeed_C;
          } 
          if ((results.value)== 0xFF5AA5){  //Button 6 Slow Speed Counter Clockwise
            goto SlowSpeed_CC;
          }  
        }

 /*************************************************************
  *         Full Speed Motor Control Counter Clockwise        *
  *************************************************************/ 
          FullSpeed_CC:      
          analogWrite(Enable12,255);  // PWM
          digitalWrite(Driver1A,HIGH);   // Boolean
          digitalWrite(Driver2A,LOW);   // Boolean

          while(Flow_Control = 1){
          DateTime now = rtc.now();
          lcd.setCursor(0, 1);
          lcd.print(now.hour());
          lcd.print(':');
          lcd.print(now.minute());
          lcd.print(':');
          lcd.print(now.second());
          lcd.print("   ");
      
          lcd.setCursor(0, 0);
          lcd.print(daysOfTheWeek[now.dayOfTheWeek()]);
          lcd.print(" ,");
          lcd.print(now.month());
          lcd.print('/');
          lcd.print(now.day());
          lcd.print('/');
          lcd.print(now.year());

          lcd.setCursor(9, 1);
          lcd.print('C');
          lcd.print('C');
          lcd.print(' ');
          lcd.print('F');
          lcd.print('U');
          lcd.print('L');
          lcd.print('L');

          if (irrecv.decode(&results)){
          (results.value, HEX);
          irrecv.resume();
          }

          if ((results.value)== 0xFF6897){  //Button 0 Motor Off
            goto Motor_Off;
          }
          if ((results.value)== 0xFF30CF){  //Button 1 Full Speed Clocwise
            goto FullSpeed_C;
          }
          if ((results.value)== 0xFF10EF){  //Button 4  Full Speed Counter Clockwise
            goto FullSpeed_CC;
          }
          if ((results.value)== 0xFF18E7){  //Button 2 Medium Speed Clocwise
            goto MedSpeed_C;
          }
          if ((results.value)== 0xFF38C7){  //Button 5 Medium Speed Counter Clockwise 
            goto MedSpeed_CC;
          }
          if ((results.value)== 0xFF7A85){  //Button 3 Slow Speed Clocwise
            goto SlowSpeed_C;
          } 
          if ((results.value)== 0xFF5AA5){  //Button 6 Slow Speed Counter Clockwise
            goto SlowSpeed_CC;
          }  
        }

 /*************************************************************
  *           Medium Speed Motor Control Clockwise            *
  *************************************************************/ 
          MedSpeed_C:      
          while(Flow_Control = 1){
          analogWrite(Enable12,255);  // PWM
          digitalWrite(Driver1A,LOW);   // Boolean
          digitalWrite(Driver2A,HIGH);   // Boolean
          delay(100);
          analogWrite(Enable12,255);  // PWM
          digitalWrite(Driver1A,LOW);   // Boolean
          digitalWrite(Driver2A,LOW);   // Boolean
          delay(100);

          DateTime now = rtc.now();
          lcd.setCursor(0, 1);
          lcd.print(now.hour());
          lcd.print(':');
          lcd.print(now.minute());
          lcd.print(':');
          lcd.print(now.second());
          lcd.print("   ");
      
          lcd.setCursor(0, 0);
          lcd.print(daysOfTheWeek[now.dayOfTheWeek()]);
          lcd.print(" ,");
          lcd.print(now.month());
          lcd.print('/');
          lcd.print(now.day());
          lcd.print('/');
          lcd.print(now.year());

          lcd.setCursor(9, 1);
          lcd.print('C');
          lcd.print(' ');
          lcd.print(' ');
          lcd.print('3');
          lcd.print('/');
          lcd.print('4');
          lcd.print(' ');

          if (irrecv.decode(&results)){
          (results.value, HEX);
          irrecv.resume();
          }

          if ((results.value)== 0xFF6897){  //Button 0 Motor Off
            goto Motor_Off;
          }
          if ((results.value)== 0xFF30CF){  //Button 1 Full Speed Clocwise
            goto FullSpeed_C;
          }
          if ((results.value)== 0xFF10EF){  //Button 4  Full Speed Counter Clockwise
            goto FullSpeed_CC;
          }
          if ((results.value)== 0xFF18E7){  //Button 2 Medium Speed Clocwise
            goto MedSpeed_C;
          }
          if ((results.value)== 0xFF38C7){  //Button 5 Medium Speed Counter Clockwise 
            goto MedSpeed_CC;
          }
          if ((results.value)== 0xFF7A85){  //Button 3 Slow Speed Clocwise
            goto SlowSpeed_C;
          } 
          if ((results.value)== 0xFF5AA5){  //Button 6 Slow Speed Counter Clockwise
            goto SlowSpeed_CC;
          }  
        }

 /*************************************************************
  *        Medium Speed Motor Control Counter Clockwise       *
  *************************************************************/ 
          MedSpeed_CC:      
          while(Flow_Control = 1){
          analogWrite(Enable12,255);  // PWM
          digitalWrite(Driver1A,HIGH);   // Boolean
          digitalWrite(Driver2A,LOW);   // Boolean
          delay(100);
          analogWrite(Enable12,255);  // PWM
          digitalWrite(Driver1A,LOW);   // Boolean
          digitalWrite(Driver2A,LOW);   // Boolean
          delay(100);

          DateTime now = rtc.now();
          lcd.setCursor(0, 1);
          lcd.print(now.hour());
          lcd.print(':');
          lcd.print(now.minute());
          lcd.print(':');
          lcd.print(now.second());
          lcd.print("   ");
      
          lcd.setCursor(0, 0);
          lcd.print(daysOfTheWeek[now.dayOfTheWeek()]);
          lcd.print(" ,");
          lcd.print(now.month());
          lcd.print('/');
          lcd.print(now.day());
          lcd.print('/');
          lcd.print(now.year());

          lcd.setCursor(9, 1);
          lcd.print('C');
          lcd.print('C');
          lcd.print(' ');
          lcd.print('3');
          lcd.print('/');
          lcd.print('4');
          lcd.print(' ');

          if (irrecv.decode(&results)){
          (results.value, HEX);
          irrecv.resume();
          }

          if ((results.value)== 0xFF6897){  //Button 0 Motor Off
            goto Motor_Off;
          }
          if ((results.value)== 0xFF30CF){  //Button 1 Full Speed Clocwise
            goto FullSpeed_C;
          }
          if ((results.value)== 0xFF10EF){  //Button 4  Full Speed Counter Clockwise
            goto FullSpeed_CC;
          }
          if ((results.value)== 0xFF18E7){  //Button 2 Med Speed Clocwise
            goto MedSpeed_C;
          }
          if ((results.value)== 0xFF38C7){  //Button 5 Med Speed Counter Clockwise 
            goto MedSpeed_CC;
          }
          if ((results.value)== 0xFF7A85){  //Button 3 Slow Speed Clocwise
            goto SlowSpeed_C;
          } 
          if ((results.value)== 0xFF5AA5){  //Button 6 Slow Speed Counter Clockwise
            goto SlowSpeed_CC;
          }  
        }
        
 /*************************************************************
  *             Slow Speed Motor Control Clockwise            *
  *************************************************************/ 
          SlowSpeed_C:      
          while(Flow_Control = 1){
          analogWrite(Enable12,255);  // PWM
          digitalWrite(Driver1A,LOW);   // Boolean
          digitalWrite(Driver2A,HIGH);   // Boolean
          delay(33);
          analogWrite(Enable12,255);  // PWM
          digitalWrite(Driver1A,LOW);   // Boolean
          digitalWrite(Driver2A,LOW);   // Boolean
          delay(33);

          DateTime now = rtc.now();
          lcd.setCursor(0, 1);
          lcd.print(now.hour());
          lcd.print(':');
          lcd.print(now.minute());
          lcd.print(':');
          lcd.print(now.second());
          lcd.print("   ");
      
          lcd.setCursor(0, 0);
          lcd.print(daysOfTheWeek[now.dayOfTheWeek()]);
          lcd.print(" ,");
          lcd.print(now.month());
          lcd.print('/');
          lcd.print(now.day());
          lcd.print('/');
          lcd.print(now.year());

          lcd.setCursor(9, 1);
          lcd.print('C');
          lcd.print(' ');
          lcd.print(' ');
          lcd.print('1');
          lcd.print('/');
          lcd.print('2');
          lcd.print(' ');

          if (irrecv.decode(&results)){
          (results.value, HEX);
          irrecv.resume();
          }

          if ((results.value)== 0xFF6897){  //Button 0 Motor Off
            goto Motor_Off;
          }
          if ((results.value)== 0xFF30CF){  //Button 1 Full Speed Clocwise
            goto FullSpeed_C;
          }
          if ((results.value)== 0xFF10EF){  //Button 4  Full Speed Counter Clockwise
            goto FullSpeed_CC;
          }
          if ((results.value)== 0xFF18E7){  //Button 2 Medium Speed Clocwise
            goto MedSpeed_C;
          }
          if ((results.value)== 0xFF38C7){  //Button 5 Medium Speed Counter Clockwise 
            goto MedSpeed_CC;
          }
          if ((results.value)== 0xFF7A85){  //Button 3 Slow Speed Clocwise
            goto SlowSpeed_C;
          } 
          if ((results.value)== 0xFF5AA5){  //Button 6 Slow Speed Counter Clockwise
            goto SlowSpeed_CC;
          }  
        }

 /*************************************************************
  *        Slow Speed Motor Control Counter Clockwise         *
  *************************************************************/ 
         SlowSpeed_CC:      
          while(Flow_Control = 1){
          analogWrite(Enable12,255);  // PWM
          digitalWrite(Driver1A,HIGH);   // Boolean
          digitalWrite(Driver2A,LOW);   // Boolean
          delay(33);
          analogWrite(Enable12,255);  // PWM
          digitalWrite(Driver1A,LOW);   // Boolean
          digitalWrite(Driver2A,LOW);   // Boolean
          delay(33);
 
          DateTime now = rtc.now();
          lcd.setCursor(0, 1);
          lcd.print(now.hour());
          lcd.print(':');
          lcd.print(now.minute());
          lcd.print(':');
          lcd.print(now.second());
          lcd.print("   ");
      
          lcd.setCursor(0, 0);
          lcd.print(daysOfTheWeek[now.dayOfTheWeek()]);
          lcd.print(" ,");
          lcd.print(now.month());
          lcd.print('/');
          lcd.print(now.day());
          lcd.print('/');
          lcd.print(now.year());

          lcd.setCursor(9, 1);
          lcd.print('C');
          lcd.print('C');
          lcd.print(' ');
          lcd.print('1');
          lcd.print('/');
          lcd.print('2');
          lcd.print(' ');

          if (irrecv.decode(&results)){
          (results.value, HEX);
          irrecv.resume();
          }

          if ((results.value)== 0xFF6897){  //Button 0 Motor Off
            goto Motor_Off;
          }
          if ((results.value)== 0xFF30CF){  //Button 1 Full Speed Clocwise
            goto FullSpeed_C;
          }
          if ((results.value)== 0xFF10EF){  //Button 4  Full Speed Counter Clockwise
            goto FullSpeed_CC;
          }
          if ((results.value)== 0xFF18E7){  //Button 2 Medium Speed Clocwise
            goto MedSpeed_C;
          }
          if ((results.value)== 0xFF38C7){  //Button 5 Medium Speed Counter Clockwise 
            goto MedSpeed_CC;
          }
          if ((results.value)== 0xFF7A85){  //Button 3 Slow Speed Clocwise
            goto SlowSpeed_C;
          } 
          if ((results.value)== 0xFF5AA5){  //Button 6 Slow Speed Counter Clockwise
            goto SlowSpeed_CC;
          }  
        }
}
