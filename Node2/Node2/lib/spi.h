/*
 * spi.h
 *
 * Created: 11.10.2017 13:50:45
 *  Author: eddas
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>

#ifndef SPI_H_
#define SPI_H_

void spi_masterInit(void);
uint8_t spi_masterTransmit(char data);

#endif /* SPI_H_ */