#include"at93c46.h"
#include<Arduino.h>

void test_menu(){
    Serial.println("93c46 Test Menu");
    Serial.println("        1 - Dump Nvram");
    Serial.println("        2 - EWEN Write Enable");
    Serial.println("        3 - EWDS Write Disable");
    Serial.println("        4 - WRAL 0xA5");
    Serial.println("        5 - ERAL Erase ALL");
    Serial.println("        6 - Write 0:12, 34, 56, 78, 9A, BC, DE, FA");
    Serial.println("        7 - Write 0:22, 44, 66, 88, AA, BB, CC, DD");
    Serial.println("        8 - Erase 0, 3, 4, 5");
    Serial.println("        9 - Write sequential numbers (ASCII Characters)");
}

void seq_num(){
    for(uint8_t i = 0; i<128; i++){
        at_write(i, i, NSS_PIN, DI_PIN, DO_PIN, CLK_PIN);
    }
}

void pattern_one(){
    uint8_t arr_1[8] = {0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xFA};

    for(uint8_t i = 0; i<8; i++){
        at_write(i, arr_1[i], NSS_PIN, DI_PIN, DO_PIN, CLK_PIN);
    }
    for(uint8_t i = 0; i<128; i++){
        uint8_t rx_data = at_read(i, NSS_PIN, DI_PIN, DO_PIN, CLK_PIN);
        if((i % 16) == 0){
            Serial.println();
        }
        Serial.print(" ");
        Serial.print(rx_data, HEX);

    }
    
}

void pattern_two(){
    uint8_t arr_1[8] = {0x22, 0x44, 0x66, 0x88, 0xAA, 0xBB, 0xCC, 0xDD};

    for(uint8_t i = 0; i<8; i++){
        at_write(i, arr_1[i], NSS_PIN, DI_PIN, DO_PIN, CLK_PIN);
    }
    for(uint8_t i = 0; i<128; i++){
        uint8_t rx_data = at_read(i, NSS_PIN, DI_PIN, DO_PIN, CLK_PIN);
        if((i % 16) == 0){
            Serial.println();
        }
        Serial.print(" ");
        Serial.print(rx_data, HEX);

    }

}
void get_cmd(char cmd){
    switch (cmd)
    {
    case '1':
        Serial.println();
        Serial.println("CMD: 1");
        Serial.println("Dumping Nvram");
        Serial.println("8 bit mode: ");
        dump_nvram();
        break;
    case '2':
        Serial.println();
        Serial.println("CMD: 2");
        Serial.println("EWEN Write Enable");
        at_ewen(NSS_PIN, DI_PIN, DO_PIN, CLK_PIN);
        delay(10);
        Serial.println("EWEN Write Enabled");
        break;
    case '3':
        Serial.println();
        Serial.println("CMD: 3");
        Serial.println("EWDS Write Disable");
        at_ewds(NSS_PIN, DI_PIN, DO_PIN, CLK_PIN);
        delay(10);
        Serial.println("EWDS Write Disabled");
        break;
    case '4':
        Serial.println();
        Serial.println("CMD: 4");
        Serial.println("WRAL 0xA5");
        at_wral(0xA5, NSS_PIN, DI_PIN, DO_PIN, CLK_PIN);
        for(uint8_t i = 0; i < 128; i++){
            uint8_t d = at_read(i, NSS_PIN, DI_PIN, DO_PIN, CLK_PIN);
            if((i % 16) == 0){
                Serial.println();
            }
            Serial.print(" 0x");
            Serial.print(d, HEX);
        }
        break;
    case '5':
        Serial.println();
        Serial.println("CMD: 5");
        Serial.println("ERAl Erase All");
        at_eral(NSS_PIN, DI_PIN, DO_PIN, CLK_PIN);
        break;
    case '6':
        Serial.println();
        Serial.println("CMD: 6");
        Serial.println("Write 0:12, 34, 56, 78, 9A, BC, DE, FA");
        pattern_one();
        break;
    case '7':
        Serial.println();
        Serial.println("CMD: 7");
        Serial.println("Write 0:22, 44, 66, 88, AA, BB, CC, DD");
        pattern_two();
        break;
    case '8':
        Serial.println();
        Serial.println("CMD: 8");
        Serial.println("Erase 0, 3, 4, 5");
        at_erase(0, NSS_PIN, DI_PIN, DO_PIN, CLK_PIN);
        at_erase(3, NSS_PIN, DI_PIN, DO_PIN, CLK_PIN);
        at_erase(4, NSS_PIN, DI_PIN, DO_PIN, CLK_PIN);
        at_erase(5, NSS_PIN, DI_PIN, DO_PIN, CLK_PIN);
        break;
    case '9':
        Serial.println();
        Serial.println("CMD: 9");
        Serial.println("Write sequential numbers");
        seq_num();
        break;
    
    default:
        break;
    }
}

void dump_nvram(){
    Serial.println("Addresses:");
    for(uint8_t i = 0; i<128; i++){
        if((i % 16) == 0){
            Serial.println();
        }
        Serial.print(" ");
        Serial.print(i, HEX);

    }
    Serial.println();
    Serial.println("Content:");
    for(uint8_t i = 0; i<128; i++){

        uint8_t rx_data = at_read(i, NSS_PIN, DI_PIN, DO_PIN, CLK_PIN);
        if((i % 16) == 0){
            Serial.println();
        }
        Serial.print(" ");
        Serial.print((char)rx_data);

    }
}