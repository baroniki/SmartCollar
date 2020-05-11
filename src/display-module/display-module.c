#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>

#include "../../include/sensor-data.h"

void init_modules(void);
void loop(void);

int main(void) {
    init_modules();

    loop();
}

loop() {
    while(1) {
        struct sensor_data data = receive_data();

        check_data(data);

        display_data(data);
    }
}

void init_modules() {
    init_radio();
    init_bluetooth();
    init_display();
}