/*
 * motorZ.h
 *
 *  Created on: Jun 20, 2022
 *      Author: Pascal
 */

#ifndef INC_MOTOR_Z_H_
#define INC_MOTOR_Z_H_

#include "gpio.h"
#include "tim.h"
#include "motor_master.h"

void initMotorZ();

uint8_t homeMotorZ();

uint8_t moveToPosZ(double Zpos);

//void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim);
//
//void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin);

#endif /* INC_MOTOR_Z_H_ */
