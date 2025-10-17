#include "main.h"
#include "i2c.h"
#include "usart.h"
#include "gpio.h"
#include "ms5637.h"
#include "pressure_sensor.h"
#include "stdio.h"
#include "string.h"
#include "math.h"

static uint16_t coefficients[7]           = {0};
static char buffer[100]                   = {0};
static uint32_t D1                        = 0;
static uint32_t D2                        = 0;
static pressure_sensor_data_t sensor_data = {0};

int main(void)
{
    HAL_Init();
    SystemClock_Config();
    MX_GPIO_Init();
    MX_I2C1_Init();
    MX_USART1_UART_Init();
    ms5637_reset(&hi2c1);
    while (1) {
        ms5637_load_coefficients(&hi2c1);
        ms5637_read_coefficients(&hi2c1, coefficients);

        ms5637_start_pressure_conversion(&hi2c1, MS5637_OSR_256);
        HAL_Delay(1);
        ms5637_read_adc(&hi2c1, &D1);

        ms5637_start_temperature_conversion(&hi2c1, MS5637_OSR_256);
        HAL_Delay(1);
        ms5637_read_adc(&hi2c1, &D2);

        pressure_sensor_calculate(coefficients, D1, D2, &sensor_data);

        sprintf(buffer, "[T,P]:%f,%f\n", sensor_data.temperature,
                sensor_data.pressure);
        HAL_UART_Transmit(&huart1, (uint8_t *)buffer, strlen(buffer), 100);
        HAL_Delay(1000);
    }
}

void SystemClock_Config(void)
{
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
    RCC_OscInitStruct.OscillatorType     = RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState           = RCC_HSE_ON;
    RCC_OscInitStruct.HSEPredivValue     = RCC_HSE_PREDIV_DIV1;
    RCC_OscInitStruct.HSIState           = RCC_HSI_ON;
    RCC_OscInitStruct.PLL.PLLState       = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource      = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLMUL         = RCC_PLL_MUL9;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) { Error_Handler(); }
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK |
                                  RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource   = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider  = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK) {
        Error_Handler();
    }
    HAL_RCC_EnableCSS();
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == TIM1) { HAL_IncTick(); }
}

void Error_Handler(void)
{
    __disable_irq();
    while (1) {}
}

#ifdef USE_FULL_ASSERT
/**
 * @brief  Reports the name of the source file and the source line number
 *         where the assert_param error has occurred.
 * @param  file: pointer to the source file name
 * @param  line: assert_param error line source number
 * @retval None
 */
void assert_failed(uint8_t *file, uint32_t line)
{
    /* USER CODE BEGIN 6 */
    /* User can add his own implementation to report the file name and line
       number, ex: printf("Wrong parameters value: file %s on line %d\r\n",
       file, line) */
    /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */