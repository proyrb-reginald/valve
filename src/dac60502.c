#include "dac60502.h"

#define DAC60502_DAC_MAX 4096

/**
 * @brief 写入寄存器
 * @param hspi: SPI句柄
 * @param reg_addr: 寄存器地址
 * @param data: 要写入的数据
 * @return 0: 成功, 1: 失败
 */
HAL_StatusTypeDef dac60502_read_register(SPI_HandleTypeDef *hspi,
                                         uint8_t reg_addr, uint16_t *data)
{
    uint8_t spi_tx_data[3]   = {0};
    uint8_t spi_rx_data[3]   = {0};
    HAL_StatusTypeDef status = HAL_OK;

    // 构造24位命令字
    spi_tx_data[0] = 0x80 | (reg_addr & 0x7F);

    HAL_GPIO_WritePin(DAC60502_SPI_CS_PORT, DAC60502_SPI_CS_PIN,
                      GPIO_PIN_RESET);
    status = HAL_SPI_TransmitReceive(hspi, spi_tx_data, spi_rx_data, 3, 10);
    HAL_GPIO_WritePin(DAC60502_SPI_CS_PORT, DAC60502_SPI_CS_PIN, GPIO_PIN_SET);
    *data = ((uint16_t)spi_rx_data[1] << 8) | spi_rx_data[2];
    return status;
}

/**
 * @brief 写入寄存器
 * @param hspi: SPI句柄
 * @param reg_addr: 寄存器地址
 * @param data: 要写入的数据
 * @return 0: 成功, 1: 失败
 */
HAL_StatusTypeDef dac60502_write_register(SPI_HandleTypeDef *hspi,
                                          uint8_t reg_addr, uint16_t data)
{
    uint8_t spi_data[3]      = {0};
    HAL_StatusTypeDef status = HAL_OK;

    // 构造24位命令字
    spi_data[0] = 0x00 | (reg_addr & 0x7F);
    spi_data[1] = (data >> 8) & 0xFF;
    spi_data[2] = data & 0xFF;

    HAL_GPIO_WritePin(DAC60502_SPI_CS_PORT, DAC60502_SPI_CS_PIN,
                      GPIO_PIN_RESET);
    status = HAL_SPI_Transmit(hspi, spi_data, 3, HAL_MAX_DELAY);
    HAL_GPIO_WritePin(DAC60502_SPI_CS_PORT, DAC60502_SPI_CS_PIN, GPIO_PIN_SET);
    return status;
}

/**
 * @brief DAC60502初始化
 * @param hspi: SPI句柄
 * @return 0: 成功, 1: 失败
 */
HAL_StatusTypeDef dac60502_reset(SPI_HandleTypeDef *hspi)
{
    return dac60502_write_register(hspi, DAC60502_CMD_TRIGGER, 0x0A);
}

HAL_StatusTypeDef dac60502_read_id(SPI_HandleTypeDef *hspi, uint16_t *id)
{
    //
}

/**
 * @brief 写入DAC_A通道数据
 * @param hspi: SPI句柄
 * @param data: 要写入的数据 (左12位)
 * @return 0: 成功, 1: 失败
 */
#include "usart.h"
#include "stdio.h"
#include "string.h"
uint8_t dac60502_write_dac_a(SPI_HandleTypeDef *hspi, float rate)
{
    rate = rate * DAC60502_DAC_MAX;
    if (rate >= DAC60502_DAC_MAX) {
        rate = DAC60502_DAC_MAX - 1;
    } else if (rate < 0) {
        rate = 0;
    }
    uint16_t data   = rate;
    data            = data << 4;
    char buffer[20] = {0};
    sprintf(buffer, "[DAC_A]:%u\n", data);
    HAL_UART_Transmit(&huart1, (uint8_t *)buffer, strlen(buffer), 10);
    return dac60502_write_register(hspi, DAC60502_CMD_DAC_A, data);
}

/**
 * @brief 写入DAC_B通道数据
 * @param hspi: SPI句柄
 * @param data: 要写入的数据 (左12位)
 * @return 0: 成功, 1: 失败
 */
uint8_t dac60502_write_dac_b(SPI_HandleTypeDef *hspi, float rate)
{
    uint16_t data = (uint16_t)(rate * DAC60502_DAC_MAX);
    if (data >= DAC60502_DAC_MAX) {
        data = DAC60502_DAC_MAX - 1;
    } else if (data < 0) {
        data = 0;
    }
    data = data << 4;
    return dac60502_write_register(hspi, DAC60502_CMD_DAC_B, data << 4);
}