#include <Keyboard.h>
#include <SoftwareSerial.h>

SoftwareSerial mySUART(10, 11);

void setup() {
  mySUART.begin(9600);
  pinMode(2, INPUT_PULLUP);
  pinMode(13, OUTPUT);
  Keyboard.begin();
}

void loop() {
  int sensorVal = digitalRead(2);
  if (sensorVal == LOW) {
    digitalWrite(13, HIGH);
    if (mySUART.available() > 0) {
      Keyboard.write((char) mySUART.read());
    }
  }
  else {
    digitalWrite(13, LOW);
  }
}
