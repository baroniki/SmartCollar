#ifndef BUZZER_H
#define BUZZER_H

void init_buzzer() {
    DDRD |= (1 << PD7);
}

void set_buzzer() {
    PORTD |= (1 << PD7);
}

void unset_buzzer() {
    PORTD &= ~(1 << PD7);
}


#endif