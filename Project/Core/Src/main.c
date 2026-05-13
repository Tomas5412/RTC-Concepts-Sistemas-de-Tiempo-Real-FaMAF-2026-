/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
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
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "task.h"
#include "semphr.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
const typedef struct {
	GPIO_TypeDef* port;
	uint16_t pin;
	uint16_t delay;
} toggleInstruction_typedef;

const typedef struct{
	GPIO_TypeDef* port;
	uint16_t pin;
	uint16_t delay;
	TaskHandle_t *task;
} toggleReadTaskInstruction_typedef;

/* 

const typedef struct{
	GPIO_TypeDef *port;
	uint16_t on_pin;
	void *sem_on;
	uint16_t off_pin;
	void *sem_off;
}toggleSemaphoreInstruction_typedef; // UNUSED - made for generalizing Challenge 7


const typedef struct{
	GPIO_TypeDef *port;
	uint16_t pins[4];
	uint16_t delay;
	void *sem;
}multipleLedInstruction_typedef; // UNUSED - made for generalizing Challenge 8 and 9 

const typedef struct{
	TaskHandle_t task;
	void *sem;
}taskSuspensionInstruction_typedef; // UNUSED - made for generalizing challenge 9

const typedef struct{
	GPIO_TypeDef *port;
	uint16_t pins[4];
	uint16_t delay;
	uint16_t timespan;
}actThenDeleteInstruction_typedef; // UNUSED - made for generalizing challenge 10
*/

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
SemaphoreHandle_t semChallenge9 = NULL;
SemaphoreHandle_t semChallenge6 = NULL;

SemaphoreHandle_t sem1Ch7 = NULL;
SemaphoreHandle_t sem2Ch7 = NULL;
SemaphoreHandle_t sem3Ch7 = NULL;

SemaphoreHandle_t semCh8 = NULL;

void vTask1Ch1(void *pvParameters){
	while(1){
		HAL_GPIO_TogglePin(GPIOD, LD4_Pin);
		HAL_Delay(50);
	}
};

void vTask2Ch1(void *pvParameters){
	while(1){
		HAL_GPIO_TogglePin(GPIOD, LD3_Pin);
		HAL_Delay(100);
	}
};

void vTask3Ch1(void *pvParameters){
	while(1){
		HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_14);
		HAL_Delay(200);
	}
};

void vTask4Ch1(void *pvParameters){
	while(1){
		HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_15);
		HAL_Delay(400);
	}
};

void vChallenge1(){
	xTaskCreate(vTask1Ch1,"Task1",100,NULL,1,NULL);
	xTaskCreate(vTask2Ch1,"Task2",100,NULL,1,NULL);
	xTaskCreate(vTask3Ch1,"Task3",100,NULL,1,NULL);
	xTaskCreate(vTask4Ch1,"Task4",100,NULL,1,NULL);
};



void vTareaParpadeo(void *pvParameters){
	toggleInstruction_typedef *specific_pin = pvParameters;
	TickType_t delay = pdMS_TO_TICKS(specific_pin->delay);
	while(1){
        HAL_GPIO_TogglePin(specific_pin->port, specific_pin->pin);
        vTaskDelay(delay);
	};
};

static const toggleInstruction_typedef instructionsCh2[4] = {{GPIOD, LD4_Pin, 50}, {GPIOD, LD3_Pin, 100},  {GPIOD, LD5_Pin, 200}, {GPIOD, LD6_Pin, 400}};

void vChallenge2(){
    xTaskCreate(vTareaParpadeo, "Task1", 100, (void *) &instructionsCh2[0], 1, NULL);
    xTaskCreate(vTareaParpadeo, "Task2", 100, (void *) &instructionsCh2[1], 1, NULL);
    xTaskCreate(vTareaParpadeo, "Task3", 100, (void *) &instructionsCh2[2], 1, NULL);
    xTaskCreate(vTareaParpadeo, "Task4", 100, (void *) &instructionsCh2[3], 1, NULL);
};

void vTaskLEDCh3(void *pvParameters){
	toggleInstruction_typedef *instruction = pvParameters;
	while(1){
        HAL_GPIO_TogglePin(instruction->port, instruction->pin);
        HAL_Delay(instruction->delay);
	}
}

void vTaskButtonCh3(void *pvParameters){
	toggleInstruction_typedef *instruction = pvParameters;
	TickType_t delay = pdMS_TO_TICKS(instruction->delay); // This delay isn't for blinking the led, but for waking up the task for the polling!
	while(1){
		GPIO_PinState buttonState = HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0);
		// GPIO_PinState ledState = HAL_GPIO_ReadPin(instruction->port,instruction->pin);
		if(buttonState == GPIO_PIN_SET){
			//
			HAL_GPIO_WritePin(instruction->port,instruction->pin, GPIO_PIN_SET);
		}
		if(buttonState == GPIO_PIN_RESET){
			// Reset the button led (just once)
			HAL_GPIO_WritePin(instruction->port,instruction->pin, GPIO_PIN_RESET);
		}
		vTaskDelay(delay);
	}
}

const toggleInstruction_typedef instructionsCh3[2] = {{GPIOD, LD4_Pin, 500}, {GPIOD, LD3_Pin, 10}};
void vChallenge3(){
    xTaskCreate(vTaskLEDCh3, "LED task", 100, (void *) &instructionsCh3[0], 0, NULL);
    xTaskCreate(vTaskButtonCh3, "Button task", 100, (void *) &instructionsCh3[1], 1, NULL);

}

void vTask1Ch4(void *pvParameters){
	toggleInstruction_typedef *specific_pin = pvParameters;
	TickType_t delay = pdMS_TO_TICKS(specific_pin->delay);
	while(1){
        HAL_GPIO_TogglePin(specific_pin->port, specific_pin->pin);
        HAL_DELAY(100);
        vTaskDelay(delay);
	};
}

void vTask2Ch4(void *pvParameters){
	toggleInstruction_typedef *specific_pin = pvParameters;
	TickType_t tickCount = xTaskGetTickCount();
	TickType_t delay = pdMS_TO_TICKS(specific_pin->delay);
	while(1){
        HAL_GPIO_TogglePin(specific_pin->port, specific_pin->pin);
        HAL_DELAY(100);
        vTaskDelayUntil(&tickCount,delay);
	};
}

const toggleInstruction_typedef instructionsCh4[2] = {{GPIOD, LD4_Pin, 400}, {GPIOD, LD3_Pin, 500}};

void vChallenge4(){
    xTaskCreate(vTask1Ch4, "task1", 100, (void *) &instructionsCh4[0], 0, NULL);
    xTaskCreate(vTask2Ch4, "task2", 100, (void *) &instructionsCh4[1], 0, NULL);
}

void vTask1Ch5(void *pvParameters){

	toggleReadTaskInstruction_typedef *instruction = pvParameters;
	uint16_t actual_priority = 1;
	uint16_t button_interval = pdMS_TO_TICKS(3000);

	uint16_t time = 0; // Would ALL these (except waiting) be better off not initiated?
	uint16_t button_time = 0;
	uint16_t target_priority = 0;
	uint16_t waiting = pdFALSE;
	uint16_t interval = 0;
	// TickType_t tickCount = xTaskGetTickCount();
	GPIO_PinState buttonState = GPIO_PIN_RESET;

	while(1){
		buttonState = HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0);

		if(buttonState != GPIO_PIN_RESET){
			target_priority = uxTaskPriorityGet(*(instruction->task));
			vTaskPrioritySet(NULL, target_priority + (uint16_t) 1); // (uint16_t) Just in case !!
			button_time = HAL_GetTick();
			waiting = pdTRUE;
		}

		if(waiting == pdTRUE){
			time = HAL_GetTick();
			interval = time - button_time;
			if(interval >= button_interval){
				vTaskPrioritySet(NULL, actual_priority);
				waiting = pdFALSE;
			}
		}
		HAL_GPIO_TogglePin(instruction->port, instruction->pin);

		HAL_Delay(instruction->delay); // Not vTaskDelay! because that wouldn't starve the other task.
	}
}


void vChallenge5(){
	  static TaskHandle_t task2Handler = NULL;
	  static const toggleInstruction_typedef task2_instruction = {GPIOD, LD5_Pin, 50};
	  static toggleReadTaskInstruction_typedef task1_instruction = {GPIOD, LD4_Pin, 300, &task2Handler};
	  xTaskCreate(vTareaParpadeo,"Task2",100,(void *) &task2_instruction,1,&task2Handler);
	  xTaskCreate(vTask1Ch5,"Task1",100,(void *) &task1_instruction,1,NULL);
}


void vTask1Challenge6(void *pvParameters){
	uint16_t status;
	while(1){
		status = xSemaphoreTake(semChallenge6, 0);
		if(status == pdTRUE) {
			HAL_GPIO_TogglePin(GPIOD,LD3_Pin);
		}
	}
}

void vChallenge6(){
  semChallenge6 = xSemaphoreCreateBinary();
	const toggleInstruction_typedef taskInstruction = {GPIOD, LD4_Pin, 0};
	xTaskCreate(vTask1Challenge6,"Task1",100,(void *) &taskInstruction,1,NULL);
}


void vTask1Ch7(void *pvParameters){
	uint16_t status;
	while(1){
		status = xSemaphoreTake(sem3Ch7, 10);
		if(status == pdTRUE){
			HAL_GPIO_WritePin(GPIOD, LD5_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOD, LD3_Pin, GPIO_PIN_SET);
			vTaskDelay(pdMS_TO_TICKS(500));
			status = xSemaphoreGive(sem1Ch7);
		}
	}
}

void vTask2Ch7(void *pvParameters){
	uint16_t status;
	while(1){
		status = xSemaphoreTake(sem1Ch7, 10);
		if(status == pdTRUE){
			HAL_GPIO_WritePin(GPIOD, LD3_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOD, LD4_Pin, GPIO_PIN_SET);
			vTaskDelay(pdMS_TO_TICKS(500));
			status = xSemaphoreGive(sem2Ch7);
		}
	}
}

void vTask3Ch7(void *pvParameters){
	uint16_t status;
	while(1){
		status = xSemaphoreTake(sem2Ch7, 10);
		if(status == pdTRUE){
			HAL_GPIO_WritePin(GPIOD, LD4_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOD, LD5_Pin, GPIO_PIN_SET);
			vTaskDelay(pdMS_TO_TICKS(500));
			status = xSemaphoreGive(sem3Ch7);
		}
	}
}

void vChallenge7(){
	sem1Ch7 = xSemaphoreCreateBinary();
	sem2Ch7 = xSemaphoreCreateBinary();
	sem3Ch7 = xSemaphoreCreateBinary();

	xSemaphoreGive(sem3Ch7);

	xTaskCreate(vTask1Ch7,"Task",256,NULL,1,NULL);
	xTaskCreate(vTask2Ch7,"Task",256,NULL,1,NULL);
	xTaskCreate(vTask3Ch7,"Task",256,NULL,1,NULL);

	osKernelStart();
}

void vTaskSequentialBlinkCh8(void *pvParameters){

	uint16_t leds[4] = {LD3_Pin,LD4_Pin,LD5_Pin,LD6_Pin};
	TickType_t delay = pdMS_TO_TICKS(300);
	// TickType_t tickCount = xTaskGetTickCount();
	uint16_t sem_taken = pdFALSE;
	uint16_t counter = 0;
	GPIO_PinState ledState;

	while(1){
		if(sem_taken == pdTRUE){
			ledState = HAL_GPIO_ReadPin(GPIOD, leds[counter]);
			if(ledState == GPIO_PIN_SET){
				HAL_GPIO_WritePin(GPIOD,leds[counter],GPIO_PIN_RESET);
				counter ++;
				if(counter == 4){
					sem_taken = pdFALSE;
					counter = 0;
					xSemaphoreGive(semCh8);
				}
			}
			else{
				HAL_GPIO_WritePin(GPIOD,leds[counter],GPIO_PIN_SET);
			}
			vTaskDelay(delay); //vTaskDelayUntil breaks things!
		}
		else{
			sem_taken = xSemaphoreTake(semCh8, 0);
		}
	}
}

