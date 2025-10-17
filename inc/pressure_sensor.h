#ifndef __PRESSURE_SENSOR_H
#define __PRESSURE_SENSOR_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f1xx_hal.h"
#include "ms5637.h"

typedef struct {
    float temperature; // Temperature in centidegrees Celsius (°C * 100)
    float pressure;    // Pressure in mbar * 100
} pressure_sensor_data_t;

/**
 * @brief 计算温度和压力值
 * @param coefficients: 传感器校准系数数组
 * @param D1: 数字压力值
 * @param D2: 数字温度值
 * @param data: 指向存储计算结果的结构体指针
 */
void pressure_sensor_calculate(uint16_t coefficients[7], uint32_t D1,
                               uint32_t D2, pressure_sensor_data_t *data);

#ifdef __cplusplus
}
#endif

#endif /* __PRESSURE_SENSOR_H */