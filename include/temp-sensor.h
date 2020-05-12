#ifndef TEMP_SENSOR_H
#define TEMP_SENSOR_H

struct temp_data {
    unsigned char temp; // temp in farenheight
};

struct temp_data read_temp_sensor() {
    struct temp_data temp_data;
    temp_data.temp = 0;

    ADCSRA = ADCSRA | (1 << ADSC);

    while(ADCSRA & (1 << ADSC));

    temp_data.temp = (ADCH * 9 / 5) + 32;

    return temp_data;
}

void init_temp_sensor() {
    PORTC &= ~(1 << PC1);
    ADCSRA = 0b10000111;
}

#endif