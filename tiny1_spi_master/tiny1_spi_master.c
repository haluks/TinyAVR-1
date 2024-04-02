/*
 * spi.c
 *
 * Created: 23.03.2024 12:02:07
 *  Author: haluk
 */ 
#include "tiny1_spi_master.h"
void spi_basla(){
	#if _PORTMUX
	PORTMUX.CTRLB|=PORTMUX_SPI0_ALTERNATE_gc;
	#else
	PORTMUX.CTRLB|=PORTMUX_SPI0_DEFAULT_gc;
	#endif
	SPI_DDR|=(1<<MOSI)|(1<<SCK)|(1<<SS);//master
	SPI_DDR&=~(1<<MISO);//master
	SS_HIGH;// master ss high
	SPI0.CTRLB|=SPI_MODE_0_gc;//spi mode 0
	SPI0.CTRLA|=SPI_MASTER_bm|SPI_ENABLE_bm|SPI_PRESC_DIV4_gc|(0<<SPI_DORD_bp);//master,spi enable,presclar 4,MSb first
}
uint8_t spi_data(uint8_t sData){
	SPI0.DATA=sData;
	while(!(SPI0.INTFLAGS&(1<<SPI_IF_bp)));
	return SPI0.DATA;
}