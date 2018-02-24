
#define LEDPin 2
#define ButtonPin 3
bool mode = true;

void setup() {
  // put your setup code here, to run once:
  
  pinMode(LEDPin, OUTPUT);
  pinMode(ButtonPin, INPUT);
  
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(LEDPin, HIGH);
  int button = digitalRead(ButtonPin);

  if (button)
    digitalWrite(LEDPin, not mode);
}
