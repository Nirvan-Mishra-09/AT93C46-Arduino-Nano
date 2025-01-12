#include <Arduino.h>
#include "at93c46.h"


void setup(){
  Serial.begin(9600);
  at_init(NSS_PIN, DI_PIN, DO_PIN, CLK_PIN);
  test_menu();


}
void loop(){
  if(Serial.available()){
    char cmd = Serial.read();
    get_cmd(cmd);
  }

}