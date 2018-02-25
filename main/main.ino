#include <StandardCplusplus.h>
#include <system_configuration.h>
#include <unwind-cxx.h>
#include <vector>
#include <utility.h>
using namespace std;
#include "sd_avg.h"


#define buttonPin 3

// Functions
void led_on_off();
void checkuser();


// Variables will change:
float difference;
float endTime;
float startTime;
int ledPin = 2;

// States
int ledState = HIGH;         // the current state of the output pin
int buttonState;             // the current reading from the input pin
int lastButtonState = LOW;   // the previous reading from the input pin
int lastLEDState = LOW;      // the previous reading from the LED pin
int currAvg = 0;
int currSD = 0;


// the following variables are unsigned longs because the time, measured in
// milliseconds, will quickly become a bigger number than can be stored in an int.
unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
unsigned long debounceDelay = 50;    // the debounce time; increase if the output flickers


// array
vector<int> timeDifferences;



void setup() {
  Serial.begin(9600);
  pinMode(buttonPin, INPUT);
  pinMode(ledPin, OUTPUT);
  // set initial LED state
  digitalWrite(ledPin, ledState);
}

void loop() {
  led_on_off();
}



void led_on_off(){

  // read the state of the switch into a local variable:
  int reading = digitalRead(buttonPin);
  
  // check to see if you just pressed the button
  // (i.e. the input went from LOW to HIGH), and you've waited long enough
  // since the last press to ignore any noise:

  // If the switch changed, due to noise or pressing:
  if (reading != lastButtonState) {
    // reset the debouncing timer
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    // whatever the reading is at, it's been there for longer than the debounce
    // delay, so take it as the actual current state:

    // if the button state has changed:
    if (reading != buttonState) {
      buttonState = reading;

      // only toggle the LED if the new button state is HIGH
      if (buttonState == HIGH) {
        ledState = !ledState;
      }
    }
  }

  // set the LED:
  digitalWrite(ledPin, ledState);
  
  if(ledState == LOW && lastLEDState == HIGH){
    startTime = millis();
    //Serial.print("startTime =");
    //Serial.println(startTime);
  }
  else if(ledState == HIGH && lastLEDState == LOW && millis() != 0){
    endTime = millis();
    //Serial.print("endTime =");
    //Serial.println(endTime);
    difference = (endTime - startTime);
        Serial.print("difference =");
    Serial.println(difference);
  }

  currAvg = avg(timeDifferences);
  currSD = sd(timeDifferences);
  if(millis() - startTime > currAvg + 2*currSD && ledState == LOW) 
    check_user();
  else
    timeDifferences.push_back(difference);
      
    

    

  
  lastLEDState = ledState;
  
  // save the reading. Next time through the loop, it'll be the lastButtonState:
  lastButtonState = reading;
  
}

void check_user() {
  //open serial window with message, press 0 to turn the LED off, press 1 to turn the LED on
  pinMode(ledPin, OUTPUT);
  Serial.println("You've been using the light for longer than usual. Should I turn it off?");
//  while(Serial.available() == 0){
//    if (digitalRead(buttonPin)==HIGH)
//        break;
//  }
  char num = Serial.read();
  if(num != 'y' && num != 'n'){
    Serial.println("Error");  
  }
  else{
    if(num == 'y'){
      Serial.println("LED is off"); 
      digitalWrite(ledPin, HIGH);
      ledState = HIGH; 
    }
    else{
      Serial.println("LED is on");  
      digitalWrite(ledPin, LOW);
      timeDifferences.push_back(difference);
    }
  }
  //run the decision for 5 seconds (5000 ms)
  //delay(5000);
  //repeat the loop
}


