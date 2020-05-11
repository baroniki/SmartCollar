#ifndef GPS_H
#define GPS_H

#include <string.h>

#define BAUDRATE 9600   
#define NMEA_LENGTH 100


struct gps_data {
    double latitude;
    double longitude;
    double latitude_min, latitude_max, longitude_min, longitude_max;
    char N;
    char E;
};

void init_gps(void);
struct gps_data read_gps(void);
struct gps_data read_line(char *);
uint8_t uart_recieve (void);

char line[NMEA_LENGTH];
uint8_t line_index = 0;


struct gps_data read_gps() {
    uint8_t c;

    while((c = uart_recieve()) != '$' || line_index == 0)
        line[line_index++] = c;

    struct gps_data gps_data = read_line(line);

    line_index = 0;

    return gps_data;
}

struct gps_data read_line(char * line) {
    uint8_t num_commas = 0;

    const char s[2] = ",";
    const char n[2] = "N";
    const char e[2] = "E";
    char *str;

    struct gps_data gps_data;
    gps_data.latitude = 0;
    gps_data.longitude = 0;
    gps_data.N = 0;
    gps_data.E = 0;

    while(num_commas < 6) {
        str = strtok(line, s);

        switch(num_commas) {
            case 2:
                //gps_data.latitude = atof(str);
                break;
            case 3:
                gps_data.N = strcmp(str, n) == 0 ? 1 : 0;
                break;
            case 4:
                //gps_data.longitude = atof(str);
                break;
            case 5:
                gps_data.E = strcmp(str, e) == 0 ? 1 : 0;
                break;
            default:
                break;
        }

        num_commas++;
    }

    return gps_data;
}

void init_gps() {
    UBRR0H = (uint8_t) (BAUDRATE >> 8);
	UBRR0L = (uint8_t) BAUDRATE;
    UCSR0B |= (1 << RXEN0) | (1 << TXEN0); 
}

uint8_t uart_recieve (void) {
    while(!(UCSR0A & (1 << RXC0)));

    return UDR0;
}

#endif