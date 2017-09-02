#include <avr/io.h>
#include "spi.h"

// Функция инициализации МК Ведущим
void SPI_MasterInit(void)
{
    SPI_DDRX = (1 << SPI_MOSI)|(1 << SPI_SCK)|(1 << SPI_SS);//настраиваем на выход
    SPCR = (0 << SPIE)|(1 << SPE)|(0 << DORD)|(1 << MSTR)|(0 << CPOL)|(0 << CPHA)|(0 << SPR1)|(1 << SPR0); //вкл SPI, ведущий, частота fck/16
}

// Функция инициализации МК Ведомым
void SPI_SlaveInit(void)
{
    // настраиваем на выход MISO, остальные на вход
    SPI_DDRX = (1 << SPI_MISO)|(0 << SPI_MOSI)|(0 << SPI_SCK)|(0 << SPI_SS);
    SPCR = (1 << SPIE)|(1 << SPE)|(0 << DORD)|(0 << MSTR)|(0 << CPOL)|(0 << CPHA)|(0 << SPR1)|(1 << SPR0); // включаем SPI
}

char SPI_SlaveReceive(void)
{
    /* Wait for reception complete */
    while(!(SPSR & (1 << SPIF)));
    /* Return data register */
    return SPDR;
}

void SPI_MasterTransmit(char cData, unsigned char end) {

    SPI_PORTX &= ~(1 << SPI_SS);

    // Start transmition
    SPDR = cData;

    // Wait for transmission complete
    while(!(SPSR & (1 << SPIF)));

    // End transmit if variable end is true
    if (end == 1) SPI_PORTX |= 1 << SPI_SS;
}
