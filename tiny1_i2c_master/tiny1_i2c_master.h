


/*
 * tiny1_i2c_master.h
 *
 * Created: 12.03.2024 11:05:21
 *  Author: haluk
 */ 

#ifndef TINY1_I2C_MASTER_H
#define TINY1_I2C_MASTER_H

#define F_CPU 3333333
#define F_SCL 100000
#define TWI_MBAUD(F_SCL,Trise) ((F_CPU/F_SCL)/2)-(5+(((F_CPU/1000000)*Trise)/2000))
#include <avr/io.h>
#include <avr/interrupt.h>

#define I2C_Rx_Boyut					16
#define I2C_Tx_Boyut					16
#define I2C_Rx_Mask						(I2C_Rx_Boyut-1)
#define I2C_Tx_Mask						(I2C_Tx_Boyut-1)
#define I2C_TIMEOUT						1000
//I2C status
#define I2C_STATUS						TWI0.MSTATUS
//I2C master status
#define I2C_MTR_ADR_ACK					0x40// adres yazýldý ack geldi
#define I2C_MTR_ADR_NACK				0x50// adres yazýldý nack geldi
#define I2C_MTR_DATA_ACK				0x42// veri gitti ack geldi
#define I2C_MTR_DATA_NACK				0x52// veri gitti nack geldi
#define I2C_MRD_ADR_ACK					0x81//adres yazýldý ack geldi
#define I2C_MRD_ADR_NACK				0x91// adres yazýldý nack geldi
#define I2C_MRD_DATA_ACK				0x83// veri alýndý ack gönderildi
#define I2C_MRD_DATA_NACK				0x93//veri alýndý nac gönderildi

//I2C error status
#define I2C_ARB_LOST					TWI_ARBLOST_bm//hat yönetim kaybý
#define I2C_UNKNOWN						TWI_BUSSTATE_UNKNOWN_gc//belirsiz durum
#define I2C_BUS_ERR						TWI_BUSERR_bm//hat hatasý

//////////////////////////////////////////////////////////////////////////

typedef enum {
	I2C_WRITE		=0,
	I2C_READ		=1,
}i2c_cmd_t;
typedef  enum {
	_REPEAT			=1,
	N_REPEAT		=0,
}i2c_rep_t;

#define	STAT_WRT_ADDR	0
#define	STAT_RD_ADDR	1
#define	STAT_WRT_DATA	2
#define	STAT_RD_DATA	3

void i2c_init();
void i2c_disable();
void i2c_reset();
void i2c_start();
void i2c_stop();
void i2c_adr(uint8_t adr,i2c_cmd_t cmd);
void i2c_data(uint8_t data);
void i2c_end( i2c_rep_t repst);
void i2c_send_data(uint8_t adr, uint8_t data,  i2c_rep_t repst);
void i2c_send(uint8_t adr, uint8_t* str,uint8_t len,  i2c_rep_t repst);
void i2c_send_str(uint8_t adr, const char* str,  i2c_rep_t repst);
void i2c_read(uint8_t adr, uint8_t len);
uint8_t i2c_gelen();
uint8_t i2c_oku();
uint8_t i2c_adres_scan();

#endif