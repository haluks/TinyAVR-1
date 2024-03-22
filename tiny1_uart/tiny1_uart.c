/*
 * tiny1_uart.c
 *
 * Created: 12.03.2024 11:10:58
 * Author : haluk
 */ 


#include "tiny1_uart.h"

#if (UART_Rx_Boyut>256)
volatile uint16_t rx_bas=0,rx_son=0;
#else
volatile uint8_t rx_bas=0,rx_son=0;
#endif
#if (UART_Tx_Boyut>256)
volatile uint16_t tx_bas=0,tx_son=0;
#else
volatile uint8_t tx_bas=0,tx_son=0;
#endif
volatile uint8_t rx_ring[UART_Rx_Boyut];
volatile uint8_t tx_ring[UART_Tx_Boyut];
volatile uint8_t uaflag=0;

ISR (USART0_RXC_vect){
	uint8_t gelen=USART0.RXDATAL;	
	rx_bas=(rx_bas+1) & UART_Rx_Mask;
	rx_ring[rx_bas]=gelen;	
	if (gelen=='\n'){
		uaflag++;
	}
}
ISR (USART0_DRE_vect){	//uart veri gönderim kesmesi
	tx_son=(tx_son+1)&UART_Tx_Mask;	
	if (tx_son==tx_bas)
	UART_Bos_Off;	
	USART0.TXDATAL=tx_ring[tx_son];	
}
ISR(USART0_TXC_vect){
	USART0.STATUS|=USART_TXCIF_bm;
}
void uart_basla(Bd_rate_t _baud){
	cli();	
	PORTB.DIRSET|=(1<<UART_TX_PIN);
	PORTB.OUTSET|=(1<<UART_TX_PIN);
	PORTB.DIRSET&=~(0<<UART_RX_PIN);
	uint16_t baudRate=0;
	baudRate=(float)(F_CPU*4/_baud);
	USART0.CTRLB|=USART_RXMODE_NORMAL_gc;
	if (_baud>=115200){//115200 ve üstünde U2X 1 yapýlýyor.
		baudRate=(float)(F_CPU*8/_baud);
		USART0.CTRLB|=USART_RXMODE_CLK2X_gc;
	}
	//USART0.BAUD=baudRate;
	USART0.BAUD=(baudRate>>8);
	USART0.BAUD=baudRate;
	USART0.CTRLA|=(1<<USART_RXCIE_bp)|(1<<USART_TXCIE_bp);
	USART0.CTRLB|=(1<<USART_RXEN_bp)|(1<<USART_TXEN_bp);
	USART0.CTRLC|=USART_CHSIZE_8BIT_gc;
	sei();
}
uint8_t uart_oku(){
	rx_son=(rx_son+1) & UART_Rx_Mask;
	return rx_ring[rx_son];
}
void uart_gonder(uint8_t uData){
	tx_bas=(tx_bas+1)&UART_Tx_Mask;
	while ((tx_son==tx_bas));
	tx_ring[tx_bas]=uData;
	UART_Bos_On;
}
void uart_dizi(const char *str){
	while(*str){
		tx_bas=(tx_bas+1)&UART_Tx_Mask;
		while ((tx_son==tx_bas));
		tx_ring[tx_bas]=*str++;
	}
	UART_Bos_On;
}
uint8_t uart_gelen(){
	if ((uaflag>0)&&!(rx_son==rx_bas))
	return 1;	
	return 0;
}
void uart_dizi_al(char *stri){
	uint8_t poz=0;
	do{		
		stri[poz++]=uart_oku();
	} while (!(rx_bas==rx_son));		
	stri[poz]='\0';
	uaflag=0;
}
