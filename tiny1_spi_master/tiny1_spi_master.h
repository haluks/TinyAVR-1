/*
 * spi.h
 *
 * Created: 23.03.2024 12:02:07
 *  Author: haluk
 */ 

#ifndef TINY1_SPI_MASTER_H
#define TINY1_SPI_MASTER_H

#include <avr/io.h>
//#include <xc.h>

#define _PORTMUX		1

#if _PORTMUX
#define MOSI			PIN2_bp
#define MISO			PIN1_bp
#define SCK				PIN0_bp
#define SS				PIN3_bp
#define SPI_PORT		PORTC.OUT
#define SPI_DDR			PORTC.DIR
#else
#define MOSI			PIN1_bp
#define MISO			PIN2_bp
#define SCK				PIN3_bp
#define SS				PIN4_bp
#define SPI_PORT		PORTA.OUT
#define SPI_DDR			PORTA.DIR
#endif

#define SS_HIGH			SPI_PORT|=(1<<SS)
#define SS_LOW			SPI_PORT&=~(1<<SS)

void spi_basla();
uint8_t spi_data(uint8_t sData);
#endif
