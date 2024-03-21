/*
 * tiny1_uart.c
 *
 * Created: 12.03.2024 11:10:58
 * Author : haluk
 */ 
#ifndef TINY1_UART_H
#define TINY1_UART_H


#define F_CPU 3333333 /* 20MHz / 6(default prescale) */
#include <avr/io.h>
#include <avr/interrupt.h>

/////////////////////////////////
#define UART_Rx_Boyut			128
#define UART_Tx_Boyut			128
#define UART_Rx_Mask			(UART_Rx_Boyut-1)
#define UART_Tx_Mask			(UART_Tx_Boyut-1)
#define UART_Bos_On				USART0.CTRLA|=(1<<USART_DREIE_bp)// uart UDR register boþ kesmesi
#define UART_Bos_Off			USART0.CTRLA&=~(1<<USART_DREIE_bp)// uart UDR register boþ kesmesi
#define PORTB2					PIN2_bp
#define PORTB3					PIN3_bp
#define UART_TX_PIN				PORTB2
#define UART_RX_PIN				PORTB3
typedef enum{
	Bd4800			=4800,
	Bd9600			=9600,
	Bd19200			=19200,
	Bd38400			=38400,
	Bd57600			=57600,
	Bd115200		=115200,
}Bd_rate_t;
////////////////////////////////////////////////////
void uart_basla(Bd_rate_t _baud);
uint8_t uart_oku();
void uart_gonder(uint8_t uData);
void uart_dizi(const char *str);
uint8_t uart_gelen();
void uart_dizi_al( char *stri);

#endif