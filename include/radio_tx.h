#ifndef RADIO_TX_H
#define RADIO_TX_H

void init_radio_tx() {
    DDRB = (1<<6);     //MISO as OUTPUT
    SPCR = (1<<SPE);   //Enable SPI
}

void transmit_data(unsigned char data) {
    // Load data into the buffer
    SPDR = data;
 
    //Wait until transmission complete
    while(!(SPSR & (1<<SPIF) ));
}

#endif