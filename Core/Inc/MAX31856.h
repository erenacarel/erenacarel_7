/*
 * MAX31865.h
 *
 *  Created on: Oct 5, 2022
 *      Author: MertechArge015
 */

#ifndef INC_MAX31856_H_
#define INC_MAX31856_H_

#include "stm32g0xx_hal.h"
#include <stdlib.h>
#include <stdbool.h>
#include "main.h"

// CR0 Registerleri
#define MAX31856_CR0_REG                   0x00         ///< Config 0 register
#define MAX31856_CR0_WREG                  0x80
#define MAX31856_CR0_AUTOCONVERT           0x80        ///< Config 0 Auto convert flag
#define CR0_ONE_SHOT                       0x40
#define CR0_OPEN_CIRCUIT_FAULT_TYPE_J      0x20         // type j'de bu, type K 0x10
#define CR0_COLD_JUNCTION_DISABLED         0x08
#define CR0_FAULT_INTERRUPT_MODE           0x04
#define CR0_FAULT_CLEAR                    0x02
#define CR0_NOISE_FILTER_50HZ              0x01
#define CR0_REJ_REG                        0x00

// CR1 Registerleri
#define MAX31856_CR1_RREG                   0x01   ///< Config 1 register
#define MAX31856_CR1_WREG                   0x81    ///< Config 1 write register

typedef enum
{
	TC_TYPE_B = 0x00,    // 0 ve 3. bitler
	TC_TYPE_E = 0x01,
	TC_TYPE_J = 0x02,
	TC_TYPE_K = 0x03,
	TC_TYPE_N = 0x04,
	TC_TYPE_R = 0x05,
	TC_TYPE_S = 0x06,
	TC_TYPE_T = 0x07,
	V_MODE_G8 = 0x08,
	V_MODE_G16 = 0x0C,

}thermocouple_t;

typedef enum
{
	AVG_1_SAMPLE = 0x00,     // 4 ve 6. bitler
	AVG_2_SAMPLES = 0x10,
	AVG_4_SAMPLES = 0x20,
	AVG_8_SAMPLES = 0x30,
	AVG_16_SAMPLES = 0x40

}averageSample_t;

// MASK Registerleri
#define MASK_COLD_JUNCTION_HIGH_FAULT      		0x20
#define MASK_COLD_JUNCTION_LOW_FAULT            0x10
#define MASK_THERMOCOUPLE_HIGH_FAULT            0x08
#define MASK_THERMOCOUPLE_LOW_FAULT             0x04
#define MASK_VOLTAGE_UNDER_OVER_FAULT           0x02
#define MASK_THERMOCOUPLE_OPEN_FAULT            0x01

#define MAX31856_MASK_REG                       0x02 // fault mask register

// SR Registerleri Fault Status Register sayfa 26
#define SR_FAULT_COLD_JUNCTION_OUT_OF_RANGE     0x80
#define SR_FAULT_THERMOCOUPLE_OUT_OF_RANGE      0x40
#define SR_FAULT_COLD_JUNCTION_HIGH             0x20
#define SR_FAULT_COLD_JUNCTION_LOW              0x10
#define SR_FAULT_THERMOCOUPLE_HIGH              0x08
#define SR_FAULT_THERMOCOUPLE_LOW               0x04
#define SR_FAULT_UNDER_OVER_VOLTAGE             0x02
#define SR_FAULT_OPEN                           0x01

// Open Circuit Detection Registeleri Sayfa 14
#define MAX31856_CR0_OCFAULT_TYPE_J_00          0x00        ///< Config 0 open circuit fault 1 flag  10
#define MAX31856_CR0_OCFAULT_TYPE_J_01          0x10        ///< Config 0 open circuit fault 0 flag  01
#define MAX31856_CR0_OCFAULT_TYPE_J_10          0x20
#define MAX31856_CR0_OCFAULT_TYPE_J_11          0x30

// Sayfa 22 ve 25 arasındaki registerler
#define MAX31856_CJHF_REG          0x03    ///< Cold junction High temp fault register
#define MAX31856_CJLF_REG          0x04    ///< Cold junction Low temp fault register
#define MAX31856_LTHFTH_REG        0x05    ///< Linearized Temperature High Fault Threshold Register, MSB
#define MAX31856_LTHFTL_REG        0x06    ///< Linearized Temperature High Fault Threshold Register, LSB
#define MAX31856_LTLFTH_REG        0x07    ///< Linearized Temperature Low Fault Threshold Register, MSB
#define MAX31856_LTLFTL_REG        0x08    ///< Linearized Temperature Low Fault Threshold Register, LSB
#define MAX31856_CJTO_REG          0x09    ///< Cold-Junction Temperature Offset Register
#define MAX31856_CJTH_REG          0x0A    ///< Cold-Junction Temperature Register, MSB
#define MAX31856_CJTL_REG          0x0B    ///< Cold-Junction Temperature Register, LSB
#define MAX31856_LTCBH_REG         0x0C    ///< Linearized TC Temperature, Byte 2
#define MAX31856_LTCBM_REG         0x0D    ///< Linearized TC Temperature, Byte 1
#define MAX31856_LTCBL_REG         0x0E    ///< Linearized TC Temperature, Byte 0
#define MAX31856_SR_REG            0x0F    ///< Fault Status Register

#define ON 1
#define OFF 0

/** Temperature conversion mode */
typedef enum
{
    CONV_ONESHOT,
    CONV_ONESHOT_NOWAIT,
    CONV_CONTINUOUS
}conversion_mode_t;

typedef enum
{
	MAX31856_NOISE_FILTER_50HZ,
	MAX31856_NOISE_FILTER_60HZ
}max31856_noise_filter_t;

void MAX31856_Write(uint8_t addr, uint8_t dt);
int MAX31856_Read(uint8_t addr, uint8_t len);
float MAX31856_readTemp();
float MAX31856_readCJ();
void MAX31856_Set_Thermocouple(uint8_t type);


#endif /* INC_MAX31856_H_ */