void vTaskParallelBlinkCh8(void *pvParameters){

	uint16_t leds[4] = {LD3_Pin,LD4_Pin,LD5_Pin,LD6_Pin};
	TickType_t delay = pdMS_TO_TICKS(300);
	// TickType_t tickCount = xTaskGetTickCount();
	uint16_t sem_taken = pdFALSE;
	GPIO_PinState ledState;

	while(1){
		if(sem_taken == pdTRUE){
			ledState = HAL_GPIO_ReadPin(GPIOD,leds[0]);
			if(ledState == GPIO_PIN_RESET){
				for(uint16_t i = 0; i < 4; i++) HAL_GPIO_WritePin(GPIOD,leds[i],GPIO_PIN_SET);
			}
			else{
				for(uint16_t i = 0; i < 4; i++) HAL_GPIO_WritePin(GPIOD,leds[i],GPIO_PIN_RESET);
				xSemaphoreGive(semCh8);
				sem_taken = pdFALSE;
			}
			vTaskDelay(delay); //vTaskDelayUntil breaks things!
		}
		else{
			sem_taken = xSemaphoreTake(semCh8,0);
		}
}}

void vChallenge8(){
	semCh8 = xSemaphoreCreateMutex();
	xTaskCreate(vTaskSequentialBlinkCh8,"Sequential",100,NULL,1,NULL);
	xTaskCreate(vTaskParallelBlinkCh8,"Parallel",100,NULL,1,NULL);
}

void vTaskSequentialBlinkCh9(void *pvParameters){
	TickType_t delay = pdMS_TO_TICKS(500);
	TickType_t tickCount = xTaskGetTickCount();
	uint16_t counter = 0;
	GPIO_PinState ledState;
	uint16_t leds[4] = {LD3_Pin,LD4_Pin,LD5_Pin,LD6_Pin};

		while(1){
			ledState = HAL_GPIO_ReadPin(GPIOD, leds[counter]);
			if(ledState == GPIO_PIN_SET){
				HAL_GPIO_WritePin(GPIOD,leds[counter],GPIO_PIN_RESET);
				counter = (counter + 1) % 4;
			}
			else{
				HAL_GPIO_WritePin(GPIOD,leds[counter],GPIO_PIN_SET);
			}
			vTaskDelayUntil(&tickCount,delay);
			}
}

void vTaskButtonControlCh9(void *pvParameters){
	TaskHandle_t instruction = pvParameters;
	uint16_t status = pdFALSE;
	while(1){
		status = xSemaphoreTake(semChallenge9,portMAX_DELAY);
		if(status == pdTRUE) {
			vTaskSuspend(instruction);
			status = xSemaphoreTake(semChallenge9,portMAX_DELAY);
			if(status == pdTRUE) {
				vTaskResume(instruction);
			}
		}
	}
}


void vChallenge9(){
  semChallenge9 = xSemaphoreCreateBinary();
	TaskHandle_t taskHandler = NULL;
	xTaskCreate(vTaskSequentialBlinkCh9,"Sequential",100,(void *)NULL,1,&taskHandler);
	xTaskCreate(vTaskButtonControlCh9,"Button",100,(void *)taskHandler,1,NULL);

}

void vLightShowCh10(void *pvParameters){
	const uint16_t leds[4] = {LD3_Pin,LD4_Pin,LD5_Pin,LD6_Pin};
	TickType_t lifespan = pdMS_TO_TICKS(10000);
	TickType_t spawn_time = xTaskGetTickCount();
	TickType_t tickCounter = xTaskGetTickCount();
	TickType_t delay = pdMS_TO_TICKS(300);
	TickType_t dtime = 0;

	uint16_t counter = 0;
	GPIO_PinState ledState;

	while(1){
		ledState = HAL_GPIO_ReadPin(GPIOD, leds[counter]);
		if(ledState == GPIO_PIN_SET){
			HAL_GPIO_WritePin(GPIOD,leds[counter],GPIO_PIN_RESET);
			counter = (counter + 1) % 4;
		}
		else{
			HAL_GPIO_WritePin(GPIOD,leds[counter],GPIO_PIN_SET);
		}
		dtime = xTaskGetTickCount() - spawn_time;
		if(dtime >= lifespan)
			vTaskDelete(NULL);
		else
			vTaskDelayUntil(&tickCounter, delay);
	}
}

void vChallenge10(){
	xTaskCreate(vLightShowCh10,"task1",100,(void *)NULL,1,NULL);
}
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
osThreadId defaultTaskHandle;
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
void StartDefaultTask(void const * argument);

