#include "stm32f0xx_hal.h"
#include <stdint.h>


I2C_HandleTypeDef hi2c1;
#define SHT3X_ADDR (0x44 << 1)

float sicaklik = 0.0f;
float nem = 0.0f;
HAL_StatusTypeDef ret;
uint8_t data[6];
uint8_t cmd[2] = {0x2C, 0x06};

void SystemClock_Config(void);
void MX_I2C1_Init(void);
void MX_GPIO_Init(void);

int main(void) {
    HAL_Init();
    SystemClock_Config();
    MX_GPIO_Init();
    MX_I2C1_Init();

    while (1) {

        ret = HAL_I2C_Master_Transmit(&hi2c1, SHT3X_ADDR, cmd, 2, 100);

        if (ret == HAL_OK) {
            HAL_Delay(15);

        ret = HAL_I2C_Master_Receive(&hi2c1, SHT3X_ADDR, data, 6, 100);

            if (ret == HAL_OK) {

                // Sıcaklık
                uint16_t raw_T = (data[0] << 8) | data[1];
                sicaklik = -45.0f + 175.0f * ((float)raw_T / 65535.0f);

                //  Nem
                uint16_t raw_RH = (data[3] << 8) | data[4];
                nem = 100.0f * ((float)raw_RH / 65535.0f);

            }
        }
        HAL_Delay(1000);
    }
}

// --- I2C AYARLARI (PB6 ve PB7 Pinleri) ---
void MX_I2C1_Init(void) {
    hi2c1.Instance = I2C1;
    hi2c1.Init.Timing = 0x2000090E; // 8MHz saat hızı için standart 100kHz I2C
    hi2c1.Init.OwnAddress1 = 0;
    hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
    hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
    hi2c1.Init.OwnAddress2 = 0;
    hi2c1.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
    hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
    hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
    HAL_I2C_Init(&hi2c1);
}


void MX_GPIO_Init(void) {
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_I2C1_CLK_ENABLE();

    GPIO_InitTypeDef GPIO_InitStruct = {0};
    // PB6 -> SCL, PB7 -> SDA
    GPIO_InitStruct.Pin = GPIO_PIN_6 | GPIO_PIN_7;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_OD; // Open Drain (I2C için şart)
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF1_I2C1;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}


void SystemClock_Config(void) {
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
    RCC_OscInitStruct.HSIState = RCC_HSI_ON;
    RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
    HAL_RCC_OscConfig(&RCC_OscInitStruct);

    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
    HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0);
}
