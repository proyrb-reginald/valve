#ifndef __MS5637_H
#define __MS5637_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f1xx_hal.h"

#define MS5637_ADDR                0b11101100

#define MS5637_CMD_RESET           0x1E
#define MS5637_CMD_CONVERT_D1_256  0x40
#define MS5637_CMD_CONVERT_D1_512  0x42
#define MS5637_CMD_CONVERT_D1_1024 0x44
#define MS5637_CMD_CONVERT_D1_2048 0x46
#define MS5637_CMD_CONVERT_D1_4096 0x48
#define MS5637_CMD_CONVERT_D1_8192 0x4A
#define MS5637_CMD_CONVERT_D2_256  0x50
#define MS5637_CMD_CONVERT_D2_512  0x52
#define MS5637_CMD_CONVERT_D2_1024 0x54
#define MS5637_CMD_CONVERT_D2_2048 0x56
#define MS5637_CMD_CONVERT_D2_4096 0x58
#define MS5637_CMD_CONVERT_D2_8192 0x5A
#define MS5637_CMD_ADC_READ        0x00
#define MS5637_CMD_PROM_READ       0xA0

#define MS5637_CMD_C0              0xA0
#define MS5637_CMD_C1              0xA2
#define MS5637_CMD_C2              0xA4
#define MS5637_CMD_C3              0xA6
#define MS5637_CMD_C4              0xA8
#define MS5637_CMD_C5              0xAA
#define MS5637_CMD_C6              0xAC

// Oversampling ratio definitions
typedef enum {
    MS5637_OSR_256  = 0,
    MS5637_OSR_512  = 1,
    MS5637_OSR_1024 = 2,
    MS5637_OSR_2048 = 3,
    MS5637_OSR_4096 = 4,
    MS5637_OSR_8192 = 5,
} ms5637_osr_t;

// Function prototypes
HAL_StatusTypeDef ms5637_reset(I2C_HandleTypeDef *hi2c);
HAL_StatusTypeDef ms5637_read_coefficients(I2C_HandleTypeDef *hi2c,
                                           uint16_t coefficients[7]);
HAL_StatusTypeDef ms5637_load_coefficients(I2C_HandleTypeDef *hi2c);
HAL_StatusTypeDef ms5637_get_coefficient(uint8_t index, uint16_t *coeff_value);
HAL_StatusTypeDef ms5637_start_pressure_conversion(I2C_HandleTypeDef *hi2c,
                                                   ms5637_osr_t osr);
HAL_StatusTypeDef ms5637_start_temperature_conversion(I2C_HandleTypeDef *hi2c,
                                                      ms5637_osr_t osr);
HAL_StatusTypeDef ms5637_read_adc(I2C_HandleTypeDef *hi2c, uint32_t *adc_value);

#ifdef __cplusplus
}
#endif

#endif /* __MS5637_H */