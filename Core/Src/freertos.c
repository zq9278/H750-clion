/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
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
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"
#include "Lcd_int.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* Hook prototypes */
void vApplicationTickHook(void);

/* USER CODE BEGIN 3 */
void vApplicationTickHook( void )
{
    lv_tick_inc(1);
   /* This function will be called by each tick interrupt if
   configUSE_TICK_HOOK is set to 1 in FreeRTOSConfig.h. User code can be
   added here, but the tick hook is called from an interrupt context, so
   code must not attempt to block, and only the interrupt safe FreeRTOS API
   functions can be used (those that end in FromISR()). */
}
/* USER CODE END 3 */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

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
  /* creation of defaultTask */
  defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void *argument)
{
  /* USER CODE BEGIN StartDefaultTask */
  /* Infinite loop */
//    lv_init();
//    lv_port_disp_init();
//    HAL_Delay(10);
//    lv_obj_t *label = lv_label_create(lv_scr_act());
//    lv_label_set_text(label,"niaho");
//    lv_obj_center(label);
//    lv_obj_t *switch_obj = lv_switch_create(lv_scr_act());
//
//    lv_obj_set_size(switch_obj, 120, 60);
//
//    lv_obj_align(switch_obj, LV_ALIGN_CENTER, 0, 0);
    HAL_Delay(2);
    LCD_WriteCommand(0xFF);
    LCD_WriteData(0x77);
    LCD_WriteData(0x01);
    LCD_WriteData(0x00);
    LCD_WriteData(0x00);
    LCD_WriteData(0x12);
    LCD_WriteCommand(0xD1);
    LCD_WriteData(0x81);

  for(;;)
  {
    //osDelay(1);
      HAL_GPIO_TogglePin(GPIOC,GPIO_PIN_13);
      LCD_WriteCommand(0xD2);
      LCD_WriteData(0x00);
      HAL_Delay(1000);
      LCD_WriteCommand(0xD2);
      LCD_WriteData(0x01);
      HAL_Delay(1000);
      LCD_WriteCommand(0xD2);
      LCD_WriteData(0x02);
      HAL_Delay(1000);
      LCD_WriteCommand(0xD2);
      LCD_WriteData(0x03);
      HAL_Delay(1000);
      LCD_WriteCommand(0xD2);
      LCD_WriteData(0x04);
      HAL_Delay(1000);
      LCD_WriteCommand(0xD2);
      LCD_WriteData(0x05);
      HAL_Delay(1000);
      LCD_WriteCommand(0xD2);
      LCD_WriteData(0x06);
      HAL_Delay(1000);
      LCD_WriteCommand(0xD2);
      LCD_WriteData(0x07);
      HAL_Delay(1000);
      LCD_WriteCommand(0xD2);
      LCD_WriteData(0x08);
      HAL_Delay(1000);
      LCD_WriteCommand(0xD2);
      LCD_WriteData(0x09);
      HAL_Delay(1000);
      LCD_WriteCommand(0xD2);
      LCD_WriteData(0x0A);
      HAL_Delay(1000);
      LCD_WriteCommand(0xD2);
      LCD_WriteData(0x0B);
      HAL_Delay(1000);
      //lv_timer_handler();
  }
  /* USER CODE END StartDefaultTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

