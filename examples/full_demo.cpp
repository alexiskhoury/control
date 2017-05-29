#include <Arduino.h>
#include "control.h"

//Definitions
#define lsensor 7
#define relay 13
#define light 11

//global variables
bool sensor = 0; // will be used to store the state of light sensor
bool lstatus = 0; //will be used to trigger the light rising edge
trigger trig(sensor);
tof timer(sensor, 1200000); // time to off - 1 200 000 ms = 20 min
ton timer2(lstatus, 10000);
ctu counter(sensor, 4);
blink blinker(1000,1000);
int mode = 0;

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
        //execute blinker code
        blinker.out();
        //ton must have a fixed status input
        if (trig.redge()) {
          lstatus = 1;
          /*Serial.print("redge detected, counter: ");
          Serial.print(counter.cv());
          Serial.print(" ; mode: ");
          Serial.println(mode);*/
        }
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
          /*Serial.print("timer2 on fires up, counter: ");
          Serial.print(counter.cv());
          Serial.print(" ; mode: ");
          Serial.println(mode);*/
          counter.rst();
          lstatus = 0;
          timer2.rst();

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
          digitalWrite(light, blinker.out());
        }
        delay(200);
}
