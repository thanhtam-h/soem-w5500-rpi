/* Raspberry pi GPIO and SPI control
 * For SOEM W5500 driver
 * By Ho Tam - thanhtam.h[at]gmail.com
 * R1.0 2017-10-09
 */

// Access from ARM Running Linux

#ifndef _RPI_PERIPH_H
#define _RPI_PERIPH_H

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stdint.h>

#ifndef sbi
#define sbi(sf, bit) sf |= (1<<bit)
#define cbi(sf, bit) sf &= ~(1<<bit)
#define bit_is_set(sf, bit)		(sf &(1<<bit))?1:0
#define bit_is_clear(sf, bit)	(sf &(1<<bit))?0:1
#define mask_is_set(sf, mask)	(sf & mask)?1:0
#define mask_is_clear(sf, mask)	(sf & mask)?0:1
#endif

#define MEM_PAGE_SIZE 	4096
#define MEM_BLOCK_SIZE 	4096

extern int  rpi_mem_fd;
extern volatile unsigned int *rpi_gpio_mem_base;
extern volatile unsigned int *rpi_spi_mem_base;


#ifndef _RPI23_
#define RPI_PERIPH_BASE        0x20000000		//RPI0 and 1
#else
#define RPI_PERIPH_BASE        0x3F000000		//RPI2 and 3
#endif


#define RPI_GPIO_BASE          (RPI_PERIPH_BASE + 0x200000)	/*GPIO start offset*/
#define RPI_SPI_BASE           (RPI_PERIPH_BASE + 0x204000) /*SPI start offset*/

#define RPI_GPIO_GPFSEL(n)			*(rpi_gpio_mem_base+n)
#define RPI_GPIO_GPSET0				*(rpi_gpio_mem_base+7)
#define RPI_GPIO_GPSET1				*(rpi_gpio_mem_base+8)

#define RPI_GPIO_GPCLR0				*(rpi_gpio_mem_base+10)
#define RPI_GPIO_GPCLR1				*(rpi_gpio_mem_base+11)

#define RPI_GPIO_DIR_IN				0x0
#define RPI_GPIO_DIR_OUT			0x1

//alternative functions
#define RPI_GPIO_ALT_0				0x4
#define RPI_GPIO_ALT_1				0x5
#define RPI_GPIO_ALT_2				0x6
#define RPI_GPIO_ALT_3				0x7
#define RPI_GPIO_ALT_4				0x3
#define RPI_GPIO_ALT_5				0x2


//GPIO PIN define
// Pin on RPI				pin on BCM chip
//		RPI_P1_Pin01			3v3
//		RPI_P1_Pin02			5v
#define RPI_P1_Pin03			0
//		RPI_P1_Pin0				5v
#define RPI_P1_Pin05			1
//		RPI_P1_Pin06			GND
#define RPI_P1_Pin07			4
#define RPI_P1_Pin08			14
//		RPI_P1_Pin09			GND
#define RPI_P1_Pin10			15
#define RPI_P1_Pin11			17
#define RPI_P1_Pin12			18
#define RPI_P1_Pin13			21
//		RPI_P1_Pin14			GND
#define RPI_P1_Pin15			22
#define RPI_P1_Pin16			23
//		RPI_P1_Pin17			GND
#define RPI_P1_Pin18			24
#define RPI_P1_Pin19			10
#//     RPI_P1_Pin20			GND
#define RPI_P1_Pin21			9
#define RPI_P1_Pin22			25
#define RPI_P1_Pin23			11
#define RPI_P1_Pin24			8
//	 	RPI_P1_Pin25			GND
#define RPI_P1_Pin26			7

//More GPIO on new
#define RPI_P1_Pin29			5
#define RPI_P1_Pin31			6
#define RPI_P1_Pin32			12
#define RPI_P1_Pin33			13
#define RPI_P1_Pin35			19
#define RPI_P1_Pin36			16
#define RPI_P1_Pin37			26
#define RPI_P1_Pin38			20
#define RPI_P1_Pin40			21

//SPI pins

/* BCM2835 datasheet 10.5, from page 152*/
#define RPI_SPI_CTRL     	*(rpi_spi_mem_base)
#define RPI_SPI_FIFO     	*(rpi_spi_mem_base + 1)
#define RPI_SPI_CLK 		*(rpi_spi_mem_base + 2)

#define RPI_GPIO_ALT_SPI			RPI_GPIO_ALT_0

//Control and Status register
/*
 * 18 - TXD
 * TXD TX FIFO can accept Data
 * 0 = TX FIFO is full and so cannot accept more data.
 * 1 = TX FIFO has space for at least 1 byte
 */
#define RPI_SPI_CS_TXD	        0x00040000
#define rpi_spi_txFifo_accept_data()	(RPI_SPI_CTRL&RPI_SPI_CS_TXD)
/*
 * 17 - RXD
 * RXD RX FIFO contains Data
 * 0 = RX FIFO is empty.
 * 1 =  RX FIFO contains at least 1 byte.
 */
#define RPI_SPI_CS_RXD	        0x00020000
#define rpi_spi_rxFifo_has_data()		(RPI_SPI_CTRL&BCM2835_SPI0_CS_RXD)

/*
 * 16- DONE:
 * Done transfer Done
	0 = Transfer is in progress (or not active TA = 0).
	1 = Transfer is complete. Cleared by writing
	more data to the TX FIFO or setting TA to 0
 */
#define RPI_SPI_CS_DONE		0x00010000
#define rpi_spi_transfer_completed()	(RPI_SPI_CTRL&RPI_SPI_CS_DONE)

