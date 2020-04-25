#ifndef GPS_H
#define GPS_H

#include <string.h>

#define BAUDRATE 9600   
#define NMEA_LENGTH 100


struct gps_data {
    double latitude;
    double longitude;
};

struct gps_data read_gps(void);
struct gps_data read_line(uint8_t *);
uint8_t uart_recieve (void);

uint8_t line[NMEA_LENGTH] = "";
uint8_t line_index = 0;

void init_gps() {
    UBRR0H = (uint8_t) (BAUDRATE >> 8);
	UBRR0L = (uint8_t) BAUDRATE;
    UCSR0B |= (1 << RXEN0) | (1 << TXEN0);
}

struct gps_data read_gps() {
    uint8_t c = uart_recieve();

    while(c != '$' || line[0] == 0) {
        line[line_index++] = c;
    }

    struct gps_data gps_data = read_line(line);

    line_index = 0;

    return gps_data;
}

struct gps_data read_line(uint8_t * line) {
    uint8_t num_commas = 0;

    const char s[2] = ",";
    const char *str;

    struct gps_data gps_data;
    gps_data.latitude = 0;
    gps_data.longitude = 0;

    while(1) {
        str = strtok(line, s);

        if(num_commas == 2) {
            gps_data.latitude = atof(str);
        }
        else if(num_commas == 4) {
            gps_data.longitude = atof(str);
            break;
        }

        num_commas++;
    }

    return gps_data;
}

uint8_t uart_recieve (void) {
    while(!(UCSR0A & (1 << RXC0)));

    return UDR0;
}

#endif