#include "at93c46_eeprom.h"
#include <util/delay.h>

void at_init(uint8_t nss_pin, uint8_t din_pin, uint8_t dout_pin, uint8_t clk_pin){
    // Pin Mapping
    uint8_t nss_bit = nss_pin - 8;
    uint8_t din_bit = din_pin - 8;
    uint8_t dout_bit = dout_pin - 8;
    uint8_t clk_bit = clk_pin - 8;

    //CLK, NSS, Dout as Output
    DDRB_n |= ((1 << nss_bit)|(1 << dout_bit)|(1 << clk_bit));
    //Din as Input
    DDRB_n &= ~(1 << din_bit);

    //NSS as LOW
    PORTB_n &= ~(1 << nss_bit);
}


void spi_transfer(uint8_t datapin, uint8_t clk_pin, uint8_t bitorder, uint8_t value){
    uint8_t clk_bit = clk_pin - 8;
    uint8_t dout_bit = datapin - 8;

    for(uint8_t i = 0; i<8; i++){
        if(bitorder == LSB){
            if(value & 1){
                PORTB_n |= (1 << dout_bit);
            }
            else{
                PORTB_n &= ~(1 << dout_bit);
            }
            value = value >> 1;
        }

        else{
            if(value & 128){
                PORTB_n |= (1 << dout_bit);
            }
            else{
                PORTB_n &= ~(1 << dout_bit);
            }
            value = value << 1;
        }
        CLK_HIGH(clk_bit);
        CLK_LOW(clk_bit);
    }
    
}

 uint8_t spi_read(uint8_t dataPin, uint8_t clk_pin, uint8_t bitorder){
    uint8_t clk_bit = clk_pin - 8;
    uint8_t din_bit = dataPin - 8;
    uint8_t rx_data = 0;

    for(uint8_t i = 0; i<8; i++){
        CLK_HIGH(clk_bit);
        _delay_ms(1);
        uint8_t pinb_read = ((PINB_n & (1 << din_bit)) ? 1 : 0);
        if(bitorder == LSB){
            rx_data |= (pinb_read << i);
        }
        else{
            rx_data |= (pinb_read << (7-i));
        }

        CLK_LOW(clk_bit);
        _delay_ms(1);
    }
    return rx_data;
 }

 void at_ewen(uint8_t nss_pin, uint8_t din_pin, uint8_t dout_pin, uint8_t clk_pin){
    uint8_t nss_bit = nss_pin - 8;
    NSS_HIGH(nss_bit);
    spi_transfer(dout_pin, clk_pin, MSB, 0b00000010);
    spi_transfer(dout_pin, clk_pin, MSB, 0b01100000);
    NSS_LOW(nss_bit);
    _delay_ms(2);
 }

 void at_ewds(uint8_t nss_pin, uint8_t din_pin, uint8_t dout_pin, uint8_t clk_pin){
    uint8_t nss_bit = nss_pin - 8;
    NSS_HIGH(nss_bit);
    spi_transfer(dout_pin, clk_pin, MSB, 0b00000010);
    spi_transfer(dout_pin, clk_pin, MSB, 0b00000000);
    NSS_LOW(nss_bit);
    _delay_ms(2);
 }

 void at_eral(uint8_t nss_pin, uint8_t din_pin, uint8_t dout_pin, uint8_t clk_pin){
    uint8_t nss_bit = nss_pin - 8;
    at_ewen(nss_pin, din_pin, dout_pin, clk_pin);
    NSS_HIGH(nss_bit);
    spi_transfer(dout_pin, clk_pin, MSB, 0b00000010);
    spi_transfer(dout_pin, clk_pin, MSB, 0b01000000);
    NSS_LOW(nss_bit);
    _delay_ms(2);
    at_ewds(nss_pin, din_pin, dout_pin, clk_pin);
 }

 void at_erase(uint8_t addr, uint8_t nss_pin, uint8_t din_pin, uint8_t dout_pin, uint8_t clk_pin){
    uint8_t nss_bit = nss_pin - 8;
    if(addr < 128){
        at_ewen(nss_pin, din_pin, dout_pin, clk_pin);
        NSS_HIGH(nss_bit);
        spi_transfer(dout_pin, clk_pin, MSB, 0b00000011);
        addr |= (1UL << 7);
        spi_transfer(dout_pin, clk_pin, MSB, addr);
        NSS_LOW(nss_bit);
        _delay_ms(2);
        at_ewds(nss_pin, din_pin, dout_pin, clk_pin);
    }
 }

 void at_wral(uint8_t data, uint8_t nss_pin, uint8_t din_pin, uint8_t dout_pin, uint8_t clk_pin){
    uint8_t nss_bit = nss_pin - 8;
    at_ewen(nss_pin, din_pin, dout_pin, clk_pin);
    NSS_HIGH(nss_bit);
    spi_transfer(dout_pin, clk_pin, MSB, 0b00000010);
    spi_transfer(dout_pin, clk_pin, MSB, 0b00100000);
    spi_transfer(dout_pin, clk_pin, MSB, data);
    NSS_LOW(nss_bit);
    _delay_ms(5);
    at_ewds(nss_pin,din_pin, dout_pin, clk_pin);
}

void at_write(uint8_t addr, uint8_t data, uint8_t nss_pin, uint8_t din_pin, uint8_t dout_pin, uint8_t clk_pin){
    uint8_t nss_bit = nss_pin - 8;
    if(addr < 128){
        at_ewen(nss_pin, din_pin, dout_pin, clk_pin);
        NSS_HIGH(nss_bit);
        spi_transfer(dout_pin, clk_pin, MSB, 0b00000010);
        addr |= (1 << 7);
        spi_transfer(dout_pin, clk_pin, MSB, addr);
        spi_transfer(dout_pin, clk_pin, MSB, data);
        NSS_LOW(nss_bit);
        _delay_ms(5);
        at_ewds(nss_pin, din_pin, dout_pin, clk_pin);
    }
}

uint8_t at_read(uint8_t addr, uint8_t nss_pin, uint8_t din_pin, uint8_t dout_pin, uint8_t clk_pin){
    uint8_t rx_data = 0;
    uint8_t nss_bit = nss_pin - 8;
    if(addr < 128){
        NSS_HIGH(nss_bit);
        _delay_ms(1);
        spi_transfer(dout_pin, clk_pin, MSB, 0b00000011);
        spi_transfer(dout_pin, clk_pin, MSB, addr);
        rx_data = spi_read(din_pin, clk_pin, MSB);
        NSS_LOW(nss_bit);
        _delay_ms(1);
    }
    return rx_data;
}
