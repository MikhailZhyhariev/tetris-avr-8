#ifndef SPI_H
#define SPI_H

#define SPI_PORTX   PORTB
#define SPI_DDRX    DDRB

#define SPI_MISO   4
#define SPI_MOSI   3
#define SPI_SCK    5
#define SPI_SS     2

void SPI_MasterInit(void);

void SPI_SlaveInit(void);

void SPI_MasterTransmit(char cData, unsigned char end);

char SPI_SlaveReceive(void);


#endif
