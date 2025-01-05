#include "eeprom.h"
#include <Arduino.h>
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