/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

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
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* definition and creation of defaultTask */
  osThreadDef(defaultTask, StartDefaultTask, osPriorityNormal, 0, 128);
  defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  // UNCOMMENT THE DESIRED CHALLENGE
  
  // vChallenge1();
  // vChallenge2();
  // vChallenge3();
  // vChallenge4();
  // vChallenge5();
  // vChallenge6();
  // vChallenge7();
  // vChallenge8();
  // vChallenge9();
  // vChallenge10();

  /* USER CODE END RTOS_THREADS */

  /* Start scheduler */
  osKernelStart();

  /* We should never get here as control is now taken by the scheduler */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
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
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  /* USER CODE BEGIN MX_GPIO_Init_1 */

  /* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(CS_I2C_SPI_GPIO_Port, CS_I2C_SPI_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(OTG_FS_PowerSwitchOn_GPIO_Port, OTG_FS_PowerSwitchOn_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, LD4_Pin|LD3_Pin|LD5_Pin|LD6_Pin
                          |Audio_RST_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : PE2 */
  GPIO_InitStruct.Pin = GPIO_PIN_2;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pin : CS_I2C_SPI_Pin */
  GPIO_InitStruct.Pin = CS_I2C_SPI_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(CS_I2C_SPI_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : PE4 PE5 MEMS_INT2_Pin */
  GPIO_InitStruct.Pin = GPIO_PIN_4|GPIO_PIN_5|MEMS_INT2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_EVT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pin : OTG_FS_PowerSwitchOn_Pin */
  GPIO_InitStruct.Pin = OTG_FS_PowerSwitchOn_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(OTG_FS_PowerSwitchOn_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : PDM_OUT_Pin */
  GPIO_InitStruct.Pin = PDM_OUT_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Alternate = GPIO_AF5_SPI2;
  HAL_GPIO_Init(PDM_OUT_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : PA0 */
  GPIO_InitStruct.Pin = GPIO_PIN_0;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : I2S3_WS_Pin */
  GPIO_InitStruct.Pin = I2S3_WS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Alternate = GPIO_AF6_SPI3;
  HAL_GPIO_Init(I2S3_WS_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : SPI1_SCK_Pin SPI1_MISO_Pin SPI1_MOSI_Pin */
  GPIO_InitStruct.Pin = SPI1_SCK_Pin|SPI1_MISO_Pin|SPI1_MOSI_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF5_SPI1;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : CLK_IN_Pin PB12 */
  GPIO_InitStruct.Pin = CLK_IN_Pin|GPIO_PIN_12;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Alternate = GPIO_AF5_SPI2;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : LD4_Pin LD3_Pin LD5_Pin LD6_Pin
                           Audio_RST_Pin */
  GPIO_InitStruct.Pin = LD4_Pin|LD3_Pin|LD5_Pin|LD6_Pin
                          |Audio_RST_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pins : I2S3_MCK_Pin I2S3_SCK_Pin I2S3_SD_Pin */
  GPIO_InitStruct.Pin = I2S3_MCK_Pin|I2S3_SCK_Pin|I2S3_SD_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Alternate = GPIO_AF6_SPI3;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pin : VBUS_FS_Pin */
  GPIO_InitStruct.Pin = VBUS_FS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(VBUS_FS_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : OTG_FS_ID_Pin OTG_FS_DM_Pin OTG_FS_DP_Pin */
  GPIO_InitStruct.Pin = OTG_FS_ID_Pin|OTG_FS_DM_Pin|OTG_FS_DP_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF10_OTG_FS;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : OTG_FS_OverCurrent_Pin */
  GPIO_InitStruct.Pin = OTG_FS_OverCurrent_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(OTG_FS_OverCurrent_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : Audio_SCL_Pin Audio_SDA_Pin */
  GPIO_InitStruct.Pin = Audio_SCL_Pin|Audio_SDA_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Alternate = GPIO_AF4_I2C1;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI0_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(EXTI0_IRQn);

  /* USER CODE BEGIN MX_GPIO_Init_2 */

  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	xSemaphoreGiveFromISR(semChallenge6,NULL);
	xSemaphoreGiveFromISR(semChallenge9,NULL);
}
/* USER CODE END 4 */

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void const * argument)
{
  /* USER CODE BEGIN 5 */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END 5 */
}

/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM3 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM3)
  {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */

  /* USER CODE END Callback 1 */
}

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
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
