#include <StandardCplusplus.h>
#include <system_configuration.h>
#include <unwind-cxx.h>
#include <utility.h>
using namespace std;
//#include "sd_avg.cpp"


#define ledPin 2
#define buttonPin 3

// Variables will change:
int ledState = HIGH;         // the current state of the output pin
int buttonState;             // the current reading from the input pin
int lastButtonState = LOW;   // the previous reading from the input pin
int lastLEDState = LOW;      // the previous reading from the LED pin
float difference;
   float endTime;
   float startTime;

// the following variables are unsigned longs because the time, measured in
// milliseconds, will quickly become a bigger number than can be stored in an int.
unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
unsigned long debounceDelay = 50;    // the debounce time; increase if the output flickers


// array
double timeDifferences[10]; 

double sd(vector<int> v){
    double sum = 0.0, mean, standardDeviation = 0.0;
    
    
    for(vector<int>::iterator it = v.begin(); it != v.end(); it++)
    {
        sum += *it;
    }
    
    mean = sum/v.size();
    
    for(auto it = v.begin(); it != v.end(); it++){
        standardDeviation += pow(*it - mean, 2);
    }
    
    return sqrt(standardDeviation / v.size());
}

double avg(vector<int> v){
    double sum = 0.0;
    for(vector<int>::iterator it = v.begin(); it != v.end(); it++)
    {
        sum += *it;
    }
    return sum/v.size();
}

bool abnormal(int value, double sd, double avg){
    if (value >= 2*sd + avg){
        return true;
    }
    return false;
}

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
  
  if(ledState == HIGH && lastLEDState == LOW){
    startTime = millis();
    Serial.print("startTime =");
    Serial.println(startTime);
  }
  else if(ledState == LOW && lastLEDState == HIGH){
    endTime = millis();
    Serial.print("endTime =");
    Serial.println(endTime);
    difference = (endTime - startTime)/1000.0;
    Serial.print("difference =");
    Serial.println(difference);
  }
  lastLEDState = ledState;
  
  // save the reading. Next time through the loop, it'll be the lastButtonState:
  lastButtonState = reading;
  
}


