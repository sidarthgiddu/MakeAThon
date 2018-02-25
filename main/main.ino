#include <StandardCplusplus.h>
#include <system_configuration.h>
#include <unwind-cxx.h>
#include <vector>
#include <utility.h>
using namespace std;
#include "sd_avg.h"

// PINS
#define button1Pin 3
int button2Pin = 7;
int button3Pin = 8;
int led1Pin = 2;
int led2Pin = 5;
int led3Pin = 6;


// Functions
void led_on_off1();
void checkuser1();


// Variables will change:
float difference1;
float endTime1;
float startTime1;

int currAvg1 = 0;
int currSD1 = 0;
int msgCount1 = 0;

// States
int ledState1 = HIGH;         // the current state of the output pin
int buttonState1;             // the current reading from the input pin
int lastButtonState1 = LOW;   // the previous reading from the input pin
int lastLEDState1 = LOW;      // the previous reading from the LED pin

void led_on_off2();
void checkuser2();


// Variables will change:
float difference2;
float endTime2;
float startTime2;

int currAvg2 = 0;
int currSD2 = 0;
int msgCount2 = 0;

// States
int ledState2 = HIGH;         // the current state of the output pin
int buttonState2;             // the current reading from the input pin
int lastButtonState2 = LOW;   // the previous reading from the input pin
int lastLEDState2 = LOW;      // the previous reading from the LED pin

void led_on_off3();
void checkuser3();


// Variables will change:
float difference3;
float endTime3;
float startTime3;

int currAvg3 = 0;
int currSD3 = 0;
int msgCount3 = 0;

// States
int ledState3 = HIGH;         // the current state of the output pin
int buttonState3;             // the current reading from the input pin
int lastButtonState3 = LOW;   // the previous reading from the input pin
int lastLEDState3 = LOW;      // the previous reading from the LED pin



// the following variables are unsigned longs because the time, measured in
// milliseconds, will quickly become a bigger number than can be stored in an int.
unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
unsigned long debounceDelay = 50;    // the debounce time; increase if the output flickers


// array
vector<int> timeDifferences1;
vector<int> timeDifferences2;
vector<int> timeDifferences3;



void setup() {
  Serial.begin(9600);
  pinMode(button1Pin, INPUT);
  pinMode(led1Pin, OUTPUT);
  pinMode(led2Pin, OUTPUT);
  pinMode(led3Pin, OUTPUT);
  // set initial LED state
  digitalWrite(led1Pin, ledState1);
  digitalWrite(led2Pin, ledState2);
  digitalWrite(led3Pin, ledState3);
}

void loop() {
  led_on_off1();
  led_on_off2();
  led_on_off3();
}



void led_on_off1(){

  // read the state of the switch into a local variable:
  int reading = digitalRead(button1Pin);
  
  // check to see if you just pressed the button
  // (i.e. the input went from LOW to HIGH), and you've waited long enough
  // since the last press to ignore any noise:

  // If the switch changed, due to noise or pressing:
  if (reading != lastButtonState1) {
    // reset the debouncing timer
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    // whatever the reading is at, it's been there for longer than the debounce
    // delay, so take it as the actual current state:

    // if the button state has changed:
    if (reading != buttonState1) {
      buttonState1 = reading;

      // only toggle the LED if the new button state is HIGH
      if (buttonState1 == HIGH) {
        ledState1 = !ledState1;
      }
    }
  }

  // set the LED:
  digitalWrite(led1Pin, ledState1);
  
  if(ledState1 == LOW && lastLEDState1 == HIGH){
    startTime1 = millis();
    //Serial.print("startTime =");
    //Serial.println(startTime);
  }
  else if(ledState1 == HIGH && lastLEDState1 == LOW && millis() != 0){
    endTime1 = millis();
    //Serial.print("endTime =");
    //Serial.println(endTime);
    difference1 = (endTime1 - startTime1);
//    Serial.print("difference = ");
//    Serial.print(difference1);
//    Serial.println(" milliseconds");
    msgCount1 = 0;
  }

  currAvg1 = avg(timeDifferences1);
  currSD1 = sd(timeDifferences1);
  if(millis() - startTime1 > currAvg1 + 2*currSD1 && ledState1 == LOW) 
      check_user1();
  else if(ledState1 == HIGH && lastLEDState1 == LOW){
    if (timeDifferences1.size() >= 10){
      timeDifferences1.erase(timeDifferences1.begin());
    }
    timeDifferences1.push_back(difference1);
  }
    
  lastLEDState1 = ledState1;
  // save the reading. Next time through the loop, it'll be the lastButtonState:
  lastButtonState1 = reading;
  
}

