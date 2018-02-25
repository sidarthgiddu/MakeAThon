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
void led_on_off(int ledPin, int buttonPin, int &buttonState, int &ledState, int &lastButtonState, int &lastLEDState, float &difference, float &endTime, float &startTime, double &currAvg, double &currSD, int &msgCount, vector<int> &timeDifferences);
void checkuser(int &ledPin, int &msgCount, int &ledState, vector<int> &timeDifferences, float &difference);


// Variables will change:
// LED1
float difference1=0.0;
float endTime1;
float startTime1;
double currAvg1 = 0;
double currSD1 = 0;
int msgCount1 = 0;

// LED2
float difference2=0.0;
float endTime2;
float startTime2;
double currAvg2 = 0;
double currSD2 = 0;
int msgCount2 = 0;

//LED3
float difference3=0.0;
float endTime3;
float startTime3;
double currAvg3 = 0;
double currSD3 = 0;
int msgCount3 = 0;

// States
int led1State = HIGH;         // the current state of the output pin
int button1State;             // the current reading from the input pin
int lastButton1State = LOW;   // the previous reading from the input pin
int lastLED1State = LOW;      // the previous reading from the LED pin

int led2State = HIGH;         // the current state of the output pin
int button2State;             // the current reading from the input pin
int lastButton2State = LOW;   // the previous reading from the input pin
int lastLED2State = LOW;      // the previous reading from the LED pin

int led3State = HIGH;         // the current state of the output pin
int button3State;             // the current reading from the input pin
int lastButton3State = LOW;   // the previous reading from the input pin
int lastLED3State = LOW;      // the previous reading from the LED pin



// the following variables are unsigned longs because the time, measured in
// milliseconds, will quickly become a bigger number than can be stored in an int.
unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
unsigned long debounceDelay = 50;    // the debounce time; increase if the output flickers


// array
vector<int> time1Differences;
vector<int> time2Differences;
vector<int> time3Differences;



void setup() {
  Serial.begin(9600);
  pinMode(button1Pin, INPUT);
  pinMode(button2Pin, INPUT);
  pinMode(button3Pin, INPUT);
  pinMode(led1Pin, OUTPUT);
  pinMode(led2Pin, OUTPUT);
  pinMode(led3Pin, OUTPUT);
  // set initial LED state
  digitalWrite(led1Pin, led1State);
  digitalWrite(led2Pin, led2State);
  digitalWrite(led3Pin, led3State);
}

void loop() {
  led_on_off( led1Pin,  button1Pin,  button1State,  led1State,  lastButton1State,  lastLED1State,  difference1,  endTime1,  startTime1,  currAvg1,  currSD1,  msgCount1, time1Differences);
  led_on_off( led2Pin,  button2Pin,  button2State,  led2State,  lastButton2State,  lastLED2State,  difference2,  endTime2,  startTime2,  currAvg2,  currSD2,  msgCount2, time2Differences);
  led_on_off( led3Pin,  button3Pin,  button3State,  led3State,  lastButton3State,  lastLED3State,  difference3,  endTime3,  startTime3,  currAvg3,  currSD3,  msgCount3, time3Differences);
  
}



void led_on_off(int ledPin, int buttonPin, int &buttonState, int &ledState, int &lastButtonState, int &lastLEDState, int &difference, int &endTime, int &startTime, double &currAvg, double &currSD, int &msgCount, vector<int> &timeDifferences){

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
    difference = (float)(endTime - startTime);
    Serial.print("difference = ");
    Serial.print(difference);
    Serial.println(" milliseconds");
    msgCount = 0;
  }

  currAvg = avg(timeDifferences);
  currSD = sd(timeDifferences);
  if(millis() - startTime > currAvg + 2*currSD && ledState == LOW) 
      check_user(ledPin, msgCount, ledState, timeDifferences, difference);
  else if(ledState == HIGH && lastLEDState == LOW){
    if (timeDifferences.size() >= 10){
      timeDifferences.erase(timeDifferences.begin());
    }
    timeDifferences.push_back(difference);
  }
    
  lastLEDState = ledState;
  // save the reading. Next time through the loop, it'll be the lastButtonState:
  lastButtonState = reading;
  
}

void check_user(int &ledPin, int &msgCount, int &ledState, vector<int> &timeDifferences, float &difference) {
    if(msgCount == 0) {
      Serial.print("You've been using the light "); 
      Serial.print(ledPin); 
      Serial.println(" for longer than usual. Should I turn it off?");
      Serial.print("Press ");
      Serial.print(ledPin);
      Serial.print(" for yes and "); 
      Serial.print("0");
      Serial.println(" for no.");
      msgCount++;
    }
    

  int num = Serial.read();

    if(num == ledPin){
      Serial.print("LED ");
      Serial.print(ledPin);  
      Serial.println("is off");
      digitalWrite(ledPin, HIGH);
      ledState = HIGH;
      msgCount = 0;
    }
    else if (num==0){
      Serial.print("LED ");
      Serial.print(ledPin);  
      Serial.println("is on"); 
      digitalWrite(ledPin, LOW);
      if (timeDifferences.size() >= 10){
        timeDifferences.erase(timeDifferences.begin());
      }
      timeDifferences.push_back(difference);
      ledState = LOW;
    }
}



