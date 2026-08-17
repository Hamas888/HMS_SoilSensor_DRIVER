#include "HMS_SoilSensor_DRIVER.h"

#if defined(HMS_SOIL_SENSOR_LOGGER_ENABLED)
  #include "ChronoLog.h"
  ChronoLogger soilLogger("HMS_SoilSensor", CHRONOLOG_LEVEL_DEBUG);
#endif

#if defined(HMS_SOIL_SENSOR_PLATFORM_ARDUINO)  
    HMS_SoilSensor(uint8_t pin, HMS_SOIL_SENSOR_Type sensorType = HMS_SOIL_SENSOR_TYPE_YL69){}
#elif defined(HMS_SOIL_SENSOR_PLATFORM_ESP_IDF)
    HMS_SoilSensor(uint8_t pin, HMS_SOIL_SENSOR_Type sensorType = HMS_SOIL_SENSOR_TYPE_YL69){}
#elif defined(HMS_SOIL_SENSOR_PLATFORM_ZEPHYR)
    HMS_SoilSensor(uint8_t pin, HMS_SOIL_SENSOR_Type sensorType = HMS_SOIL_SENSOR_TYPE_YL69){}
#elif defined(HMS_SOIL_SENSOR_PLATFORM_STM32_HAL)
    HMS_SoilSensor::HMS_SoilSensor(ADC_HandleTypeDef *hadc, uint32_t channel, HMS_SOIL_SENSOR_Type sensorType): HMS_SOIL_SENSOR_hadc{hadc}, adcChannel{channel}, Type{sensorType}{
        if(HMS_SOIL_SENSOR_hadc == NULL)
        {
            #ifdef HMS_SOIL_SENSOR_LOGGER_ENABLED
            soilLogger.error("ADC handle is null");
            #endif
        }
        setDefaultValues();
    }
#endif
HMS_SoilSensor::~HMS_SoilSensor(){}

#if defined(HMS_SOIL_SENSOR_PLATFORM_STM32_HAL)
int32_t HMS_SoilSensor::readSoilADC(uint32_t channel)
{
    if (HMS_SOIL_SENSOR_hadc == NULL) return -1;

    ADC_ChannelConfTypeDef sConfig = {0};
    sConfig.Channel      = channel;
    sConfig.Rank         = ADC_REGULAR_RANK_1;
    sConfig.SamplingTime = ADC_SAMPLETIME_2CYCLES_5;
    sConfig.SingleDiff   = ADC_SINGLE_ENDED;
    sConfig.OffsetNumber = ADC_OFFSET_NONE;
    sConfig.Offset       = 0;
    if (HAL_ADC_ConfigChannel(HMS_SOIL_SENSOR_hadc, &sConfig) != HAL_OK) return -1;
    if (HAL_ADC_Start(HMS_SOIL_SENSOR_hadc) != HAL_OK) return -1;
    if (HAL_ADC_PollForConversion(HMS_SOIL_SENSOR_hadc, 100) != HAL_OK) return -1;
    int32_t val = (int32_t)HAL_ADC_GetValue(HMS_SOIL_SENSOR_hadc);
    HAL_ADC_Stop(HMS_SOIL_SENSOR_hadc);
    return val;
}
#endif
// call initlization to calibrate the resistnace and  Read the sensor 
HMS_SOIL_SENSOR_Status HMS_SoilSensor::init(){
    ReadSensor();
    return HMS_SOIL_SENSOR_OK;
}

