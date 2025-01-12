#include <stdint.h>
#ifndef at93c46_h
#define at93c46_h
#define MSB 1
#define LSB 0
#define NSS_PIN 2
#define DO_PIN  3
#define DI_PIN  4
#define CLK_PIN 5
/*Register Information*/
#define DDRB_n       *((volatile uint8_t*)0x24)
#define PORTB_n      *((volatile uint8_t*)0x25)
#define PINB_n       *((volatile uint8_t*)0x23)

/*CLK MACROs*/
#define CLK_HIGH(x)     PORTB_n |= (1 << x)
#define CLK_LOW(x)      PORTB_n &= ~(1 << x)

/*CLK MACROs*/
#define NSS_HIGH(x)     PORTB_n |= (1 << x)
#define NSS_LOW(x)      PORTB_n &= ~(1 << x)

/*Pin Setup*/
void at_init(uint8_t nss_pin, uint8_t di_pin, uint8_t do_pin, uint8_t clk_pin);

/*Spi Bit Bang functions*/
void spi_data_tx(uint8_t data_pin, uint8_t clk_pin, uint8_t bitorder, uint8_t tx_value);
uint8_t spi_data_rx(uint8_t data_pin, uint8_t clk_pin, uint8_t bitorder);

/*AT93c46 Instruction Sets*/
void at_ewen(uint8_t nss_pin, uint8_t di_pin, uint8_t do_pin, uint8_t clk_pin);
void at_ewds(uint8_t nss_pin, uint8_t di_pin, uint8_t do_pin, uint8_t clk_pin);
void at_eral(uint8_t nss_pin, uint8_t di_pin, uint8_t do_pin, uint8_t clk_pin);
void at_erase(uint8_t addr, uint8_t nss_pin, uint8_t di_pin, uint8_t do_pin, uint8_t clk_pin);
void at_wral(uint8_t data, uint8_t nss_pin, uint8_t di_pin, uint8_t do_pin, uint8_t clk_pin);
void at_write(uint8_t addr, uint8_t data, uint8_t nss_pin, uint8_t di_pin, uint8_t do_pin, uint8_t clk_pin);
uint8_t at_read(uint8_t addr, uint8_t nss_pin, uint8_t di_pin, uint8_t do_pin, uint8_t clk_pin);

/*AT93c46 Test API's*/
void test_menu();
void get_cmd(char cmd);
void dump_nvram();

#endif