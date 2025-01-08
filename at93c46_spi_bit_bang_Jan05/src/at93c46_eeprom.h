#include <stdint.h>
#ifndef at93c46_eeprom_h
#define at93c46_eeprom_h
#define MSB 1
#define LSB 0
#define NSS_PIN 10
#define DO_PIN  11
#define DI_PIN  12
#define CLK_PIN 13
/*Register Info*/
#define DDRB_n  *((volatile uint8_t*)0x24)
#define PORTB_n *((volatile uint8_t*)0x25)
#define PINB_n  *((volatile uint8_t*)0x23)

/*CLOCK MACRO*/
#define CLK_HIGH(x)     PORTB_n |= (1 << x)
#define CLK_LOW(x)      PORTB_n &= ~(1 << x)

/*Chip Select MACRO*/
#define NSS_HIGH(x)     PORTB_n |= (1 << x)
#define NSS_LOW(x)      PORTB_n &= ~(1 << x)

/* Pin Connection*/
void at_init(uint8_t nss_pin, uint8_t in_pin, uint8_t out_pin, uint8_t clk_pin);

/*SPI functions*/
void spi_transfer(uint8_t datapin, uint8_t clk_pin, uint8_t bitorder, uint8_t value);
uint8_t spi_read(uint8_t dataPin, uint8_t clk_pin, uint8_t bitorder);

/*AT93c46 Instructions*/
void at_ewen(uint8_t nss_pin, uint8_t din_pin, uint8_t dout_pin, uint8_t clk_pin);
void at_ewds(uint8_t nss_pin, uint8_t din_pin, uint8_t dout_pin, uint8_t clk_pin);
void at_eral(uint8_t nss_pin, uint8_t din_pin, uint8_t dout_pin, uint8_t clk_pin);
void at_erase(uint8_t addr, uint8_t nss_pin, uint8_t din_pin, uint8_t dout_pin, uint8_t clk_pin);
void at_wral(uint8_t data, uint8_t nss_pin, uint8_t din_pin, uint8_t dout_pin, uint8_t clk_pin);
void at_write(uint8_t addr, uint8_t data, uint8_t nss_pin, uint8_t din_pin, uint8_t dout_pin, uint8_t clk_pin);
uint8_t at_read(uint8_t addr, uint8_t nss_pin, uint8_t din_pin, uint8_t dout_pin, uint8_t clk_pin);

/*Test Menu*/
void test_menu();
void get_cmd(char cmd);
void dump_nvram();
void seq_num();
void pattern_one();
void pattern_two();
#endif