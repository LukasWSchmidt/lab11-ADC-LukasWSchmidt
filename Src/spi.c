/**
 * Name: Lukas Schmidt
 * Course: CPE2610
 * Section: 111
 * Assignment: Lab 10 - Led Matrix
 * File: spi.c
 * Dependencies: gpio, stdint, spi
 * Usage: Used for making SPI enabled for SPI2
 */
#include "../Inc/spi.h"
#include "../Inc/gpio.h"
#include <stdint.h>
#define PICO 15
#define SCK 13
#define CS 12
#define TXE 1
#define BSY 7

	static volatile GPIO_t *gpiob = (GPIO_t *)GPIOB_BASE;
  static 	volatile SPI_t *spi = (volatile SPI_t*)SPI2_BASE;
	
void SPIInit(void) {
	/*
  set: BR[2:0] in SPI_CR1 8MHz,
	16 bit Data frame size in SPI_CR1,
	and set MSB first,
	Mode: MODE 0, setting CPOL and CPHA
   */
   
	volatile uint32_t *spi_rcc = (volatile uint32_t *)RCC_APB1ENR;
	*spi_rcc|= RCC_SPI2_EN;

	uint32_t controls1 = spi->cr1;

	//using SPI2
	//SPeed of data sending
	controls1 |= (000 << BR);
	//16 bit data frame
	controls1 |= (1 << DFF);
	controls1 |= (0 << MSB);
	//MODE 0
	controls1 |= (0 << CPOL);
	controls1 |= (0 << CPHA);
  //Controller mode not peripheral SPI 	
	controls1 |= (1 << MSTR);
	//We're not using a pin for NSS, use SSM bit:
  //set the SSM bit and set high the SSI bit in the SPI_CR1 register (set lo for data transfer)
	controls1 |= (1 << SSM);
	controls1 |= (1 << SSI);

	spi->cr1 = controls1;

	//SSOE from his slides (I dont think it's needed cause were doing software Chip select)
	spi->cr2 |= (1 << SSOE);
  
	//Pin setups
	//Using :
	//(PB15) PICO - Alt Func - AF05
	//(PB13) SCK - Alt Func - AF05
	//(PB12) CS - output (drive with bsrr)

	//enable GPIOB
	volatile uint32_t *rcc = (volatile uint32_t *)RCC_AHB1_ENR;
	*rcc |= (1<<GPIOB_EN);

	// PICO(PB15) is Alt func
	uint32_t reg = gpiob->moder;
	gpiob->moder = ~(0b11 << (2 * PICO));
	gpiob->moder = (0b10 << (2 * PICO));
	//set Alt func (AF05)
	gpiob->afrh &= ~(0b1111<<(4 * (PICO - 8)));
	gpiob->afrh |= (0b0101<<(4 * (PICO - 8)));

	// SCK(PB13) is alt func
	reg = gpiob->moder;
	reg &= ~(0b11 << (2 * SCK));
	reg |= (0b10 << (2 * SCK));
	gpiob->moder = reg;
	//set Alt func (AF05)
	gpiob->afrh &= ~(0b1111<<(4 * (SCK - 8)));
	gpiob->afrh |= (0b0101<<(4 * (SCK - 8)));

	// CS(PB12) is output, but driven with func using bssr so no alt func
	reg = gpiob->moder;
	reg &= ~(0b11 << ( 2 * CS));
	reg |= (0b01 << ( 2 * CS));
	gpiob->moder = reg;
	}

void transmitData(uint16_t data) {
	//set CS to 0 from bsrr
 	gpiob->bsrr |= ( 1 << (CS + 16));

	//now enable SPE(enable serial peripheral)
	spi->cr1 |= (1 << SPE);

	//write data to Data reg
	spi->dr = data;

	uint8_t txe_flag = (spi->sr & (1 << TXE)) >> TXE;

	while(txe_flag == 0) {
		txe_flag = (spi->sr & (1 << TXE)) >> TXE;
	}
  
  //data has been sent
  //Wait for BSY flag
  uint8_t bsy_flag = (spi->sr & (1 << BSY)) >> BSY;
	while(bsy_flag == 1) {
  	bsy_flag = (spi->sr & (1 << BSY)) >> BSY;
	}

  //Turn off SPE
 	spi->cr1 &=  ~(1 << SPE);
 	
  //set CS to 1 from bsrr
 	gpiob->bsrr |= (1 << CS);
 	
 	//clear OVR
 	uint16_t temp = spi->dr;
 	temp = spi->dr;
}
 