void check_user1() {
    if(msgCount1 == 0) {
      Serial.print("You've been using light");
      Serial.print(led1Pin);
      Serial.println(" for longer than usual. Should I turn it off?");
      Serial.println("Press 2 for yes and x for no.");
      msgCount1++;
    }
    

  char num = Serial.read();

    if(num == '6'){
      Serial.print("LED ");
      Serial.print(led3Pin);
      Serial.println(" is off"); 
      digitalWrite(led3Pin, HIGH);
      ledState3 = HIGH;
      msgCount3 = 0;
    }
    else if (num == '5'){
      Serial.print("LED ");
      Serial.print(led2Pin);
      Serial.println(" is off"); 
      digitalWrite(led2Pin, HIGH);
      ledState2 = HIGH;
      msgCount2 = 0;
    }
    else if (num == '2'){
      Serial.print("LED ");
      Serial.print(led1Pin);
      Serial.println(" is off"); 
      digitalWrite(led1Pin, HIGH);
      ledState1 = HIGH;
      msgCount1 = 0;
    }
    else if (num=='x'){
      Serial.print("LED ");
      Serial.print(led1Pin);
      Serial.println(" is on");  
      digitalWrite(led1Pin, LOW);
      if (timeDifferences1.size() >= 10){
      timeDifferences1.erase(timeDifferences1.begin());
      }
      timeDifferences1.push_back(difference1);
      ledState1 = LOW;
    }
    else if (num=='y'){
      Serial.print("LED ");
      Serial.print(led2Pin);
      Serial.println(" is on");  
      digitalWrite(led2Pin, LOW);
      if (timeDifferences2.size() >= 10){
      timeDifferences2.erase(timeDifferences2.begin());
      }
      timeDifferences2.push_back(difference2);
      ledState2 = LOW;
    }
    else if (num=='z'){
      Serial.print("LED ");
      Serial.print(led3Pin);
      Serial.println(" is on");  
      digitalWrite(led3Pin, LOW);
      if (timeDifferences3.size() >= 10){
      timeDifferences3.erase(timeDifferences3.begin());
      }
      timeDifferences3.push_back(difference3);
      ledState3 = LOW;
    }
}

void led_on_off2(){

  // read the state of the switch into a local variable:
  int reading = digitalRead(button2Pin);
  
  // check to see if you just pressed the button
  // (i.e. the input went from LOW to HIGH), and you've waited long enough
  // since the last press to ignore any noise:

  // If the switch changed, due to noise or pressing:
  if (reading != lastButtonState2) {
    // reset the debouncing timer
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    // whatever the reading is at, it's been there for longer than the debounce
    // delay, so take it as the actual current state:

    // if the button state has changed:
    if (reading != buttonState2) {
      buttonState2 = reading;

      // only toggle the LED if the new button state is HIGH
      if (buttonState2 == HIGH) {
        ledState2 = !ledState2;
      }
    }
  }

  // set the LED:
  digitalWrite(led2Pin, ledState2);
  
  if(ledState2 == LOW && lastLEDState2 == HIGH){
    startTime2 = millis();
    //Serial.print("startTime =");
    //Serial.println(startTime);
  }
  else if(ledState2 == HIGH && lastLEDState2 == LOW && millis() != 0){
    endTime2 = millis();
    //Serial.print("endTime =");
    //Serial.println(endTime);
    difference2 = (endTime2 - startTime2);
//    Serial.print("difference = ");
//    Serial.print(difference2);
//    Serial.println(" milliseconds");
    msgCount2 = 0;
  }

  currAvg2 = avg(timeDifferences2);
  currSD2 = sd(timeDifferences2);
  if(millis() - startTime2 > currAvg2 + 2*currSD2 && ledState2 == LOW) 
      check_user2();
  else if(ledState2 == HIGH && lastLEDState2 == LOW){
    if (timeDifferences2.size() >= 10){
      timeDifferences2.erase(timeDifferences2.begin());
    }
    timeDifferences2.push_back(difference2);
  }
    
  lastLEDState2 = ledState2;
  // save the reading. Next time through the loop, it'll be the lastButtonState:
  lastButtonState2 = reading;
  
}

