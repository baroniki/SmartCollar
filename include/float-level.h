#ifndef FLOAT_LEVEL_H
#define FLOAT_LEVEL_H

struct float_data {
    char drowning;
};

struct float_data read_float_level() {
    struct float_data float_data;
    float_data.drowning = 0;

    if(PIND & (1 << PD5))
        float_data.drowning = 1;

    return float_data;
}

void init_float_level() {
    DDRD &= ~(1 << PD5);
    PORTD |= (1 << PD5);
}

#endif