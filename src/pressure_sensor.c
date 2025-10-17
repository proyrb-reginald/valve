#include "pressure_sensor.h"
#include "math.h"

void pressure_sensor_calculate(uint16_t coefficients[7], uint32_t D1,
                               uint32_t D2, pressure_sensor_data_t *data)
{
    int32_t dT;
    float TEMP;
    float T2;
    float OFF;
    float OFF2;
    float SENS;
    float SENS2;
    float P;

    // 计算实际温度和压力值
    dT   = D2 - coefficients[5] * 256;
    TEMP = 2000 + ((float)dT * coefficients[6]) / 8388608.0f;
    OFF  = (float)coefficients[2] * 131072.0f + coefficients[4] * dT / 64.0f;
    SENS = (float)coefficients[1] * 65536.0f + coefficients[3] * dT / 128.0f;

    // Second order temperature compensation
    if (TEMP > 2000.0f) {
        T2    = 5.0f * (dT * dT) / 274877906944.0f;
        OFF2  = 0.0f;
        SENS2 = 0.0f;
    } else {
        // For lower temperatures, additional compensation is needed
        T2    = 3.0f * (dT * dT) / 8589934592.0f;
        OFF2  = 3.8125f * (TEMP - 2000.0f) * (TEMP - 2000.0f);
        SENS2 = 1.8125f * (TEMP - 2000.0f) * (TEMP - 2000.0f);

        // Further compensation for very low temperatures
        if (TEMP < -1500.0f) {
            OFF2  = OFF2 + 17.0f * (TEMP + 1500.0f) * (TEMP + 1500.0f);
            SENS2 = SENS2 + 9.0f * (TEMP + 1500.0f) * (TEMP + 1500.0f);
        }
    }

    OFF  = OFF - OFF2;
    SENS = SENS - SENS2;
    TEMP = TEMP - T2;
    P    = ((float)D1 * SENS / 2097152.0f - OFF) / 32768.0f;

    // 将结果存储在输出结构体中
    data->temperature = TEMP; // 温度以0.01°C为单位
    data->pressure    = P;    // 压力以0.01mbar为单位
}