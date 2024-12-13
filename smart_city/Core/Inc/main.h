/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define potentiometer_VALUE_Pin GPIO_PIN_0
#define potentiometer_VALUE_GPIO_Port GPIOA
#define photoresistor_value_Pin GPIO_PIN_1
#define photoresistor_value_GPIO_Port GPIOA
#define LED1_Pin GPIO_PIN_4
#define LED1_GPIO_Port GPIOA
#define LED2_Pin GPIO_PIN_5
#define LED2_GPIO_Port GPIOA
#define LED3_Pin GPIO_PIN_6
#define LED3_GPIO_Port GPIOA
#define buzzer_Pin GPIO_PIN_7
#define buzzer_GPIO_Port GPIOA
#define TRIG_Pin GPIO_PIN_0
#define TRIG_GPIO_Port GPIOB
#define GAS_value_Pin GPIO_PIN_1
#define GAS_value_GPIO_Port GPIOB
#define flame_Pin GPIO_PIN_2
#define flame_GPIO_Port GPIOB
#define LED4_Pin GPIO_PIN_12
#define LED4_GPIO_Port GPIOB
#define LED5_Pin GPIO_PIN_13
#define LED5_GPIO_Port GPIOB
#define LED6_Pin GPIO_PIN_14
#define LED6_GPIO_Port GPIOB
#define TIM_1_servo_Pin GPIO_PIN_9
#define TIM_1_servo_GPIO_Port GPIOA
#define TIM_2_ultrasonic_Pin GPIO_PIN_15
#define TIM_2_ultrasonic_GPIO_Port GPIOA
#define Relay3_Pin GPIO_PIN_7
#define Relay3_GPIO_Port GPIOB
#define Relay1_Pin GPIO_PIN_8
#define Relay1_GPIO_Port GPIOB
#define Relay2_Pin GPIO_PIN_9
#define Relay2_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
