#include "eeprom.h"
#include <stdint.h>
#include <Arduino.h>
#include <util/delay.h>

/*----------------------SPI BIT BANGING FUNCTIONS-----------------------------------*/

uint8_t spi_read(uint8_t dataPin, uint8_t clockPin, uint8_t bitodr){
  uint8_t rx_value = 0;
  uint8_t di_pin = dataPin - 8;
  uint8_t sk_pin = clockPin - 8;

  for(uint8_t i = 0; i<8; i++){
    SCLK_HIGH(sk_pin);
    _delay_ms(1);

    uint8_t PINB_status = ((PINB & (1 << di_pin)) ? 1 : 0);
    if(bitodr == LSB){
      rx_value |= (PINB_status << i);
    } 
    else {
      rx_value |= (PINB_status << (7 - i));
    }
    SCLK_LOW(sk_pin);
    _delay_ms(1);
  }
  return rx_value;
}

void spi_transfer(uint8_t dataPin, uint8_t clockPin, uint8_t bitodr, uint8_t value){
  uint8_t sk_pin = clockPin - 8;
  uint8_t do_pin = dataPin - 8;
  
  for(uint8_t i = 0; i<8; i++){
    if(bitodr == LSB){
      if(value & 1){
        PORTB |= (1 << do_pin);
      }
      else{
        PORTB &= ~(1 << do_pin);
      }
      value = value >> 1;
    }
    else{
      if(value & 128){
        PORTB |= (1<<do_pin);
      }
      else{
        PORTB &= ~(1<<do_pin);
      }
      value = value << 1;

    }
    SCLK_HIGH(sk_pin);
    SCLK_LOW(sk_pin);
  }
}
/*----------------------EEPROM COMMANDS-----------------------------------*/

void init(uint8_t nss, uint8_t di_bit, uint8_t do_bit, uint8_t sk){
  /*Pin Mapping*/
  uint8_t nss_pin = nss - 8;
  uint8_t sk_pin = sk - 8;
  uint8_t di_pin = di_bit - 8;
  uint8_t do_pin = do_bit - 8;
  // NSS, SK, DO as output
  DDRB |= ((1<<nss_pin)|(1<<sk_pin)|(1<<do_pin));
  //DI as input
  DDRB &= ~(1<<di_pin);
  //NSS LOW
  PORTB &= ~(1<<nss_pin);
}

void EWEN(uint8_t nss, uint8_t di_bit, uint8_t do_bit, uint8_t sk){
  uint8_t nss_pin = nss - 8;
  NSS_HIGH(nss_pin);
  spi_transfer(do_bit, sk, MSB, 0b00000010);
  spi_transfer(do_bit, sk, MSB, 0b01100000);
  NSS_LOW(nss_pin);
  _delay_ms(2);
}

uint8_t READ(uint8_t addr, uint8_t nss, uint8_t di_bit, uint8_t do_bit, uint8_t sk){
  uint8_t nss_pin = nss - 8;
  uint8_t rx_data = 0;
  if(addr<128){
    NSS_HIGH(nss_pin);
    spi_transfer(do_bit, sk, MSB, 0b00000011);
    spi_transfer(do_bit, sk, MSB, addr);
    rx_data = spi_read(di_bit, sk , MSB);
    NSS_LOW(nss_pin);
    _delay_ms(2);
  }
  else{
    Serial.println("[READ] : Address exceeds 128!!");
  }
  return rx_data;

}

void EWDS(uint8_t nss, uint8_t di_bit, uint8_t do_bit, uint8_t sk){
  uint8_t nss_pin = nss - 8;
  NSS_HIGH(nss_pin);
  spi_transfer(do_bit, sk, MSB, 0b00000010);
  spi_transfer(do_bit, sk, MSB, 0b00000000);
  NSS_LOW(nss_pin);
  _delay_ms(2);
}

void ERAL(uint8_t nss, uint8_t di_bit, uint8_t do_bit, uint8_t sk){
  uint8_t nss_pin = nss - 8;
  EWEN(nss, di_bit, do_bit, sk);
  NSS_HIGH(nss_pin);
  spi_transfer(do_bit, sk, MSB, 0b00000010);
  spi_transfer(do_bit, sk, MSB, 0b01000000);
  NSS_LOW(nss_pin);
  _delay_ms(5);
  EWDS(nss, di_bit, do_bit, sk);
}

void WRITE(uint8_t addr, uint8_t data, uint8_t nss, uint8_t di_bit, uint8_t do_bit, uint8_t sk){
  uint8_t nss_pin = nss - 8;
  if(addr < 128){
    EWEN(nss, di_bit, do_bit, sk);
    NSS_HIGH(nss_pin);
    spi_transfer(do_bit, sk, MSB, 0b00000010);
    addr |= (1<<7);
    spi_transfer(do_bit, sk, MSB, addr);
    spi_transfer(do_bit, sk, MSB, data);
    NSS_LOW(nss_pin);
    _delay_ms(5);
    EWDS(nss, di_bit, do_bit, sk);
  }
  else{
    Serial.println("[WRITE] : Address exceeds 128!!");
  }
}

