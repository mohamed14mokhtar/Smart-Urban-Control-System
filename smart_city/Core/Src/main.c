/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "dma.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stdio.h"
#include "string.h"
#include "stander_file.h"
#include "keypad_cfg.h"


#include "servo_cfg.h"
#include "ultrasonic_cfg.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim);
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define NUM_CHANNELS 3  // Number of ADC channels (photoresistor and potentiometer)
#define PHOTORESISTOR_CHANNEL ADC_CHANNEL_0  // ADC channel for the photoresistor
#define POTENTIOMETER_CHANNEL ADC_CHANNEL_1  // ADC channel for the potentiometer
#define GAS_CHANNEL           ADC_CHANNEL_2  // ADC channel for the gas

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
HAL_StatusTypeDef ret;                // return status
uint32_t IC_Val1 ;                    // for ultasonic read
uint32_t IC_Val2 ;                    // for ultasonic read
uint32_t Difference ;                 // different between val1 and val2
uint8_t Is_First_Captured ;           // is the first value captured ?
uint8_t Distance ;                    // distance from ultrasonic

uint8_t ir_sensor ;
uint8_t gas_sensor ;
uint8_t flam_sensor ;


uint32_t ADC_VALUE[NUM_CHANNELS] ;             // Array to store ADC results
uint32_t photoresistor_value = 0;              // value between 0 : 100
uint32_t potentiometer_value = 0;              // value between 0 : 100
uint32_t gas_value = 0;                        // value between 0 : 100
uint8_t  flame_value = 1 ;                     // value between 0 : 1  defualte 1
uint8_t  uart_recieve ;                        //recieve data from uaer

uint32_t previousMillis = 0;
uint32_t currentMillis  = 0;
uint32_t counterOutside = 0;                  //For testing only
uint32_t counterInside  = 0;                  //For testing only
uint32_t counterInside2 = 0;                  //For testing only

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
   if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_2)  // if the interrupt source is channel1
   {
       if (Is_First_Captured == 0) // if the first value is not captured
       {
           IC_Val1 = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_2); // read the first value
           Is_First_Captured = 1;  // set the first captured as true
           // Now change the polarity to falling edge
           __HAL_TIM_SET_CAPTUREPOLARITY(htim, TIM_CHANNEL_2, TIM_INPUTCHANNELPOLARITY_FALLING);
       }

       else if (Is_First_Captured == 1)   // if the first is already captured
       {
           IC_Val2 = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_2);  // read second value
           __HAL_TIM_SET_COUNTER(htim, 0);  // reset the counter

           if (IC_Val2 > IC_Val1)
           {
               Difference = IC_Val2-IC_Val1;
           }

           else if (IC_Val1 > IC_Val2)
           {
               Difference = (0xffff - IC_Val1) + IC_Val2;
           }

           Distance = Difference * (0.034/2);
           Is_First_Captured = 0; // set it back to false

           // set polarity to rising edge
           __HAL_TIM_SET_CAPTUREPOLARITY(htim, TIM_CHANNEL_2, TIM_INPUTCHANNELPOLARITY_RISING);
           __HAL_TIM_DISABLE_IT(&htim1, TIM_IT_CC2);
       }
   }
}


