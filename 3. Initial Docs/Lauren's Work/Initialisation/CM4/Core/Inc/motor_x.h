/*
 * motorX.h
 *
 *  Created on: Jun 20, 2022
 *      Author: Pascal
 */

#ifndef INC_MOTOR_X_H_
#define INC_MOTOR_X_H_

#include "gpio.h"
#include "tim.h"
#include "motor_master.h"

void initMotorX();

uint8_t homeMotorX();

uint8_t moveToPosX(double Xpos);

//void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim);
//
//void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin);

#endif /* INC_MOTOR_X_H_ */
