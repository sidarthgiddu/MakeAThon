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
void checkuser(int n);


// Variables will change:
float difference;
float endTime;
float startTime;
int ledPin = 2;
int currAvg = 0;
int currSD = 0;
int msgCount = 0;

// States
int ledState = HIGH;         // the current state of the output pin
int buttonState;             // the current reading from the input pin
int lastButtonState = LOW;   // the previous reading from the input pin
int lastLEDState = LOW;      // the previous reading from the LED pin



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
    Serial.print("difference = ");
    Serial.print(difference);
    Serial.println(" milliseconds");
    msgCount = 0;
  }

  currAvg = avg(timeDifferences);
  currSD = sd(timeDifferences);
  if(millis() - startTime > currAvg + 2*currSD && ledState == LOW) 
      check_user(reading);
  else if(ledState == HIGH && lastLEDState == LOW){
    if (timeDifference.size() >= 10){
      timeDifferences.erase(timeDifferences.begin());
    }
    timeDifferences.push_back(difference);
  }
    
  lastLEDState = ledState;
  // save the reading. Next time through the loop, it'll be the lastButtonState:
  lastButtonState = reading;
  
}

void check_user(int n) {
    if(msgCount == 0) {
      Serial.println("You've been using the light for longer than usual. Should I turn it off?");
      Serial.println("Press y for yes and n for no.");
    }
    msgCount++;
<<<<<<< HEAD
  char num = Serial.read();
=======

  char num = Serial.read();

>>>>>>> c27e337a3e5ed7862326a85dc20efbe5828db856
    if(num == 'y'){
      Serial.println("LED is off"); 
      digitalWrite(ledPin, HIGH);
      ledState = HIGH;
      msgCount = 0;
<<<<<<< HEAD

=======
>>>>>>> c27e337a3e5ed7862326a85dc20efbe5828db856
    }
    else if (num=='n'){
      Serial.println("LED is on");  
      digitalWrite(ledPin, LOW);
      timeDifferences.push_back(difference);
      ledState = LOW;
    }
}