void ERASE(uint8_t addr, uint8_t nss, uint8_t di_bit, uint8_t do_bit, uint8_t sk){
  uint8_t nss_pin = nss - 8;

  if(addr<128){
    EWEN(nss,di_bit,do_bit, sk);
    NSS_HIGH(nss_pin);
    spi_transfer(do_bit, sk, MSB, 0b00000011);
    addr |= (1 << 7);
    spi_transfer(do_bit, sk, MSB, addr);
    NSS_LOW(nss_pin);
    EWDS(nss, di_bit, do_bit, sk);
  }
  else{
    Serial.print("Address exceeds 128!!");
  }
}

void WRAL(uint8_t data, uint8_t nss, uint8_t di_bit, uint8_t do_bit, uint8_t sk){
  uint8_t nss_pin = nss - 8;
    EWEN(nss, di_bit, do_bit, sk);
    NSS_HIGH(nss_pin);
    spi_transfer(do_bit, sk, MSB, 0b00000010);
    spi_transfer(do_bit, sk, MSB, 0b00100000);
    spi_transfer(do_bit, sk, MSB, data);
    NSS_LOW(nss_pin);
    _delay_ms(5);
    EWDS(nss, di_bit, do_bit, sk);
}

/*----------------------TEST API FUNCTIONS-----------------------------------*/

void test_menu(){
  Serial.println("93c46 Test Menu: ");
  Serial.println("        1 - Dump Nvram");
  Serial.println("        2 - EWEN Write Enable");
  Serial.println("        3 - EWDS Write Disable");
  Serial.println("        4 - WRAL 0xA5");
  Serial.println("        5 - ERAL Erase ALL");
  Serial.println("        6 - Write 0:12, 34, 56, 78, 9A, BC, DE, FA");
  Serial.println("        7 - Write 0:22, 44, 66, 88, AA, BB, CC, DD");
  Serial.println("        8 - Erase 0, 3, 4, 7");
  Serial.println("        9 - Write Sequential numbers");
}

void dumpNvram(){
  Serial.println("1 - Dump Nvram");
  Serial.println("8 bit mode: ");
  for(uint8_t i = 0; i<128; i++){
    WRITE(i, i, CS_PIN, DI_PIN, DO_PIN, SK_PIN);
    delay(5);
  }
  delay(1000);
  for(uint8_t i = 0; i<128; i++){
    uint8_t d = READ(i, CS_PIN, DI_PIN, DO_PIN, SK_PIN);
    delay(5);
    Serial.print(i);
    Serial.print(" : ");
    Serial.println((char)d);
  }
}

void seq_num(){
  Serial.println("9 - Sequential Numbers");
  for(uint8_t i = 0; i<128; i++){
    WRITE(i, i, CS_PIN, DI_PIN, DO_PIN, SK_PIN);
    delay(5);
  }
  delay(1000);
  for(uint8_t i = 0; i<128; i++){
    uint8_t d = READ(i, CS_PIN, DI_PIN, DO_PIN, SK_PIN);
    delay(5);
    Serial.print(i);
    Serial.print(" : ");
    Serial.println(d);
  }
}
void write_6_addr(){
    WRITE(0x12, 0x12, CS_PIN, DI_PIN, DO_PIN, SK_PIN);
    WRITE(0x34, 0x34, CS_PIN, DI_PIN, DO_PIN, SK_PIN);
    WRITE(0x56, 0x56, CS_PIN, DI_PIN, DO_PIN, SK_PIN);
    WRITE(0x78, 0x78, CS_PIN, DI_PIN, DO_PIN, SK_PIN);
    WRITE(0x94, 0x9A, CS_PIN, DI_PIN, DO_PIN, SK_PIN);
    WRITE(0xBC, 0xBC, CS_PIN, DI_PIN, DO_PIN, SK_PIN);
    WRITE(0xDE, 0XDE, CS_PIN, DI_PIN, DO_PIN, SK_PIN);
    WRITE(0xFA, 0XFA, CS_PIN, DI_PIN, DO_PIN, SK_PIN);
    delay(5);
    uint8_t d0 = READ(0x12, CS_PIN, DI_PIN, DO_PIN, SK_PIN);
    Serial.print("0x");
    Serial.println(d0, HEX);
    uint8_t d1 = READ(0x34, CS_PIN, DI_PIN, DO_PIN, SK_PIN);
    Serial.print("0x");
    Serial.println(d1, HEX);
    uint8_t d2 = READ(0x56, CS_PIN, DI_PIN, DO_PIN, SK_PIN);
    Serial.print("0x");
    Serial.println(d2, HEX);
    uint8_t d3 = READ(0x78, CS_PIN, DI_PIN, DO_PIN, SK_PIN);
    Serial.print("0x");
    Serial.println(d3, HEX);
    uint8_t d4 = READ(0x9A, CS_PIN, DI_PIN, DO_PIN, SK_PIN);
    Serial.print("0x");
    Serial.println(d4, HEX);
    uint8_t d5 = READ(0xBC, CS_PIN, DI_PIN, DO_PIN, SK_PIN);
    Serial.print("0x");
    Serial.println(d5, HEX); 
    uint8_t d6 = READ(0XDE, CS_PIN, DI_PIN, DO_PIN, SK_PIN);
    Serial.print("0x");
    Serial.println(d6, HEX); 
    uint8_t d7 = READ(0XFA, CS_PIN, DI_PIN, DO_PIN, SK_PIN);
    Serial.print("0x");
    Serial.println(d7, HEX); 
}

