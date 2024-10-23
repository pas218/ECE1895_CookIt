typedef struct
{
     int analogInputNumber;
     float expectVoltage;
}ingredient;

ingredient Burger  = {0, 1};
ingredient Lettuce = {0, 1};
ingredient Cheese  = {0, 1};
ingredient Onion   = {0, 1};

void setup() {
  pinMode(0, INPUT_PULLUP);
  pinMode(1, OUTPUT);
}

void loop() {
  
  digitalWrite(1, HIGH);
  delay(1000);
  chopIt();
}

void chopIt() {

  int sensorVal;

  while(true){
    sensorVal = digitalRead(0);

    if (sensorVal == LOW){
      digitalWrite(1, LOW);
      delay(2000);
      break;
    }
  }

}

void cookIt(){

}

void assembleIt(){

}