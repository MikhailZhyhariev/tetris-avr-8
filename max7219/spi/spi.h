#ifndef SPI_H
#define SPI_H

#define SPI_PORTX   PORTB
#define SPI_DDRX    DDRB

#define SPI_MISO   3
#define SPI_MOSI   2
#define SPI_SCK    1
#define SPI_SS     0

void SPI_MasterInit(void);

void SPI_SlaveInit(void);

void SPI_MasterTransmit(char cData, unsigned char end);

char SPI_SlaveReceive(void);


#endif
