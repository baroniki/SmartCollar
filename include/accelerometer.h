#ifndef ACCELEROMETER_H
#define ACCELEROMETER_H

#include <string.h>

void i2c_init(uint8_t);
uint8_t i2c_io(uint8_t, uint8_t *, uint16_t,
               uint8_t *, uint16_t, uint8_t *, uint16_t);

void sci_init(uint8_t);
void sci_out(char);
void sci_outs(char *);

uint8_t rdprom(uint8_t *, uint16_t, uint16_t);
uint8_t wrprom(uint8_t *, uint16_t, uint16_t);

#define FOSC 9830400
#define BAUD 9600
#define MYUBRR FOSC/16/BAUD-1
#define BDIV (FOSC / 100000 - 16) / 2 + 1

#define LIS3DH_DEFAULT_ADDRESS 0x18     // accelerometer address
#define LIS3DH_REG_OUT_X_L 0x28         // data address

struct acc_data {
    uint16_t x;
    uint16_t y;
    uint16_t z;
};

void init_acc() {
    i2c_init(BDIV);

    sci_init(MYUBRR);
}

struct acc_data read_acc() {
    struct acc_data acc_data;
    acc_data.x = 0;
    acc_data.y = 0;
    acc_data.z = 0;

    uint8_t status;
    uint8_t buffer[6];
    status = rdprom(buffer, 6, LIS3DH_REG_OUT_X_L);

    if(status) {
        uint16_t x, y, z;
        x = buffer[0];
        x |= ((uint16_t)buffer[1]) << 8;
        y = buffer[2];
        y |= ((uint16_t)buffer[3]) << 8;
        z = buffer[4];
        z |= ((uint16_t)buffer[5]) << 8;

        acc_data.x = x;
        acc_data.y = y;
        acc_data.z = z;
    }

    return acc_data;
}

/*
  wrprom - write bytes to the EEPROM over the I2C bus
*/
uint8_t wrprom(uint8_t *p, uint16_t n, uint16_t a)
{
    uint16_t maxw;              // Maximum bytes to write in the page
    uint8_t status;
    uint8_t adata[2];           // Array to hold the address

    while (n > 0) {
        adata[0] = a >> 8;      // Put EEPROM address in adata buffer,
        adata[1] = a & 0xff;    // MSB first, LSB second
        // We can write up to the next 64 byte boundary,
        // but no more than is left to write
        maxw = 64 - (a % 64);   // Max for this page
        if (n < maxw)
            maxw = n;           // Number left to write in page
        status = i2c_io(LIS3DH_DEFAULT_ADDRESS, adata, 2, p, maxw, NULL, 0);
        if (status != 0)
            return(status);
        _delay_ms(5);           // Wait 5ms for EEPROM to write
        p += maxw;              // Increment array address
        a += maxw;              // Increment address
        n -= maxw;              // Decrement byte count
    }
    return(0);
}


/*
  rdprom - read bytes from the EEPROM over the I2C bus
*/
uint8_t rdprom(uint8_t *p, uint16_t n, uint16_t a)
{
    uint8_t status;
    uint8_t adata[2];           // Array to hold the address

    adata[0] = a >> 8;          // Put EEPROM address in adata buffer,
    adata[1] = a & 0xff;        // MSB first, LSB second

    status = i2c_io(LIS3DH_DEFAULT_ADDRESS, adata, 2, NULL, 0, p, n);
    return(status);
}


