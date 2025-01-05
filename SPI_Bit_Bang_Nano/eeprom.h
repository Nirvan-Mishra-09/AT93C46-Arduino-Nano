#include <stdint.h>
#ifndef eeprom_h
#define eeprom_h
#define CS_PIN 10
#define SK_PIN 13
#define DO_PIN 11
#define DI_PIN 12
#define MSB 1
#define LSB 0
#define DDRB  *((volatile uint8_t*)0x24)
#define PORTB *((volatile uint8_t*)0x25)
#define PINB  *((volatile uint8_t*)0x23)

#define SCLK_HIGH(x)  PORTB |= (1 << x)
#define SCLK_LOW(x)   PORTB &= ~(1 << x)

#define NSS_HIGH(x)   PORTB |= (1 << x)
#define NSS_LOW(x)    PORTB &= ~(1 << x)

/*---------------EEPROM COMMANDS--------------*/
void init(uint8_t nss, 
          uint8_t di_bit, 
          uint8_t do_bit, 
          uint8_t sk);

void EWEN(uint8_t nss,
          uint8_t di_bit,
          uint8_t do_bit,
          uint8_t sk);

uint8_t READ( uint8_t addr,
              uint8_t nss,
              uint8_t di_bit,
              uint8_t do_bit,
              uint8_t sk);

void ERASE( uint8_t addr,
            uint8_t nss,
            uint8_t di_bit,
            uint8_t do_bit,
            uint8_t sk);

void WRITE( uint8_t addr,
            uint8_t data,
            uint8_t nss,
            uint8_t di_bit,
            uint8_t do_bit,
            uint8_t sk);

void WRAL(  uint8_t data,
            uint8_t nss,
            uint8_t di_bit,
            uint8_t do_bit,
            uint8_t sk);

void EWDS(  uint8_t nss,
            uint8_t di_bit,
            uint8_t do_bit,
            uint8_t sk);

void ERAL(  uint8_t nss,
            uint8_t di_bit,
            uint8_t do_bit,
            uint8_t sk);
/*--------SPI Bit Banging Functions---------*/
void spi_transfer(uint8_t dataPin,
                  uint8_t clockPin,
                  uint8_t bitodr,
                  uint8_t value);

uint8_t spi_read( uint8_t dataPin,
                  uint8_t clockPin,
                  uint8_t bitodr);
/*---------API Functions------------------ */
void test_menu();
void dumpNvram();
void seq_num();
void write_6_addr();
void write_7_addr();
void erase_at_loc();
void test_func(char cmd);
#endif 