/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_TIM1_Init();
  MX_TIM2_Init();
  MX_ADC1_Init();
  MX_TIM10_Init();
  MX_TIM11_Init();
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */
  HAL_UART_Receive_IT(&huart2,&uart_recieve,1);
  HAL_TIM_IC_Start_IT(&htim1, TIM_CHANNEL_2);                  //start timer for ultasonic


  //HAL_ADC_Start_DMA(&hadc1, adc_values, NUM_CHANNELS);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {

	  //servo_Motor_Start_angle_90(&servo);
	  HCSR04_Read(&ultra_1);
	  if(uart_recieve == 'a'){
		  servo_Motor_Start_angle_90(&servo);
	  }else{
		  if(Distance <= 9){
		  	  servo_Motor_Start_angle_90(&servo);
		  }else{
		  	  servo_Motor_ADC_Control(&servo,potentiometer_value);
		  }
	  }

	  // Read the photoresistor channel
	  ADC_ChannelConfTypeDef sConfig = {0};
	  sConfig.Channel = ADC_CHANNEL_0;
	  sConfig.Rank = 1;
	  sConfig.SamplingTime = ADC_SAMPLETIME_15CYCLES;
	  HAL_ADC_ConfigChannel(&hadc1, &sConfig);  // Configure ADC to photoresistor channel

	  HAL_ADC_Start(&hadc1);
	  if (HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY) == HAL_OK)
	  {
	     ADC_VALUE[0] = HAL_ADC_GetValue(&hadc1);
	  }
	  HAL_ADC_Stop(&hadc1);  // Stop ADC after reading
	  potentiometer_value = ((ADC_VALUE[0] * 100)/4100);

	  // Read the potentiometer channel
	  sConfig.Channel = ADC_CHANNEL_1;
	  sConfig.Rank = 1;
	  HAL_ADC_ConfigChannel(&hadc1, &sConfig);  // Configure ADC to potentiometer channel

	  HAL_ADC_Start(&hadc1);
	  if (HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY) == HAL_OK)
	  {
	      ADC_VALUE[1] = HAL_ADC_GetValue(&hadc1);
	  }
	  HAL_ADC_Stop(&hadc1);  // Stop ADC after reading
	  photoresistor_value = ((ADC_VALUE[1] * 100)/4000);

	   sConfig.Channel = ADC_CHANNEL_9;
	   sConfig.Rank = 1;
	   sConfig.SamplingTime = ADC_SAMPLETIME_15CYCLES;
	   HAL_ADC_ConfigChannel(&hadc1, &sConfig);  // Configure ADC to photoresistor channel

	   HAL_ADC_Start(&hadc1);
	   if (HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY) == HAL_OK)
	   {
	       ADC_VALUE[2] = HAL_ADC_GetValue(&hadc1);
	   }
	   HAL_ADC_Stop(&hadc1);  // Stop ADC after reading
	   gas_value = ((ADC_VALUE[2] * 100)/4000);


	   // photo resistance chick
	   if(uart_recieve == 'b'){
		   HAL_GPIO_WritePin(GPIOA, LED1_Pin, GPIO_PIN_SET);
		   HAL_GPIO_WritePin(GPIOB, LED4_Pin, GPIO_PIN_SET);
		   HAL_Delay(20);
		   HAL_GPIO_WritePin(GPIOA, LED2_Pin, GPIO_PIN_SET);
		   HAL_GPIO_WritePin(GPIOB, LED5_Pin, GPIO_PIN_SET);
		   HAL_Delay(20);
		   HAL_GPIO_WritePin(GPIOA, LED3_Pin, GPIO_PIN_SET);
		   HAL_GPIO_WritePin(GPIOB, LED6_Pin, GPIO_PIN_SET);
	   }else if(uart_recieve == 'c'){
		   HAL_GPIO_WritePin(GPIOA,LED1_Pin|LED2_Pin|LED3_Pin,GPIO_PIN_RESET);
		   HAL_GPIO_WritePin(GPIOB,LED4_Pin|LED5_Pin|LED6_Pin,GPIO_PIN_RESET);
	   }else{
		   if(photoresistor_value < 12){
			   HAL_GPIO_WritePin(GPIOA, LED1_Pin, GPIO_PIN_SET);
			   HAL_GPIO_WritePin(GPIOB, LED4_Pin, GPIO_PIN_SET);
			   HAL_Delay(20);
			   HAL_GPIO_WritePin(GPIOA, LED2_Pin, GPIO_PIN_SET);
			   HAL_GPIO_WritePin(GPIOB, LED5_Pin, GPIO_PIN_SET);
			   HAL_Delay(20);
			   HAL_GPIO_WritePin(GPIOA, LED3_Pin, GPIO_PIN_SET);
			   HAL_GPIO_WritePin(GPIOB, LED6_Pin, GPIO_PIN_SET);
		   }else{
			   HAL_GPIO_WritePin(GPIOA,LED1_Pin|LED2_Pin|LED3_Pin,GPIO_PIN_RESET);
			   HAL_GPIO_WritePin(GPIOB,LED4_Pin|LED5_Pin|LED6_Pin,GPIO_PIN_RESET);
		   }
	   }
	   // buzzer and water pump
	   	flame_value = HAL_GPIO_ReadPin(flame_GPIO_Port, flame_Pin);  // reed logic of flame sensor
	   	if(uart_recieve == 'f'){
	   		HAL_GPIO_WritePin(GPIOB, Relay3_Pin, GPIO_PIN_SET);
	   	}else{
		   	if((gas_value < 5) || (flame_value == 0)){
		    // pump initialization
		   	HAL_GPIO_WritePin(GPIOA, buzzer_Pin, GPIO_PIN_SET);
		   	HAL_GPIO_WritePin(GPIOB, Relay3_Pin, GPIO_PIN_RESET);
		   	}else{
		   		HAL_GPIO_WritePin(GPIOA, buzzer_Pin, GPIO_PIN_RESET);
		   		HAL_GPIO_WritePin(GPIOB, Relay3_Pin, GPIO_PIN_SET);
		   	}
	   	}

	   if(uart_recieve == 'd'){
	   		HAL_GPIO_WritePin(GPIOA, buzzer_Pin, GPIO_PIN_SET);
	   		HAL_GPIO_WritePin(GPIOB, Relay3_Pin, GPIO_PIN_RESET);
	   }else if (uart_recieve == 'e'){
	   		HAL_GPIO_WritePin(GPIOA, buzzer_Pin, GPIO_PIN_RESET);
	   		HAL_GPIO_WritePin(GPIOB, Relay3_Pin, GPIO_PIN_SET);
	   }

//	   if(uart_recieve == 'j'){
//		   HAL_GPIO_WritePin(GPIOB, Relay1_Pin, GPIO_PIN_SET);
//		   HAL_GPIO_WritePin(GPIOB, Relay2_Pin, GPIO_PIN_SET);
//	   }else if (uart_recieve == 'g'){
//		   HAL_GPIO_WritePin(GPIOB, Relay1_Pin, GPIO_PIN_SET);
//		   HAL_GPIO_WritePin(GPIOB, Relay2_Pin, GPIO_PIN_RESET);
//	   }else if (uart_recieve == 'h'){
//		   HAL_GPIO_WritePin(GPIOB, Relay1_Pin, GPIO_PIN_RESET);
//		   HAL_GPIO_WritePin(GPIOB, Relay2_Pin, GPIO_PIN_SET);
//	   }

    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 25;
  RCC_OscInitStruct.PLL.PLLN = 128;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV4;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
	if(huart == &huart2){

		HAL_UART_Receive_IT(&huart2,&uart_recieve,1);
	}

}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  counterOutside++; //For testing only
  currentMillis = HAL_GetTick();
  if (GPIO_Pin == GPIO_PIN_3 && (currentMillis - previousMillis > 10))
  {
    counterInside++; //For testing only
    HAL_GPIO_WritePin(GPIOB, Relay1_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOB, Relay2_Pin, GPIO_PIN_RESET);
    HAL_TIM_Base_Start_IT(&htim10);

    previousMillis = currentMillis;
  }
  if (GPIO_Pin == GPIO_PIN_4 && (currentMillis - previousMillis > 10))
  {
    counterInside2++; //For testing only
    HAL_GPIO_WritePin(GPIOB, Relay1_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOB, Relay2_Pin, GPIO_PIN_SET);
    HAL_TIM_Base_Start_IT(&htim10);

    previousMillis = currentMillis;
  }
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    // Check if the interrupt was triggered by Timer 3

   if (htim->Instance == TIM10)
        {
	        HAL_GPIO_WritePin(GPIOB, Relay1_Pin, GPIO_PIN_RESET);
	        HAL_GPIO_WritePin(GPIOB, Relay2_Pin, GPIO_PIN_RESET);
	        HAL_TIM_Base_Stop_IT(&htim10);
        }
   if (htim->Instance == TIM11)
        {
	        HAL_GPIO_WritePin(GPIOB, Relay1_Pin, GPIO_PIN_RESET);
	        HAL_GPIO_WritePin(GPIOB, Relay2_Pin, GPIO_PIN_RESET);
	        HAL_TIM_Base_Stop_IT(&htim11);
        }
}


/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
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
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
