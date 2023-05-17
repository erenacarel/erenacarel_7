/*
 * MAX31856.c
 *
 *  Created on: Oct 5, 2022
 *      Author: MertechArge015
 */

#include <MAX31856.h>
#include <stdbool.h>

extern SPI_HandleTypeDef hspi1;

uint8_t TxBuffer[2] = {0,0};
uint8_t RxBuffer[20];
uint8_t data;

#define cs_reset() HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);
#define cs_set() HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);
#define cs_strobe() cs_reset(); cs_set();
#define TIMEOUT 100

void MAX31856_Write(uint8_t addr, uint8_t dt)
{
	TxBuffer[0] = addr;
	TxBuffer[1] = dt;
	cs_strobe();
	HAL_SPI_Transmit(&hspi1, &TxBuffer[0], 1, TIMEOUT);//HAL_SPI_Transmit(&hspi1, &addr, sizeof(addr), TIMEOUT);
	while(HAL_SPI_GetState(&hspi1)!= HAL_SPI_STATE_READY);
	HAL_SPI_Transmit(&hspi1, &TxBuffer[1], 1, TIMEOUT);//HAL_SPI_Transmit(&hspi1, &dt, sizeof(dt), TIMEOUT);
	while(HAL_SPI_GetState(&hspi1)!= HAL_SPI_STATE_READY);
	cs_reset();

}


float MAX31856_readCJ()
{
	int32_t temp;
	uint8_t buf_read[3], buf_write[2] = {MAX31856_CJTH_REG, MAX31856_CJTL_REG};

	buf_read[0] = MAX31856_Read(buf_write[0], 1);
	buf_read[1] = MAX31856_Read(buf_write[1], 1);

	//Convert the registers contents into the correct value
	temp = ((int32_t)(buf_read[0] << 6));             // Shift the MSB into place
	temp |= ((int32_t)(buf_read[1] >> 2));            // Shift the LSB into place
	float val = ((float)(temp/64.0));                 // Divide the binary string by 2 to the 6th power

	return val;

}


int MAX31856_Read(uint8_t addr, uint8_t len)
{
	RxBuffer[0] = 0;
	uint8_t status1;
	cs_set();
	HAL_SPI_Transmit(&hspi1, &addr, len, TIMEOUT);
	while(HAL_SPI_GetState(&hspi1)!= HAL_SPI_STATE_READY);
	HAL_SPI_Receive(&hspi1, (uint8_t*)RxBuffer, len, TIMEOUT);
	while(HAL_SPI_GetState(&hspi1)!= HAL_SPI_STATE_READY);
	cs_reset();
	status1 = RxBuffer[0];
	return status1;

}


void MAX31856_Set_Thermocouple(thermocouple_t type)
{
	uint8_t status, read_addr, write_addr, clear_bits;
	read_addr = MAX31856_CR1_RREG;
	write_addr = MAX31856_CR1_WREG;
	status = MAX31856_Read(read_addr, 1);
	clear_bits = 0xF0;
	status = status & clear_bits;
	status = status | type;
	MAX31856_Write(write_addr, status);

}


float MAX31856_readTemp()
{
	uint8_t temp_reg[3], TxBuffer[2];
	int temp;
	float val;

	TxBuffer[0] = MAX31856_CR0_WREG;
	TxBuffer[1] = MAX31856_CR0_AUTOCONVERT | CR0_NOISE_FILTER_50HZ;
	MAX31856_Write(TxBuffer[0], TxBuffer[1]);

	temp_reg[0] = MAX31856_Read(MAX31856_LTCBH_REG, 1);
	temp_reg[1] = MAX31856_Read(MAX31856_LTCBM_REG, 1);
	temp_reg[2] = MAX31856_Read(MAX31856_LTCBL_REG, 1);

	// Temperature calculation
    //Convert the registers contents into the correct value
    temp =((temp_reg[0] & 0xFF) << 11);       //Shift Byte 2 into place
    temp|=((temp_reg[1] & 0xFF) << 3);        //Shift Byte 1 into place
    temp|=((temp_reg[2] & 0xFF) >> 5);        //Shift Byte 0 into place
    val=(temp/128.0f);                        //Divide the binary string by 2 to the 7th power
    return val;
}
