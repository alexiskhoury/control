#include <Arduino.h>
#include "control.h"

//Definitions
#define lsensor 7
#define relay 13

//global variables
bool sensor = 0; // will be used to store the state of light sensor
tof timer(sensor, 1200000); // time to off - 1 200 000 ms = 20 min

//setup function
void setup() {
  pinMode(lsensor, INPUT);
  pinMode(relay, OUTPUT);
  timer.rst();
}

//loop function
void loop() {
  //get light sensor status
  sensor = !(bool)digitalRead(lsensor);
  //assign timer off output to digital out
  digitalWrite(relay, (uint8_t)timer.out());
  delay(1000);
}
