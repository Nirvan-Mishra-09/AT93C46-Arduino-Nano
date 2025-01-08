#include<Arduino.h>
#include "at93c46_eeprom.h"

void setup(){
    Serial.begin(9600);
    at_init(NSS_PIN, DI_PIN, DO_PIN, CLK_PIN);
    // at_write(14, 0x54, NSS_PIN, DI_PIN, DO_PIN, CLK_PIN);
    // uint8_t d = at_read(14, NSS_PIN, DI_PIN, DO_PIN, CLK_PIN);

    // Serial.print("0x");
    // Serial.println(d, HEX);
    test_menu();

    

}
void loop(){
    if(Serial.available()){
        char cmd = Serial.read();
        get_cmd(cmd);
    }

}