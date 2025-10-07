   /*
 ====================================================================================================
 * File:        HMS_SoilSensor_Config.h
 * Author:      Hamas Saeed
 * Version:     Rev_1.0.0
 * Date:        Sep 23 2025
 * Brief:       This Package Provide Soil Sensor Driver Configuration.
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

#ifndef HMS_SOIL_SENSOR_CONFIG_H
#define HMS_SOIL_SENSOR_CONFIG_H

/*
  ┌─────────────────────────────────────────────────────────────────────┐
  │ Note:     Enable only if ChronoLog is included                      │
  │ Requires: ChronoLog library → https://github.com/Hamas888/ChronoLog │
  └─────────────────────────────────────────────────────────────────────┘
*/
#define HMS_SOIL_SENSOR_DEBUG_ENABLED           0                            // Enable debug messages (1=enabled, 0=disabled)
  
// ADC values measured for dry and wet soil
#define HMS_SOIL_SENSOR_MOISTURE_MAX_VALUE      4095
#define HMS_SOIL_SENSOR_MOISTURE_DRY_VALUE      4000                         // ADC value measured for dry soil
#define HMS_SOIL_SENSOR_MOISTURE_WET_VALUE      1800                          // ADC value measured for wet soil
// Moisture Thresholds ADC
#define HMS_SOIL_SENSOR_THRESHOLD_DRY_PERCENT   30.0f                         // Threshold for dry soil in percent
#define HMS_SOIL_SENSOR_THRESHOLD_WET_PERCENT   60.0f                         // Threshold for wet soil in percent
// Volatge threashold
#define HMS_SOIL_SENSOR_THRESHOLD_DRY_VOLTAGE   3.30f                          // Threshold voltage
#define HMS_SOIL_SENSOR_THRESHOLD_WET_VOLTAGE   1.0f                          // Threshold voltage
// Define ADC range limits for readability
#define HMS_SOIL_ADC_VERY_WET_MAX               1000.0f
#define HMS_SOIL_ADC_WET_MAX                    2000.0f
#define HMS_SOIL_ADC_MEDIUM_MAX                 3000.0f
#define HMS_SOIL_ADC_DRY_MAX                    4095.0f

//ratio in clean air
#define HMS_SOIL_SENSOR_CLEAN_AIR_RATIO         1.0f                          // Ratio of clean air
#define HMS_SOIL_SENSOR_PERCENTAGE              100.0f

#endif // HMS_NEO6M_CONFIG_H