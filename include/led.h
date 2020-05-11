#ifndef LED_H
#define LED_H

void init_led() {
    DDRD |= (1 << PD6);
}

void set_led() {
    PORTD |= (1 << PD6);
}

void unset_led() {
    PORTD &= ~(1 << PD6);
}

#endif