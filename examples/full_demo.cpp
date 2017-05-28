#include <Arduino.h>
#include "control.h"

//Definitions
#define lsensor 7
#define relay 13
#define light 11

//global variables
bool sensor = 0; // will be used to store the state of light sensor
tof timer(sensor, 1200000); // time to off - 1 200 000 ms = 20 min
ton timer2(sensor, 5000);
ctu counter(sensor, 4);
int mode = 2;

//setup function
void setup() {
        //Initialize serial console
        //Serial.begin(9600);
        //Serial.println("Program Start");
        pinMode(lsensor, INPUT);
        pinMode(relay, OUTPUT);
        pinMode(light, OUTPUT);
        timer.rst();
        timer2.rst();
        counter.rst();
}

//loop function
void loop() {
        //get light sensor status
        sensor = !(bool)digitalRead(lsensor);
        //set mode based on counter variable
        if (counter.cv() == 2) {
          mode = 0;
        }
        if (counter.cv() == 3) {
          mode = 1;
        }
        if (counter.cv() == 4) {
          mode = 2;
        }
        //reset counter when timer2 fires up
        if (timer2.out()) {
          counter.rst();
        }
        //mode selection
        if (mode == 0) {
          digitalWrite(relay, LOW);
          digitalWrite(light, LOW);
        }
        if (mode == 1) {
          digitalWrite(relay, HIGH);
          digitalWrite(light, HIGH);
        }
        if (mode == 2) {
          //assign timer output to digital out
          digitalWrite(relay, (uint8_t)timer.out());
          digitalWrite(light, !digitalRead(light));
          // in case timer2 won't fire up - reset counter when timer fires up
          if (!timer.out()) {
            counter.rst();
          }
        }
        delay(1000);
}
