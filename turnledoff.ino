const int ledPin = 2;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);
}

void loop() {
  //open serial window with message, press 0 to turn the LED off, press 1 to turn the LED on
  Serial.println("Enter 0 to turn the LED off, enter 1 to turn the LED on");
  while(Serial.available() == 0){}
  char num = Serial.read();
  if(num != '0' && num != '1'){
    Serial.println("Error");  
  }
  else{
    if(num == '0'){
      Serial.println("LED is off"); 
      digitalWrite(ledPin, HIGH); 
    }
    else{
      Serial.println("LED is on");  
      digitalWrite(ledPin, LOW);
    }
  }
  
  //run the decision for 5 seconds (5000 ms)
  delay(5000);
  //repeat the loop
}
