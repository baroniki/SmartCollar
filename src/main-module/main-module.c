#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <stdlib.h>

#include "../../include/sensor-data.h"
#include "../../include/radio_tx.h"

void loop (void);
void init_sensors(void);
void read_data(void);
void check_data(void);
void send_data(void);
void spi_init_master (void);

struct sensor_data data;

int main(void) {
    init_sensors();

    loop();
    return 0;
}

void loop() {
    while(1) {
        read_data();

        check_data();

        send_data();
    }
}

void read_data() {
    data.acc_data = read_acc();
    data.gps_data = read_gps();
    data.temp_data = read_temp_sensor();
    data.pulse_data = read_pulse_sensor();
    data.float_data = read_float_level();
}



void init_sensors() {
    init_acc();
    init_gps();
    init_temp_sensor();
    init_pulse_sensor();
    init_float_level();
    init_led();
    init_buzzer();
    init_radio_tx();

    spi_init_master();
}

unsigned char get_upper_bits(uint16_t d) {
    return ((d & 0xFF00) >> 8);
}

unsigned char get_lower_bits(uint16_t d) {
    return (d  & 0xFF);
}

void send_data() {
    unsigned char low, high; // used if data exceeds 8 bit bus width

    // send acc data
    low = get_lower_bits(data.acc_data.x);
    high = get_upper_bits(data.acc_data.x);
    transmit_data(low);
    transmit_data(high);
    low = get_lower_bits(data.acc_data.y);
    high = get_upper_bits(data.acc_data.y);
    transmit_data(low);
    transmit_data(high);
    low = get_lower_bits(data.acc_data.z);
    high = get_upper_bits(data.acc_data.z);
    transmit_data(low);
    transmit_data(high);

    // send gps data
    low = get_lower_bits(data.gps_data.latitude);
    high = get_upper_bits(data.gps_data.latitude);
    transmit_data(low);
    transmit_data(high);
    low = get_lower_bits(data.gps_data.longitude);
    high = get_upper_bits(data.gps_data.longitude);
    transmit_data(low);
    transmit_data(high);
    transmit_data(data.gps_data.N);
    transmit_data(data.gps_data.E);

    // send temp data
    transmit_data(data.temp_data.temp);

    // send pulse data
    transmit_data(data.pulse_data.pulse);

    // send float data
    transmit_data(data.float_data.drowning);
}

void check_data() {
    if(data.acc_data.z >= 98) {
        set_buzzer();
        _delay_ms(10000);
        unset_buzzer();
    }

    struct gps_data gps_data = data.gps_data;

    if(gps_data.latitude < gps_data.latitude_min || gps_data.latitude > gps_data.latitude_max || gps_data.longitude < gps_data.longitude_min || gps_data.longitude > gps_data.longitude_max) {
        set_buzzer();
        set_led();
    }
    else {
        unset_buzzer();
        unset_led();
    }
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