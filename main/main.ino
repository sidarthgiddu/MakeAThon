#include <StandardCplusplus.h>
#include <system_configuration.h>
#include <unwind-cxx.h>
#include <utility.h>


#include "sd_avg.cpp"
#define ledPin 2
#define buttonPin 3

// Variables will change:
int ledState = HIGH;         // the current state of the output pin
int buttonState;             // the current reading from the input pin
int lastButtonState = LOW;   // the previous reading from the input pin
int lastLEDState = LOW;      // the previous reading from the LED pin


// the following variables are unsigned longs because the time, measured in
// milliseconds, will quickly become a bigger number than can be stored in an int.
unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
unsigned long debounceDelay = 50;    // the debounce time; increase if the output flickers


// array
double times[10]; 

void setup() {
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
  float difference;
   float startTime;
   float endTime;
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
  
  if(ledState == HIGH && lastLEDState == LOW){
    startTime = millis();
  }
  if(ledState == LOW && lastLEDState == HIGH){
    endTime = millis();
    difference = endTime - startTime;
    Serial.println(difference);
  }
  lastLEDState = ledState;
  
  // save the reading. Next time through the loop, it'll be the lastButtonState:
  lastButtonState = reading;
  
}


