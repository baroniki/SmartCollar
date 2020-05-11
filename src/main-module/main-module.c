#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>

#include "../../include/sensor-data.h"

void loop (void);
void init_sensors(void);
void read_data(void);
void check_data(void);
void send_data(void);

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
}

void send_data() {

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