#include "at93c46.h"
#include <util/delay.h>

void at_init(uint8_t nss_pin, uint8_t di_pin, uint8_t do_pin, uint8_t clk_pin){
    //NSS, CLK, DO as OUTPUT
    DDRB_n |= ((1 << nss_pin)|(1 << do_pin)|(1 << clk_pin));
    //DI as INPUT
    DDRB_n &= ~(1 << di_pin);
    //NSS as LOW
    PORTB_n &= ~(1 << nss_pin);
}

void spi_data_tx(uint8_t data_pin, uint8_t clk_pin, uint8_t bitorder, uint8_t tx_value){
    for(uint8_t i = 0; i<8; i++){
        if(bitorder == LSB){
            if (tx_value & 1){
                PORTB_n |= (1 << data_pin);
            }
            else{
                PORTB_n &= ~(1 << data_pin);
            }
            tx_value = tx_value >> 1;
        }
        else{
            if (tx_value & 0x80){
                PORTB_n |= (1 << data_pin);
            }
            else{
                PORTB_n &= ~(1 << data_pin);
            }
            tx_value = tx_value << 1;

        }
        CLK_HIGH(clk_pin);
        CLK_LOW(clk_pin);
    }
}

uint8_t spi_data_rx(uint8_t data_pin, uint8_t clk_pin, uint8_t bitorder){
    uint8_t rx_data = 0;
    for(uint8_t i = 0; i<8; i++){
        CLK_HIGH(clk_pin);
        _delay_ms(1);
        uint8_t pinb_read = ((PINB_n & (1 << data_pin)) ? 1 : 0);
        if(bitorder == LSB){
            rx_data |= (pinb_read << i);
        }
        else{
            rx_data |= (pinb_read << (7-i));
        }
        CLK_LOW(clk_pin);
        _delay_ms(1);
    }
    return rx_data;
}

void at_ewen(uint8_t nss_pin, uint8_t di_pin, uint8_t do_pin, uint8_t clk_pin){
    NSS_HIGH(nss_pin);
    spi_data_tx(do_pin, clk_pin, MSB, 0x02);
    spi_data_tx(do_pin, clk_pin, MSB, 0x60);
    NSS_LOW(nss_pin);
    _delay_ms(2);
}
void at_ewds(uint8_t nss_pin, uint8_t di_pin, uint8_t do_pin, uint8_t clk_pin){
    NSS_HIGH(nss_pin);
    spi_data_tx(do_pin, clk_pin, MSB, 0x02);
    spi_data_tx(do_pin, clk_pin, MSB, 0x00);
    NSS_LOW(nss_pin);
    _delay_ms(2);
}
void at_eral(uint8_t nss_pin, uint8_t di_pin, uint8_t do_pin, uint8_t clk_pin){
    at_ewen(nss_pin, di_pin, do_pin, clk_pin);
    NSS_HIGH(nss_pin);
    spi_data_tx(do_pin, clk_pin, MSB, 0x02);
    spi_data_tx(do_pin, clk_pin, MSB, 0x40);
    NSS_LOW(nss_pin);
    _delay_ms(2);
    at_ewds(nss_pin, di_pin, do_pin, clk_pin);
}
void at_erase(uint8_t addr, uint8_t nss_pin, uint8_t di_pin, uint8_t do_pin, uint8_t clk_pin){
    if(addr < 128){
        at_ewen(nss_pin, di_pin, do_pin, clk_pin);
        NSS_HIGH(nss_pin);
        spi_data_tx(do_pin, clk_pin, MSB, 0x03);
        addr |= (1 << 7);
        spi_data_tx(do_pin, clk_pin, MSB, addr);
        NSS_LOW(nss_pin);
        _delay_ms(2);
        at_ewds(nss_pin, di_pin, do_pin, clk_pin);
    }
}

void at_wral(uint8_t data, uint8_t nss_pin, uint8_t di_pin, uint8_t do_pin, uint8_t clk_pin){
    at_ewen(nss_pin, di_pin, do_pin, clk_pin);
    NSS_HIGH(nss_pin);
    spi_data_tx(do_pin, clk_pin, MSB, 0x02);
    spi_data_tx(do_pin, clk_pin, MSB, 0x20);
    spi_data_tx(do_pin, clk_pin, MSB, data);
    NSS_LOW(nss_pin);
    _delay_ms(5);
    at_ewds(nss_pin, di_pin, do_pin, clk_pin);
}

void at_write(uint8_t addr, uint8_t data, uint8_t nss_pin, uint8_t di_pin, uint8_t do_pin, uint8_t clk_pin){
    if(addr < 128){
        at_ewen(nss_pin, di_pin, do_pin, clk_pin);
        NSS_HIGH(nss_pin);
        spi_data_tx(do_pin, clk_pin, MSB, 0x02);
        addr |= (1 << 7);
        spi_data_tx(do_pin, clk_pin, MSB, addr);
        spi_data_tx(do_pin, clk_pin, MSB, data);
        NSS_LOW(nss_pin);
        _delay_ms(5);
        at_ewds(nss_pin, di_pin, do_pin, clk_pin);
    }
}

uint8_t at_read(uint8_t addr, uint8_t nss_pin, uint8_t di_pin, uint8_t do_pin, uint8_t clk_pin){
    uint8_t rx_value = 0;
    if(addr < 128){
        NSS_HIGH(nss_pin);
        _delay_ms(1);
        spi_data_tx(do_pin, clk_pin, MSB, 0x03);
        spi_data_tx(do_pin, clk_pin, MSB, addr);
        rx_value = spi_data_rx(di_pin, clk_pin, MSB);
        NSS_LOW(nss_pin);
        _delay_ms(1);

    }
    return rx_value;

}