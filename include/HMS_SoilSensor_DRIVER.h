   /*
 ====================================================================================================
 * File:        HMS_SoilSensor_DRIVER.h
 * Author:      Hamas Saeed
 * Version:     Rev_1.0.0
 * Date:        Oct 1 2025
 * Brief:       This Package Provide Soil Sensor Driver Library for Cross Platform (STM/ESP/nRF)
 * 
 ====================================================================================================
 * License: 
 * MIT License
 * 
 * Copyright (c) 2025 Hamas Saeed
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 * 
 * For any inquiries, contact Hamas Saeed at hamasaeed@gmail.com
 *
 ====================================================================================================
 */

#ifndef HMS_SOIL_SENSOR_DRIVER_H
#define HMS_SOIL_SENSOR_DRIVER_H

#if defined(ARDUINO)                                                                                       // Platform detection
    #define HMS_SOIL_SENSOR_PLATFORM_ARDUINO
#elif defined(ESP_PLATFORM)
    #define HMS_SOIL_SENSOR_PLATFORM_ESP_IDF
#elif defined(__ZEPHYR__)
    #define HMS_SOIL_SENSOR_PLATFORM_ZEPHYR
#elif defined(__STM32__)
    #define HMS_SOIL_SENSOR_PLATFORM_STM32_HAL
#endif

#if defined(HMS_SOIL_SENSOR_PLATFORM_ARDUINO)
    #include <Arduino.h>
#elif defined(HMS_SOIL_SENSOR_PLATFORM_ESP_IDF)
    #include <stdio.h>
    #include <stdint.h>
#elif defined(HMS_SOIL_SENSOR_PLATFORM_ZEPHYR)
    #include <stdio.h>
    #include <stdint.h>
    #include <zephyr/device.h>
    #include <zephyr/drivers/i2c.h>
#elif defined(HMS_SOIL_SENSOR_PLATFORM_STM32_HAL)
    #include "main.h"
    #include <stdio.h>
    #include <stdint.h>
    #if defined(osCMSIS) || defined(FREERTOS)
        #define HMS_SOIL_SENSOR_STM32_FREERTOS
    #endif
#endif

#include "HMS_SoilSensor_Config.h"

#if defined(HMS_SOIL_SENSOR_DEBUG_ENABLED) && (HMS_SOIL_SENSOR_DEBUG_ENABLED == 1)
    #define HMS_SOIL_SENSOR_LOGGER_ENABLED
#endif

typedef enum {
    HMS_SOIL_SENSOR_OK       = 0x00,
    HMS_SOIL_SENSOR_ERROR    = 0x01,
} HMS_SOIL_SENSOR_Status;

typedef enum {
    HMS_SOIL_SENSOR_TYPE_YL69   = 0x00,   // Analog Output
} HMS_SOIL_SENSOR_Type;


class HMS_SoilSensor {
    public:
        #if defined(HMS_SOIL_SENSOR_PLATFORM_ARDUINO)
            HMS_SoilSensor(uint8_t pin, HMS_SOIL_SENSOR_Type sensorType = HMS_SOIL_SENSOR_TYPE_YL69  );
        #elif defined(HMS_SOIL_SENSOR_PLATFORM_ESP_IDF)
            HMS_SoilSensor(uint8_t pin, HMS_SOIL_SENSOR_Type sensorType = HMS_SOIL_SENSOR_TYPE_YL69  );
        #elif defined(HMS_SOIL_SENSOR_PLATFORM_ZEPHYR)
            HMS_SoilSensor(uint8_t pin, HMS_SOIL_SENSOR_Type sensorType = HMS_SOIL_SENSOR_TYPE_YL69  );
        #elif defined(HMS_SOIL_SENSOR_PLATFORM_STM32_HAL)
            HMS_SoilSensor(ADC_HandleTypeDef *hadc, HMS_SOIL_SENSOR_Type sensorType = HMS_SOIL_SENSOR_TYPE_YL69  );
        #endif

        ~HMS_SoilSensor();

    private:
        #if defined(HMS_SOIL_SENSOR_PLATFORM_ARDUINO)
        uint8_t         adcBitResolution    = 10;
        uint8_t         pin                 = A0;
        #elif defined(HMS_SOIL_SENSOR_PLATFORM_ZEPHYR)
        uint8_t         adcBitResolution    = 12;
        const struct device *adc_dev        = NULL;
        uint8_t         pin                 = 0;
        uint8_t         channel             = 0;
        int16_t         adc_raw             = 0;
        int             ret                 = 0;
        #elif defined(HMS_SOIL_SENSOR_PLATFORM_ESP_IDF)
        uint8_t         adcBitResolution    = 10;
        uint8_t         pin                 = 36;
        #elif defined(HMS_SOIL_SENSOR_PLATFORM_STM32_HAL)
        float             voltageResolution   = 3.3;
        uint8_t           adcBitResolution    = 12;
        ADC_HandleTypeDef *HMS_SOIL_SENSOR_hadc;
        #endif

        bool                    isInitialized;
};

#endif // HMS_SOIL_SENSOR_DRIVER_H