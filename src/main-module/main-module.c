#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>

#include "sensor-data.h"

void loop (void);
void read_data(void);
char check_data();
char send_data();

struct sensor_data data;

int main(void) {
    loop();
    return 0;
}

void loop() {
    while(1) {
        read_data();

        if(check_data())
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

char check_data() {
    return 1;
}

char send_data() {
    return 1;
}