/* 7-TA
 * Transfer Active
	0 = Transfer not active./CS lines are all high
	(assuming CSPOL = 0). RXR and DONE are 0.
	Writes to SPIFIFO write data into bits -0 of
	SPICS allowing DMA data blocks to set mode
	before sending data.
	1 = Transfer active. /CS lines are set according
	to CS bits and CSPOL. Writes to SPIFIFO write
	data to TX FIFO.TA is cleared by a
	dma_frame_end pulse from the DMA controller
 */
#define RPI_SPI_CS_TA		0x00000080
#define rpi_spi_start_transfer()	(RPI_SPI_CTRL|=RPI_SPI_CS_CLEAR_FIFO|RPI_SPI_CS_TA)
#define rpi_spi_stop()				(RPI_SPI_CTRL &=~RPI_SPI_CS_TA)

/*
 * CLEAR FIFO Clear
	00 = No action.
	x1 = Clear TX FIFO. One shot operation.
	1x = Clear RX FIFO. One shot operation.
	If CLEAR and TA are both set in the same
	operation, the FIFOs are cleared before the new
	frame is started. Read back as 0
 */
#define RPI_SPI_CS_CLEAR_FIFO	0x00000030

#define RPI_SPI_CS_CPOL			0x00000008
#define RPI_SPI_CS_CPHA			0x00000004
#define rpi_spi_set_mode0()		(RPI_SPI_CTRL &=~(RPI_SPI_CS_CLEAR_FIFO|RPI_SPI_CS_TA))
#define rpi_spi_set_mode3()		(RPI_SPI_CTRL |=(RPI_SPI_CS_CLEAR_FIFO|RPI_SPI_CS_TA))


/* Register masks for SPI0_CS */
#define BCM2835_SPI0_CS_LEN_LONG             0x02000000 /*!< Enable Long data word in Lossi mode if DMA_LEN is set */
#define BCM2835_SPI0_CS_DMA_LEN              0x01000000 /*!< Enable DMA mode in Lossi mode */
#define BCM2835_SPI0_CS_CSPOL2               0x00800000 /*!< Chip Select 2 Polarity */
#define BCM2835_SPI0_CS_CSPOL1               0x00400000 /*!< Chip Select 1 Polarity */
#define BCM2835_SPI0_CS_CSPOL0               0x00200000 /*!< Chip Select 0 Polarity */
#define BCM2835_SPI0_CS_RXF                  0x00100000 /*!< RXF - RX FIFO Full */
#define BCM2835_SPI0_CS_RXR                  0x00080000 /*!< RXR RX FIFO needs Reading (full) */
#define BCM2835_SPI0_CS_TXD                  0x00040000 /*!< TXD TX FIFO can accept Data */
#define BCM2835_SPI0_CS_RXD                  0x00020000 /*!< RXD RX FIFO contains Data */
#define BCM2835_SPI0_CS_DONE                 0x00010000 /*!< Done transfer Done */
#define BCM2835_SPI0_CS_TE_EN                0x00008000 /*!< Unused */
#define BCM2835_SPI0_CS_LMONO                0x00004000 /*!< Unused */
#define BCM2835_SPI0_CS_LEN                  0x00002000 /*!< LEN LoSSI enable */
#define BCM2835_SPI0_CS_REN                  0x00001000 /*!< REN Read Enable */
#define BCM2835_SPI0_CS_ADCS                 0x00000800 /*!< ADCS Automatically Deassert Chip Select */
#define BCM2835_SPI0_CS_INTR                 0x00000400 /*!< INTR Interrupt on RXR */
#define BCM2835_SPI0_CS_INTD                 0x00000200 /*!< INTD Interrupt on Done */
#define BCM2835_SPI0_CS_DMAEN                0x00000100 /*!< DMAEN DMA Enable */
#define BCM2835_SPI0_CS_TA                   0x00000080 /*!< Transfer Active */
#define BCM2835_SPI0_CS_CSPOL                0x00000040 /*!< Chip Select Polarity */
#define BCM2835_SPI0_CS_CLEAR                0x00000030 /*!< Clear FIFO Clear RX and TX */
#define BCM2835_SPI0_CS_CLEAR_RX             0x00000020 /*!< Clear FIFO Clear RX  */
#define BCM2835_SPI0_CS_CLEAR_TX             0x00000010 /*!< Clear FIFO Clear TX  */
#define BCM2835_SPI0_CS_CPOL                 0x00000008 /*!< Clock Polarity */
#define BCM2835_SPI0_CS_CPHA                 0x00000004 /*!< Clock Phase */
#define BCM2835_SPI0_CS_CS                   0x00000003 /*!< Chip Select */


#define RPI_GPIO_MOSI_Pin			RPI_P1_Pin19
#define RPI_GPIO_MISO_Pin			RPI_P1_Pin21
#define RPI_GPIO_SCLK_Pin			RPI_P1_Pin23
#define RPI_GPIO_CE0_Pin			RPI_P1_Pin24
#define RPI_GPIO_CE1_Pin			RPI_P1_Pin26





int rpi_periph_init(void); //memory map

void rpi_set_pin_dir(uint32_t pin, uint32_t dir);
void rpi_set_pin_alt(uint32_t pin, uint32_t alt);
void rpi_set_pin(uint32_t pin);
void rpi_clr_pin(uint32_t pin);

#define RPI_SET_PIN(pin)	RPI_GPIO_GPSET0 =(1<<pin)
#define RPI_CLR_PIN(pin)	RPI_GPIO_GPCLR0 =(1<<pin)

void rpi_spi_config(uint32_t CLK_Div);
void rpi_spi_send_buf(uint8_t *tx_buf, int len);

void rpi_spi_send_recv_buf(uint8_t *tx_buf, uint8_t *rx_buf, int len);
void rpi_spi_recv_buf(uint8_t *rx_buf, int len);

uint8_t rpi_spi_send_recv_byte(uint8_t val);

#endif //_RPI_PERIPH_H
