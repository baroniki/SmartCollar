#ifndef PULSE_SENSOR_H
#define PULSE_SENSOR_H

struct pulse_data {
    unsigned char pulse;
};

struct pulse_data read_pulse_sensor() {
    struct pulse_data pulse_data;
    pulse_data.pulse = 0;

    ADCSRA = ADCSRA | (1 << ADSC);

    while(ADCSRA & (1 << ADSC));

    pulse_data.pulse = ADCH;

    return pulse_data;
}

void init_pulse_sensor() {
    PORTC &= ~(1 << PC2);
    ADCSRA = 0b10000111;
}

#endif