void HMS_SoilSensor::ReadSensor()
{
    sensorVolt = getVoltage(true, false, 0); 
    adc = getADC();   
    CalculateMoisture(sensorVolt,adc);
    
}
float HMS_SoilSensor::getVoltage(bool read, bool injected, int value)
{   
    float voltage;
    if(read){
        float avg = 0.0;
        for(int i = 0; i< retries; i++){
            #if defined(HMS_SOIL_SENSOR_PLATFORM_ARDUINO)
                adc = analogRead(pin);
            #elif defined(HMS_SOIL_SENSOR_PLATFORM_STM32_HAL)
            // STM32 HAL ADC reading - assumes ADC is configured in CubeMX
                int32_t raw = readSoilADC(adcChannel);
                if (raw < 0) {
                    #ifdef HMS_SOIL_SENSOR_LOGGER_ENABLED
                    soilLogger.error("ADC read failed");
                    return 0.0;
                    #endif
                }
                adc = (float)raw;
            #elif defined(HMS_SOIL_SENSOR_PLATFORM_ESP_IDF)
            // ESP-IDF ADC reading - needs ADC configuration
                adc = 2048; // Placeholder - needs actual ESP-IDF implementation
            #elif defined(HMS_SOIL_SENSOR_PLATFORM_ZEPHYR)
            // Zephyr ADC reading - needs ADC device binding
                adc = 2048; // Placeholder - needs actual Zephyr implementation
            #endif

            avg += adc;
            #ifdef HMS_SOIL_SENSOR_LOGGER_ENABLED
            soilLogger.debug("ADC Value: %.2f", adc);
            #endif
            soilDelay(retryInterval);
        }

        #if defined(HMS_SOIL_SENSOR_PLATFORM_STM32_HAL)
            HAL_ADC_Stop(HMS_SOIL_SENSOR_hadc);
        #endif

        voltage = (avg / retries) * voltageResolution / ((pow(2, adcBitResolution)) - 1);
        sensorVolt = voltage;
    }
    else if(injected){
        // External voltage injection (for testing or external ADC)
        voltage = (value) * voltageResolution / ((pow(2, adcBitResolution)) - 1);
        sensorVolt = voltage;
    }
    else{
        voltage = sensorVolt;
    }
    #ifdef HMS_SOIL_SENSOR_LOGGER_ENABLED
    soilLogger.debug("Sensor Voltage: %.2f", voltage);
    soilLogger.debug("Moisture Percentage: %.2f", moisture);    
    #endif
    return voltage;
}
void HMS_SoilSensor::soilDelay(uint32_t ms){
    #if defined(HMS_SOIL_SENSOR_PLATFORM_ARDUINO)
        delay(ms);
    #elif defined(HMS_SOIL_SENSOR_PLATFORM_STM32_HAL)
        HAL_Delay(ms);
    #elif defined(HMS_SOIL_SENSOR_PLATFORM_ESP_IDF)
        vTaskDelay(ms / portTICK_PERIOD_MS);
    #elif defined(HMS_SOIL_SENSOR_PLATFORM_ZEPHYR)
        k_msleep(ms);
    #endif
}
void HMS_SoilSensor::setDefaultValues(){
    if(Type == HMS_SOIL_SENSOR_TYPE_YL69){
        dry = HMS_SOIL_SENSOR_MOISTURE_DRY_VALUE;
        wet = HMS_SOIL_SENSOR_MOISTURE_WET_VALUE;
    }else  return;
    setDryThreshold(dry);
    setWetThreshold(wet);
}

float HMS_SoilSensor::CalculateMoisture(float sensorVolt, float adcValue)
{
    adcRange = CalculateAdcRange(adcValue);
    if (adcRange == HMS_SOIL_ADC_RANGE_INVALID)  return -1.0f;
        
    // ---- Calculate Moisture Percentage ----
    MoisturePercentage = ((HMS_SOIL_SENSOR_MOISTURE_DRY_VALUE - adcValue) / (HMS_SOIL_SENSOR_MOISTURE_DRY_VALUE - HMS_SOIL_SENSOR_MOISTURE_WET_VALUE)) * HMS_SOIL_SENSOR_PERCENTAGE;
    MoisturePercentage = std::clamp(MoisturePercentage, 0.0f, 100.0f);

    const char* rangeText = "";
    switch (adcRange) {
        case HMS_SOIL_ADC_RANGE_WET:                rangeText = "Wet";          break;
        case HMS_SOIL_ADC_RANGE_MEDIUM:             rangeText = "Medium";       break;
        case HMS_SOIL_ADC_RANGE_DRY:                rangeText = "Dry";          break;
        case HMS_SOIL_ADC_RANGE_VERY_DRY:           rangeText = "Very Dry";     break;
        default:                                    rangeText = "Invalid";      break;
    }
    
    #ifdef HMS_SOIL_SENSOR_LOGGER_ENABLED
        soilLogger.debug("ADC: %.2f | Voltage: %.2f | Moisture: %.2f%% | Range: %s",
                     adcValue, sensorVolt, MoisturePercentage, rangeText);
    #endif
    return MoisturePercentage;
}
HMS_SOIL_ADC_RANGE HMS_SoilSensor::CalculateAdcRange(float adcValue){
    if (adcValue < 0 || adcValue > HMS_SOIL_SENSOR_MOISTURE_MAX_VALUE || isnan(adcValue)) {
        #ifdef HMS_SOIL_SENSOR_LOGGER_ENABLED
        soilLogger.error("Invalid ADC value: %.2f", adcValue);
        #endif
        return HMS_SOIL_ADC_RANGE_INVALID; 
    }
    if (adcValue <= HMS_SOIL_ADC_WET_MAX && adcValue > HMS_SOIL_ADC_VERY_WET_MAX) {
        return  HMS_SOIL_ADC_RANGE_WET;
    } 
    else if (adcValue <= HMS_SOIL_ADC_MEDIUM_MAX) {
        return  HMS_SOIL_ADC_RANGE_MEDIUM;
    } 
    else if (adcValue <= HMS_SOIL_ADC_DRY_MAX) {
        return  HMS_SOIL_ADC_RANGE_DRY;
    } 
    else {
        return  HMS_SOIL_ADC_RANGE_VERY_DRY;
    }
}