void check_user2() {
    if(msgCount2 == 0) {
      Serial.print("You've been using light");
      Serial.print(led2Pin);
      Serial.println(" for longer than usual. Should I turn it off?");
      Serial.println("Press 5 for yes and y for no.");
      msgCount2++;
    }
    

 char num = Serial.read();

    if(num == '6'){
      Serial.print("LED ");
      Serial.print(led3Pin);
      Serial.println(" is off"); 
      digitalWrite(led3Pin, HIGH);
      ledState3 = HIGH;
      msgCount3 = 0;
    }
    else if (num == '5'){
      Serial.print("LED ");
      Serial.print(led2Pin);
      Serial.println(" is off"); 
      digitalWrite(led2Pin, HIGH);
      ledState2 = HIGH;
      msgCount2 = 0;
    }
    else if (num == '2'){
      Serial.print("LED ");
      Serial.print(led1Pin);
      Serial.println(" is off"); 
      digitalWrite(led1Pin, HIGH);
      ledState1 = HIGH;
      msgCount1 = 0;
    }
    else if (num=='x'){
      Serial.print("LED ");
      Serial.print(led1Pin);
      Serial.println(" is on");  
      digitalWrite(led1Pin, LOW);
      if (timeDifferences1.size() >= 10){
      timeDifferences1.erase(timeDifferences1.begin());
      }
      timeDifferences1.push_back(difference1);
      ledState1 = LOW;
    }
    else if (num=='y'){
      Serial.print("LED ");
      Serial.print(led2Pin);
      Serial.println(" is on");  
      digitalWrite(led2Pin, LOW);
      if (timeDifferences2.size() >= 10){
      timeDifferences2.erase(timeDifferences2.begin());
      }
      timeDifferences2.push_back(difference2);
      ledState2 = LOW;
    }
    else if (num=='z'){
      Serial.print("LED ");
      Serial.print(led3Pin);
      Serial.println(" is on");  
      digitalWrite(led3Pin, LOW);
      if (timeDifferences3.size() >= 10){
      timeDifferences3.erase(timeDifferences3.begin());
      }
      timeDifferences3.push_back(difference3);
      ledState3 = LOW;
    }
}

void led_on_off3(){

  // read the state of the switch into a local variable:
  int reading = digitalRead(button3Pin);
  
  // check to see if you just pressed the button
  // (i.e. the input went from LOW to HIGH), and you've waited long enough
  // since the last press to ignore any noise:

  // If the switch changed, due to noise or pressing:
  if (reading != lastButtonState3) {
    // reset the debouncing timer
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    // whatever the reading is at, it's been there for longer than the debounce
    // delay, so take it as the actual current state:

    // if the button state has changed:
    if (reading != buttonState3) {
      buttonState3 = reading;

      // only toggle the LED if the new button state is HIGH
      if (buttonState3 == HIGH) {
        ledState3 = !ledState3;
      }
    }
  }

  // set the LED:
  digitalWrite(led3Pin, ledState3);
  
  if(ledState3 == LOW && lastLEDState3 == HIGH){
    startTime3 = millis();
    //Serial.print("startTime =");
    //Serial.println(startTime);
  }
  else if(ledState3 == HIGH && lastLEDState3 == LOW && millis() != 0){
    endTime3 = millis();
    //Serial.print("endTime =");
    //Serial.println(endTime);
    difference3 = (endTime3 - startTime3);
//    Serial.print("difference = ");
//    Serial.print(difference3);
//    Serial.println(" milliseconds");
    msgCount3 = 0;
  }

  currAvg3 = avg(timeDifferences3);
  currSD3 = sd(timeDifferences3);
  if(millis() - startTime3 > currAvg3 + 3*currSD3 && ledState3 == LOW) 
      check_user3();
  else if(ledState3 == HIGH && lastLEDState3 == LOW){
    if (timeDifferences3.size() >= 10){
      timeDifferences3.erase(timeDifferences3.begin());
    }
    timeDifferences3.push_back(difference3);
  }
    
  lastLEDState3 = ledState3;
  // save the reading. Next time through the loop, it'll be the lastButtonState:
  lastButtonState3 = reading;
  
}

