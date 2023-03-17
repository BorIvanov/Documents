/*
 * motorX.c
 *
 *  Created on: Jun 20, 2022
 *      Author: Pascal
 */
#include "motor_x.h"
#include "usart.h"
#include "stdio.h"

int32_t counterX;
double position_mm_X;

int8_t i_X = 0;
uint8_t once_X = 1;

void initMotorX(){
	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_3);
	HAL_TIM_Encoder_Start_IT(&htim3, TIM_CHANNEL_ALL);
}

uint8_t homeMotorX(){
	__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_3, 140);
	HAL_GPIO_WritePin(Direction_X_GPIO_Port, Direction_X_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(Ready_X_GPIO_Port, Ready_X_Pin, GPIO_PIN_SET);

	while(!get_Homing_X()){

	}
	__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_3, 0);
	HAL_GPIO_WritePin(Ready_X_GPIO_Port, Ready_X_Pin, GPIO_PIN_RESET);
	HAL_Delay(200);
	position_mm_X = 0.0;
	counterX = 0;
	i_X = 0;
	__HAL_TIM_SET_COUNTER(&htim3, 0);
	moveToPosX(10);
	return 1;
}

uint8_t moveToPosX(double Xpos){
	counterX = __HAL_TIM_GET_COUNTER(&htim3);
	position_mm_X = (double) ((counterX / 3855.0)+(i_X*17));
	double delta = Xpos - position_mm_X;
	__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_3, 140);
	HAL_GPIO_WritePin(Ready_X_GPIO_Port, Ready_X_Pin, GPIO_PIN_SET);
	while(abs(delta)>2){
		if(delta > 0){
			HAL_GPIO_WritePin(Direction_X_GPIO_Port, Direction_X_Pin, GPIO_PIN_RESET);
		}
		else if(delta < 0){
			HAL_GPIO_WritePin(Direction_X_GPIO_Port, Direction_X_Pin, GPIO_PIN_SET);
		}else{
			break;
		}
		counterX = __HAL_TIM_GET_COUNTER(&htim3);
		if(counterX > 61680 && delta>0 && once_X){
			once_X = 0;
		}else if (counterX < 3855 && delta>0 && !once_X){
			once_X = 1;
			i_X += 1;
		}else if (counterX < 3855 && delta<0){
			once_X = 0;
		}else if (counterX > 61680 && delta<0 && !once_X){
			once_X = 1;
			i_X -= 1;
		}
		position_mm_X = (double) ((counterX / 3855)+(i_X*17));
		delta = Xpos - position_mm_X;
//		uint8_t send[30];
//		sprintf(send, "%ld \r\n %d \r\n", position_mm_X, i_X);
//		ST_LINK_WRITE(send, sizeof(send));
		HAL_Delay(50);

		//		if(abs(delta) < 60){
		//			y = 2*(pow(abs(delta), 0.49)) + 215;
		//			if(y>250){
		//				y = 250;
		//			}
		//		} else {
		//			y = 250;
		//		}
		//
		//		i++;
		//		if(i == 200){
		//			__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_3, y);
		//			i = 0;
		//		}
	}
	__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_3, 0);
	HAL_GPIO_WritePin(Ready_X_GPIO_Port, Ready_X_Pin, GPIO_PIN_RESET);
	return 1;
}

//void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim){
//	if(htim == &htim3){
//		counterX = __HAL_TIM_GET_COUNTER(htim);
//
//		position_mm_X = (int32_t) counterX / 1104;
//	}
//}
//
//void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
//{
//	if(GPIO_Pin == Homing_X_Pin){
//		if(HAL_GPIO_ReadPin(Homing_X_GPIO_Port, Homing_X_Pin)){
//			/* do something */
//			__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_3, 0);
//			HAL_GPIO_WritePin(Ready_X_GPIO_Port, Ready_X_Pin, GPIO_PIN_RESET);
//			homeDone = 1;
//		}
//		if(!HAL_GPIO_ReadPin(Homing_X_GPIO_Port, Homing_X_Pin)){ //check pin state
//			/* do something */
//			HAL_GPIO_WritePin(Ready_X_GPIO_Port, Ready_X_Pin, GPIO_PIN_RESET);
//			__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_3, 240);
//			HAL_GPIO_WritePin(Direction_X_GPIO_Port, Direction_X_Pin, GPIO_PIN_RESET);
//			HAL_GPIO_WritePin(Ready_X_GPIO_Port, Ready_X_Pin, GPIO_PIN_SET);
//		}
//	}
//}
