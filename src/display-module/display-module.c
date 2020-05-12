#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>

#include "../../include/sensor-data.h"
#include "../../include/radio_rx.h"

void init_modules(void);
void loop(void);
struct sensor_data get_data(void);

int main(void) {
    init_modules();

    loop();
}

loop() {
    while(1) {
        struct sensor_data data = get_data();

        check_data(data);

        display_data(data);
    }
}

void init_modules() {
    init_radio();
    init_bluetooth();
    init_display();

    spi_init_master();
}

uint16_t combine_chars(unsigned char low, unsigned char high) {
    return (low | (high << 8));
}

struct sensor_data get_data() {
    struct sensor_data data;
    unsigned char low, high;
    // get acc data
    low = receive_data();
    high = receive_data();
    data.acc_data.x = combine_chars(low, high);
    low = receive_data();
    high = receive_data();
    data.acc_data.y = combine_chars(low, high);
    low = receive_data();
    high = receive_data();
    data.acc_data.z = combine_chars(low, high);

    // get gps data
    low = receive_data();
    high = receive_data();
    data.gps_data.latitude = combine_chars(low, high);
    low = receive_data();
    high = receive_data();
    data.gps_data.longitude = combine_chars(low, high);
    data.gps_data.N = receive_data();
    data.gps_data.E = receive_data();

    // get temp data
    data.temp_data.temp = receive_data();

    // get pulse data
    data.pulse_data.pulse = receive_data();

    // get float data
    data.float_data.drowning = receive_data();

    return data;
}

// Initialize SPI Master Device (with SPI interrupt)
void spi_init_master (void)
{
    // Set MOSI, SCK as Output
    DDRB=(1<<5)|(1<<3);
 
    // Enable SPI, Set as Master
    // Prescaler: Fosc/16, Enable Interrupts
    //The MOSI, SCK pins are as per ATMega8
    SPCR=(1<<SPE)|(1<<MSTR)|(1<<SPR0)|(1<<SPIE);
 
    // Enable Global Interrupts
    sei();
}