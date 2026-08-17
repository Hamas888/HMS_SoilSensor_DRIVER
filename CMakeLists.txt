# HMS_SoilSensor_DRIVER/CMakeLists.txt

set(HMS_SoilSensor_DRIVER_VERSION 1.0.0)

set(HMS_SoilSensor_IS_STM32 FALSE)
if(CMAKE_SYSTEM_NAME STREQUAL "Generic" OR
   CMAKE_C_COMPILER_TARGET MATCHES "arm-none-eabi" OR
   CMAKE_CXX_COMPILER_TARGET MATCHES "arm-none-eabi" OR
   CMAKE_SYSTEM_PROCESSOR MATCHES "^(arm|ARM|armv)")
  set(HMS_SoilSensor_IS_STM32 TRUE)
endif()

# Check if we're building with Zephyr
if(DEFINED ZEPHYR_BASE)
    zephyr_include_directories(include)
    zephyr_library_sources(src/HMS_SoilSensor_DRIVER.cpp)

# Check if we're building with ESP-IDF
elseif(IDF_PROJECT)
    idf_component_register(
        SRCS "src/HMS_SoilSensor_DRIVER.cpp"
        INCLUDE_DIRS "include"
    )

# STM32 / generic embedded CMake project
elseif(HMS_SoilSensor_IS_STM32)
    add_library(HMS_SoilSensor_DRIVER STATIC
        src/HMS_SoilSensor_DRIVER.cpp
    )
    target_include_directories(HMS_SoilSensor_DRIVER PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
    )
    target_compile_features(HMS_SoilSensor_DRIVER PUBLIC cxx_std_17)

# Desktop / other CMake project
else()
    add_library(HMS_SoilSensor_DRIVER INTERFACE)
    target_include_directories(HMS_SoilSensor_DRIVER INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)
    target_compile_features(HMS_SoilSensor_DRIVER INTERFACE cxx_std_17)
endif()