uint8_t i2c_io(uint8_t device_addr, uint8_t *ap, uint16_t an, 
               uint8_t *wp, uint16_t wn, uint8_t *rp, uint16_t rn)
{
    uint8_t status, send_stop, wrote, start_stat;

    status = 0;
    wrote = 0;
    send_stop = 0;

    if (an > 0 || wn > 0) {
        wrote = 1;
        send_stop = 1;

        TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTA);  // Send start condition
        while (!(TWCR & (1 << TWINT)));     // Wait for TWINT to be set
        status = TWSR & 0xf8;
        if (status != 0x08)                 // Check that START was sent OK
            return(status);

        TWDR = device_addr & 0xfe;          // Load device address and R/W = 0;
        TWCR = (1 << TWINT) | (1 << TWEN);  // Start transmission
        while (!(TWCR & (1 << TWINT)));     // Wait for TWINT to be set
        status = TWSR & 0xf8;
        if (status != 0x18) {               // Check that SLA+W was sent OK
            if (status == 0x20)             // Check for NAK
                goto nakstop;               // Send STOP condition
            return(status);                 // Otherwise just return the status
        }

        // Write "an" data bytes to the slave device
        while (an-- > 0) {
            TWDR = *ap++;                   // Put next data byte in TWDR
            TWCR = (1 << TWINT) | (1 << TWEN); // Start transmission
            while (!(TWCR & (1 << TWINT))); // Wait for TWINT to be set
            status = TWSR & 0xf8;
            if (status != 0x28) {           // Check that data was sent OK
                if (status == 0x30)         // Check for NAK
                    goto nakstop;           // Send STOP condition
                return(status);             // Otherwise just return the status
            }
        }

        // Write "wn" data bytes to the slave device
        while (wn-- > 0) {
            TWDR = *wp++;                   // Put next data byte in TWDR
            TWCR = (1 << TWINT) | (1 << TWEN); // Start transmission
            while (!(TWCR & (1 << TWINT))); // Wait for TWINT to be set
            status = TWSR & 0xf8;
            if (status != 0x28) {           // Check that data was sent OK
                if (status == 0x30)         // Check for NAK
                    goto nakstop;           // Send STOP condition
                return(status);             // Otherwise just return the status
            }
        }

        status = 0;                         // Set status value to successful
    }

    if (rn > 0) {
        send_stop = 1;

        // Set the status value to check for depending on whether this is a
        // START or repeated START
        start_stat = (wrote) ? 0x10 : 0x08;

        // Put TWI into Master Receive mode by sending a START, which could
        // be a repeated START condition if we just finished writing.
        TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTA);
                                            // Send start (or repeated start) condition
        while (!(TWCR & (1 << TWINT)));     // Wait for TWINT to be set
        status = TWSR & 0xf8;
        if (status != start_stat)           // Check that START or repeated START sent OK
            return(status);

        TWDR = device_addr  | 0x01;         // Load device address and R/W = 1;
        TWCR = (1 << TWINT) | (1 << TWEN);  // Send address+r
        while (!(TWCR & (1 << TWINT)));     // Wait for TWINT to be set
        status = TWSR & 0xf8;
        if (status != 0x40) {               // Check that SLA+R was sent OK
            if (status == 0x48)             // Check for NAK
                goto nakstop;
            return(status);
        }

        // Read all but the last of n bytes from the slave device in this loop
        rn--;
        while (rn-- > 0) {
            TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWEA); // Read byte and send ACK
            while (!(TWCR & (1 << TWINT))); // Wait for TWINT to be set
            status = TWSR & 0xf8;
            if (status != 0x50)             // Check that data received OK
                return(status);
            *rp++ = TWDR;                   // Read the data
        }

        // Read the last byte
        TWCR = (1 << TWINT) | (1 << TWEN);  // Read last byte with NOT ACK sent
        while (!(TWCR & (1 << TWINT)));     // Wait for TWINT to be set
        status = TWSR & 0xf8;
        if (status != 0x58)                 // Check that data received OK
            return(status);
        *rp++ = TWDR;                       // Read the data

        status = 0;                         // Set status value to successful
    }
    
nakstop:                                    // Come here to send STOP after a NAK
    if (send_stop)
        TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);  // Send STOP condition

    return(status);
}


/*
  i2c_init - Initialize the I2C port
*/
void i2c_init(uint8_t bdiv)
{
    TWSR = 0;                           // Set prescalar for 1
    TWBR = bdiv;                        // Set bit rate register
}

/* ----------------------------------------------------------------------- */

/*
  sci_init - Initialize the SCI port
*/
void sci_init(uint8_t ubrr) {
    UBRR0 = ubrr;            // Set baud rate
    UCSR0B |= (1 << TXEN0);  // Turn on transmitter
    UCSR0C = (3 << UCSZ00);  // Set for asynchronous operation, no parity, 
                             // one stop bit, 8 data bits
}

/*
  sci_out - Output a byte to SCI port
*/
void sci_out(char ch)
{
    while ((UCSR0A & (1<<UDRE0)) == 0);
    UDR0 = ch;
}

/*
  sci_outs - Print the contents of the character string "s" out the SCI
  port. The string must be terminated by a zero byte.
*/
void sci_outs(char *s)
{
    char ch;

    while ((ch = *s++) != (char) '\0')
        sci_out(ch);
}

#endif