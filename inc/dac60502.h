#ifndef __DAC60502_H
#define __DAC60502_H

#include "stm32f1xx_hal.h"
#include <stdint.h>

#define DAC60502_SPI_CS_PORT GPIOB
#define DAC60502_SPI_CS_PIN  GPIO_PIN_12

// DAC60502寄存器地址
#define DAC60502_CMD_NOOP    0x0
#define DAC60502_CMD_DEVID   0x1
#define DAC60502_CMD_SYNC    0x2
#define DAC60502_CMD_CONFIG  0x3
#define DAC60502_CMD_GAIN    0x4
#define DAC60502_CMD_TRIGGER 0x5
#define DAC60502_CMD_BRDCAST 0x6
#define DAC60502_CMD_STATUS  0x7
#define DAC60502_CMD_DAC_A   0x8
#define DAC60502_CMD_DAC_B   0x9

// 函数声明
HAL_StatusTypeDef dac60502_read_register(SPI_HandleTypeDef *hspi,
                                         uint8_t reg_addr, uint16_t *data);
HAL_StatusTypeDef dac60502_write_register(SPI_HandleTypeDef *hspi,
                                          uint8_t reg_addr, uint16_t data);
HAL_StatusTypeDef dac60502_reset(SPI_HandleTypeDef *hspi);
HAL_StatusTypeDef dac60502_read_id(SPI_HandleTypeDef *hspi, uint16_t *id);
uint8_t dac60502_write_dac_a(SPI_HandleTypeDef *hspi, float rate);
uint8_t dac60502_write_dac_b(SPI_HandleTypeDef *hspi, float rate);

#endif /* __DAC60502_H */