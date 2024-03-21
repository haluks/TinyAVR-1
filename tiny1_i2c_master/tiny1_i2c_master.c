


/*
 * tiny1_i2c_master.h
 *
 * Created: 12.03.2024 11:05:21
 *  Author: haluk
 */ 


#include "tiny1_i2c_master.h"
#include <util/delay.h>
static volatile uint8_t i2c_rx_bas=0,i2c_rx_son=0,i2c_tx_bas=0,i2c_tx_son=0,i2c_rx_len=0;
static volatile uint8_t i2c_rx_ring[I2C_Rx_Boyut];
static volatile uint8_t i2c_tx_ring[I2C_Tx_Boyut];
static volatile uint8_t Sladr_RW=0, i2c_rep=0;
static volatile uint8_t i2c_state=0;
volatile uint32_t timeout=0;
volatile uint8_t adres_nack=0;
void i2c_init(){	
	cli();
	TWI0.MBAUD=TWI_MBAUD(F_SCL,200);	
	TWI0.MCTRLA|=TWI_ENABLE_bm/*|(1<<TWI_SMEN_bp)|(TWI_TIMEOUT_200US_gc)|(1<<TWI_QCEN_bp)*/|TWI_WIEN_bm|TWI_RIEN_bm;
	I2C_STATUS|= TWI_RIF_bm | TWI_WIF_bm|TWI_BUSSTATE_IDLE_gc;
	sei();
}
void i2c_disable(){
	TWI0.MCTRLB=TWI_FLUSH_bm;
	TWI0.CTRLA=0x00;	
}
void i2c_reset(){
	i2c_disable();
	i2c_init();	
}
void i2c_start( ){	
	TWI0.MADDR=Sladr_RW;	
}
void i2c_adr(uint8_t adr,i2c_cmd_t cmd){
		while(!(I2C_STATUS & TWI_BUSSTATE_IDLE_gc)){
			timeout++;
			if (timeout>I2C_TIMEOUT){
				timeout=0;
				i2c_reset();
				return;
			}
		}
		i2c_tx_bas=0;
		Sladr_RW=((adr<<1)|cmd);
		i2c_state=cmd;	
}
void i2c_data(uint8_t data){
	i2c_tx_ring[i2c_tx_bas++]=data;
}
void i2c_end( i2c_rep_t repst){
	i2c_rep=repst;
	i2c_tx_son=0;	
	i2c_start();
}
void i2c_stop(){
	TWI0.MCTRLB|=TWI_MCMD_STOP_gc;
	
	while(!(I2C_STATUS & TWI_BUSSTATE_IDLE_gc)){
	timeout++;
	if (timeout>I2C_TIMEOUT){
		timeout=0;
		i2c_reset();
		return;
		}
	}	
	TWI0.MCTRLB=0x00;
}
void i2c_send_data(uint8_t adr, uint8_t data,  i2c_rep_t repst){	
	
	i2c_adr(adr, I2C_WRITE);
	i2c_data(data);
	i2c_end(repst);
}
void i2c_send(uint8_t adr, uint8_t* str, uint8_t len,  i2c_rep_t repst){
	
	i2c_adr(adr, I2C_WRITE);
	i2c_tx_bas=0;
	for (uint8_t i=0;i<len;i++){
		i2c_data( str[i]);
	}
	i2c_end(repst);
}
void i2c_send_str(uint8_t adr, const char* str,  i2c_rep_t repst){
	i2c_adr(adr, I2C_WRITE);
	i2c_tx_bas=0;
	while (*str){
		i2c_data(*str++);		
	}	
	i2c_end(repst);	
}
void i2c_read(uint8_t adr, uint8_t len){	
	i2c_rx_bas=0;
	i2c_rx_son=0;
	i2c_rx_len=len;
	i2c_adr(adr, I2C_READ);	
	i2c_start();
}
uint8_t i2c_gelen(){	

	if (i2c_rx_son==i2c_rx_bas)	return 0;
	return 1;	
}
uint8_t i2c_oku(){		
	return i2c_rx_ring[i2c_rx_son++];
}
uint8_t i2c_adres_scan(){
	for (uint8_t i=0;i<128;i++){
		adres_nack=1;
		i2c_send_data(i,0x00,N_REPEAT);
		_delay_ms(10);
		if (adres_nack)
		return i;
	}
	return 0;
}
ISR (TWI0_TWIM_vect){
	i2c_state|=(I2C_STATUS&0xD0);
	switch (i2c_state){
		case I2C_MTR_ADR_ACK://adres yazýldý ack geldi
			i2c_state= STAT_WRT_DATA;
		case I2C_MTR_DATA_ACK://data yazýldý ack geldi
			if (i2c_tx_son!=i2c_tx_bas){
				TWI0.MDATA=i2c_tx_ring[i2c_tx_son++];
			}
			else if (i2c_rep==1){
				i2c_rep=0;
				TWI0.MCTRLB|=TWI_MCMD_REPSTART_gc;
			}
			else{
				i2c_stop();
			}		
		break;
		case I2C_MTR_ADR_NACK://adres yazýldý nack geldi
		adres_nack=0;
		i2c_stop();
		break;
		case I2C_MTR_DATA_NACK:	//data yazýldý nack geldi
		i2c_stop();
		break;	
		case I2C_MRD_DATA_ACK://okuma data yazýldý ack geldi	
					
		case I2C_MRD_ADR_ACK://okuma adres yazýldý ack geldi
		i2c_state=STAT_RD_DATA;	
		
		i2c_rx_ring[i2c_rx_bas++]=TWI0.MDATA;
			if (i2c_rx_bas!=i2c_rx_len){	
				TWI0.MCTRLB |= TWI_ACKACT_ACK_gc|TWI_MCMD_RECVTRANS_gc;
				//TWI0.MCTRLB |= TWI_MCMD_RECVTRANS_gc;
				}else{
				i2c_state=I2C_MRD_DATA_NACK;						
				TWI0.MCTRLB |= TWI_ACKACT_NACK_gc;
				
				i2c_stop();
			}
		break;
		case I2C_MRD_ADR_NACK://okuma adres yazýldý ack geldi
			i2c_stop();
		break;	
		case I2C_MRD_DATA_NACK:		
			i2c_stop();
		break;
		default:
			i2c_stop();
		break;
	}
	
}