#include "eeprom.h"
#include <Arduino.h>

// #define CS_PIN 10
// #define SK_PIN 13
// #define DO_PIN 11
// #define DI_PIN 12

void setup(){
  Serial.begin(57600);
  init(CS_PIN, DI_PIN, DO_PIN, SK_PIN);
  test_menu();

}


void loop(){
  if(Serial.available()){
    char cmd = Serial.read();
    test_func(cmd);
  }
}



