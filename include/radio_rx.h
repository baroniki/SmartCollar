#ifndef RADIO_RX_H
#define RADIO_RX_H

void init_radio_rx() {
    DDRB = (1<<6);     //MISO as OUTPUT
    SPCR = (1<<SPE);   //Enable SPI
}

unsigned char receive_data(void) {
    // Load data into the buffer
    SPDR = data;
 
    //Wait until transmission complete
    while(!(SPSR & (1<<SPIF) ));

    return SPDR;
}

#endif