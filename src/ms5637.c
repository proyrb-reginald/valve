#include "ms5637.h"

// Static variables to store coefficients (only accessible within this file)
static uint16_t ms5637_coefficients[7]    = {0};
static uint8_t ms5637_coefficients_loaded = 0;

/**
 * @brief Reset the MS5637 sensor
 * @param hi2c: I2C handle
 * @return HAL status
 */
HAL_StatusTypeDef ms5637_reset(I2C_HandleTypeDef *hi2c)
{
    uint8_t cmd = MS5637_CMD_RESET;
    return HAL_I2C_Master_Transmit(hi2c, MS5637_ADDR, &cmd, 1, 100);
}

/**
 * @brief Read a specific PROM coefficient from MS5637
 * @param hi2c: I2C handle
 * @param coeff_addr: Coefficient address (0-6)
 * @param coeff_value: Pointer to store the coefficient value (16-bit)
 * @return HAL status
 */
static HAL_StatusTypeDef ms5637_read_prom_coefficient(I2C_HandleTypeDef *hi2c,
                                                      uint8_t coeff_addr,
                                                      uint16_t *coeff_value)
{
    uint8_t cmd = MS5637_CMD_PROM_READ + (coeff_addr << 1);
    uint8_t coeff_data[2];
    HAL_StatusTypeDef status;

    // Send PROM read command for specific coefficient
    status = HAL_I2C_Master_Transmit(hi2c, MS5637_ADDR, &cmd, 1, 100);
    if (status != HAL_OK) { return status; }

    // Read 2 bytes of coefficient data
    status = HAL_I2C_Master_Receive(hi2c, MS5637_ADDR, coeff_data, 2, 100);
    if (status != HAL_OK) { return status; }

    // Combine 2 bytes into 16-bit value
    *coeff_value = (coeff_data[0] << 8) | coeff_data[1];

    return HAL_OK;
}

/**
 * @brief Read all PROM coefficients (C0-C6) from MS5637
 * @param hi2c: I2C handle
 * @param coefficients: Array to store the 7 coefficient values
 * @return HAL status
 */
HAL_StatusTypeDef ms5637_read_coefficients(I2C_HandleTypeDef *hi2c,
                                           uint16_t coefficients[7])
{
    HAL_StatusTypeDef status;

    // Read all 7 coefficients (C0-C6)
    for (uint8_t i = 0; i < 7; i++) {
        status = ms5637_read_prom_coefficient(hi2c, i, &coefficients[i]);
        if (status != HAL_OK) { return status; }
    }

    return HAL_OK;
}

/**
 * @brief Load PROM coefficients from MS5637 to internal storage
 * @param hi2c: I2C handle
 * @return HAL status
 */
HAL_StatusTypeDef ms5637_load_coefficients(I2C_HandleTypeDef *hi2c)
{
    HAL_StatusTypeDef status =
        ms5637_read_coefficients(hi2c, ms5637_coefficients);
    if (status == HAL_OK) { ms5637_coefficients_loaded = 1; }
    return status;
}

/**
 * @brief Get a specific coefficient from internal storage
 * @param index: Coefficient index (0-6)
 * @param coeff_value: Pointer to store the coefficient value
 * @return HAL status
 */
HAL_StatusTypeDef ms5637_get_coefficient(uint8_t index, uint16_t *coeff_value)
{
    if (index > 6) { return HAL_ERROR; }

    if (!ms5637_coefficients_loaded) { return HAL_ERROR; }

    *coeff_value = ms5637_coefficients[index];
    return HAL_OK;
}

/**
 * @brief Start pressure conversion with selected oversampling ratio
 * @param hi2c: I2C handle
 * @param osr: Oversampling ratio
 * @return HAL status
 */
HAL_StatusTypeDef ms5637_start_pressure_conversion(I2C_HandleTypeDef *hi2c,
                                                   ms5637_osr_t osr)
{
    uint8_t cmd;

    switch (osr) {
        case MS5637_OSR_256:
            cmd = MS5637_CMD_CONVERT_D1_256;
            break;
        case MS5637_OSR_512:
            cmd = MS5637_CMD_CONVERT_D1_512;
            break;
        case MS5637_OSR_1024:
            cmd = MS5637_CMD_CONVERT_D1_1024;
            break;
        case MS5637_OSR_2048:
            cmd = MS5637_CMD_CONVERT_D1_2048;
            break;
        case MS5637_OSR_4096:
            cmd = MS5637_CMD_CONVERT_D1_4096;
            break;
        case MS5637_OSR_8192:
            cmd = MS5637_CMD_CONVERT_D1_8192;
            break;
        default:
            return HAL_ERROR;
    }

    return HAL_I2C_Master_Transmit(hi2c, MS5637_ADDR, &cmd, 1, 100);
}

/**
 * @brief Start temperature conversion with selected oversampling ratio
 * @param hi2c: I2C handle
 * @param osr: Oversampling ratio
 * @return HAL status
 */
HAL_StatusTypeDef ms5637_start_temperature_conversion(I2C_HandleTypeDef *hi2c,
                                                      ms5637_osr_t osr)
{
    uint8_t cmd;

    switch (osr) {
        case MS5637_OSR_256:
            cmd = MS5637_CMD_CONVERT_D2_256;
            break;
        case MS5637_OSR_512:
            cmd = MS5637_CMD_CONVERT_D2_512;
            break;
        case MS5637_OSR_1024:
            cmd = MS5637_CMD_CONVERT_D2_1024;
            break;
        case MS5637_OSR_2048:
            cmd = MS5637_CMD_CONVERT_D2_2048;
            break;
        case MS5637_OSR_4096:
            cmd = MS5637_CMD_CONVERT_D2_4096;
            break;
        case MS5637_OSR_8192:
            cmd = MS5637_CMD_CONVERT_D2_8192;
            break;
        default:
            return HAL_ERROR;
    }

    return HAL_I2C_Master_Transmit(hi2c, MS5637_ADDR, &cmd, 1, 100);
}

/**
 * @brief Read ADC value from MS5637
 * @param hi2c: I2C handle
 * @param adc_value: Pointer to store the ADC value (24-bit)
 * @return HAL status
 */
HAL_StatusTypeDef ms5637_read_adc(I2C_HandleTypeDef *hi2c, uint32_t *adc_value)
{
    uint8_t cmd = MS5637_CMD_ADC_READ;
    uint8_t adc_data[3];
    HAL_StatusTypeDef status;

    // Send ADC read command
    status = HAL_I2C_Master_Transmit(hi2c, MS5637_ADDR, &cmd, 1, 100);
    if (status != HAL_OK) { return status; }

    // Read 3 bytes of ADC data
    status = HAL_I2C_Master_Receive(hi2c, MS5637_ADDR, adc_data, 3, 100);
    if (status != HAL_OK) { return status; }

    // Combine 3 bytes into 24-bit value
    *adc_value = (adc_data[0] << 16) | (adc_data[1] << 8) | adc_data[2];

    return HAL_OK;
}