void write_7_addr(){
    WRITE(0x22, 0x22, CS_PIN, DI_PIN, DO_PIN, SK_PIN);
    WRITE(0x44, 0x44, CS_PIN, DI_PIN, DO_PIN, SK_PIN);
    WRITE(0x66, 0x66, CS_PIN, DI_PIN, DO_PIN, SK_PIN);
    WRITE(0x88, 0x88, CS_PIN, DI_PIN, DO_PIN, SK_PIN);
    WRITE(0xAA, 0xAA, CS_PIN, DI_PIN, DO_PIN, SK_PIN);
    WRITE(0xBB, 0xBB, CS_PIN, DI_PIN, DO_PIN, SK_PIN);
    WRITE(0xCC, 0XCC, CS_PIN, DI_PIN, DO_PIN, SK_PIN);
    WRITE(0xDD, 0XDD, CS_PIN, DI_PIN, DO_PIN, SK_PIN);
    delay(5);
    uint8_t d0 = READ(0x22, CS_PIN, DI_PIN, DO_PIN, SK_PIN);
    Serial.print("0x");
    Serial.println(d0, HEX);
    uint8_t d1 = READ(0x44, CS_PIN, DI_PIN, DO_PIN, SK_PIN);
    Serial.print("0x");
    Serial.println(d1, HEX);
    uint8_t d2 = READ(0x66, CS_PIN, DI_PIN, DO_PIN, SK_PIN);
    Serial.print("0x");
    Serial.println(d2, HEX);
    uint8_t d3 = READ(0x88, CS_PIN, DI_PIN, DO_PIN, SK_PIN);
    Serial.print("0x");
    Serial.println(d3, HEX);
    uint8_t d4 = READ(0xAA, CS_PIN, DI_PIN, DO_PIN, SK_PIN);
    Serial.print("0x");
    Serial.println(d4, HEX);
    uint8_t d5 = READ(0xBB, CS_PIN, DI_PIN, DO_PIN, SK_PIN);
    Serial.print("0x");
    Serial.println(d5, HEX); 
    uint8_t d6 = READ(0XCC, CS_PIN, DI_PIN, DO_PIN, SK_PIN);
    Serial.print("0x");
    Serial.println(d6, HEX); 
    uint8_t d7 = READ(0XDD, CS_PIN, DI_PIN, DO_PIN, SK_PIN);
    Serial.print("0x");
    Serial.println(d7, HEX); 
}

void erase_at_loc(){
  ERASE(0x00, CS_PIN, DI_PIN, DO_PIN, SK_PIN);
  ERASE(0x03, CS_PIN, DI_PIN, DO_PIN, SK_PIN);
  ERASE(0x04, CS_PIN, DI_PIN, DO_PIN, SK_PIN);
  ERASE(0x07, CS_PIN, DI_PIN, DO_PIN, SK_PIN);
  uint8_t d4 = READ(0x00, CS_PIN, DI_PIN, DO_PIN, SK_PIN);
  Serial.print("0x");
  Serial.println(d4, HEX);
  uint8_t d5 = READ(0x03, CS_PIN, DI_PIN, DO_PIN, SK_PIN);
  Serial.print("0x");
  Serial.println(d5, HEX); 
  uint8_t d6 = READ(0X04, CS_PIN, DI_PIN, DO_PIN, SK_PIN);
  Serial.print("0x");
  Serial.println(d6, HEX); 
  uint8_t d7 = READ(0X07, CS_PIN, DI_PIN, DO_PIN, SK_PIN);
  Serial.print("0x");
  Serial.println(d7, HEX); 

}

void test_func(char cmd){
  switch(cmd){
    case '1':
      dumpNvram();
      break;
    case '2':
      EWEN(CS_PIN, DI_PIN, DO_PIN, SK_PIN);
      break;
    case '3':
      EWDS(CS_PIN, DI_PIN, DO_PIN, SK_PIN);
      break;
    case '4':
      WRAL(0xA5, CS_PIN, DI_PIN, DO_PIN, SK_PIN);
      break;
    case '5':
      ERAL(CS_PIN, DI_PIN, DO_PIN, SK_PIN);
      break;
    case '6':
      write_6_addr();
      break;
    case '7':
      write_7_addr();
      break;
    case '8':
      erase_at_loc();
      break;
    case '9':
      seq_num();
      break;
  }
}