void check_user3() {
    if(msgCount3 == 0) {
      Serial.print("You've been using light");
      Serial.print(led3Pin);
      Serial.println(" for longer than usual. Should I turn it off?");
      Serial.println("Press 6 for yes and z for no.");
      msgCount3++;
    }
    

  char num = Serial.read();

    if(num == '6'){
      Serial.print("LED ");
      Serial.print(led3Pin);
      Serial.println(" is off"); 
      digitalWrite(led3Pin, HIGH);
      ledState3 = HIGH;
      msgCount3 = 0;
    }
    else if (num == '5'){
      Serial.print("LED ");
      Serial.print(led2Pin);
      Serial.println(" is off"); 
      digitalWrite(led2Pin, HIGH);
      ledState2 = HIGH;
      msgCount2 = 0;
    }
    else if (num == '2'){
      Serial.print("LED ");
      Serial.print(led1Pin);
      Serial.println(" is off"); 
      digitalWrite(led1Pin, HIGH);
      ledState1 = HIGH;
      msgCount1 = 0;
    }
    else if (num=='x'){
      Serial.print("LED ");
      Serial.print(led1Pin);
      Serial.println(" is on");  
      digitalWrite(led1Pin, LOW);
      if (timeDifferences1.size() >= 10){
      timeDifferences1.erase(timeDifferences1.begin());
      }
      timeDifferences1.push_back(difference1);
      ledState1 = LOW;
    }
    else if (num=='y'){
      Serial.print("LED ");
      Serial.print(led2Pin);
      Serial.println(" is on");  
      digitalWrite(led2Pin, LOW);
      if (timeDifferences2.size() >= 10){
      timeDifferences2.erase(timeDifferences2.begin());
      }
      timeDifferences2.push_back(difference2);
      ledState2 = LOW;
    }
    else if (num=='z'){
      Serial.print("LED ");
      Serial.print(led3Pin);
      Serial.println(" is on");  
      digitalWrite(led3Pin, LOW);
      if (timeDifferences3.size() >= 10){
      timeDifferences3.erase(timeDifferences3.begin());
      }
      timeDifferences3.push_back(difference3);
      ledState3 = LOW;
    }
}

void check_user(int &ledPin, int &msgCount, int &ledState, vector<int> &timeDifferences, float &difference){
  if(msgCount == 0) {
      char off;
      if (ledPin == 2){
        off = 'x';
      }
      else if (ledPin == 5){
        off = 'y';
      }
      else if (ledPin == 6){
        off = 'z';
      }
      Serial.print("You've been using light ");
      Serial.print(ledPin);
      Serial.println(" for longer than usual. Should I turn it off?");
      Serial.print("Press ");
      Serial.print(ledPin);
      Serial.print(" for yes and "); 
      Serial.print(off);
      Serial.println(" for no.");
      msgCount++;
    }
    

  char num = Serial.read();
  if(num == '6' && ledPin == 6){
      Serial.print("LED ");
      Serial.print(ledPin);
      Serial.println(" is off"); 
      digitalWrite(ledPin, HIGH);
      ledState = HIGH;
      msgCount = 0;
    }
    else if (num == '5' && ledPin == 5){
      Serial.print("LED ");
      Serial.print(ledPin);
      Serial.println(" is off"); 
      digitalWrite(ledPin, HIGH);
      ledState = HIGH;
      msgCount = 0;
    }
    else if (num == '2' && ledPin == 2){
      Serial.print("LED ");
      Serial.print(ledPin);
      Serial.println(" is off"); 
      digitalWrite(ledPin, HIGH);
      ledState = HIGH;
      msgCount = 0;
    }
    else if (num=='x'){
      Serial.print("LED ");
      Serial.print(ledPin);
      Serial.println(" is on");  
      digitalWrite(ledPin, LOW);
      if (timeDifferences.size() >= 10){
      timeDifferences.erase(timeDifferences.begin());
      }
      timeDifferences.push_back(difference);
      ledState = LOW;
    }
    else if (num=='y'){
      Serial.print("LED ");
      Serial.print(ledPin);
      Serial.println(" is on");  
      digitalWrite(ledPin, LOW);
      if (timeDifferences.size() >= 10){
      timeDifferences.erase(timeDifferences.begin());
      }
      timeDifferences.push_back(difference);
      ledState = LOW;
    }
    else if (num=='z'){
      Serial.print("LED ");
      Serial.print(ledPin);
      Serial.println(" is on");  
      digitalWrite(ledPin, LOW);
      if (timeDifferences.size() >= 10){
      timeDifferences.erase(timeDifferences.begin());
      }
      timeDifferences.push_back(difference);
      ledState = LOW;
    }
}

