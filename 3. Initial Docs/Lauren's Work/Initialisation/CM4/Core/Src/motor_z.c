/*
 * motorZ.c
 *
 *  Created on: Jun 20, 2022
 *      Author: Pascal
 */
#include "motor_z.h"
#include "usart.h"
#include "stdio.h"

int32_t counterZ;
double position_mm_Z;

uint8_t i_Z = 0;
uint8_t once_Z = 1;

void initMotorZ(){
	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_4);
	HAL_TIM_Encoder_Start_IT(&htim4, TIM_CHANNEL_ALL);
}

uint8_t homeMotorZ(){
	__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_4, 125);
	HAL_GPIO_WritePin(Direction_Z_GPIO_Port, Direction_Z_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(Ready_Z_GPIO_Port, Ready_Z_Pin, GPIO_PIN_SET);

	while(!get_Homing_Z()){

	}
	__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_4, 200);
//	HAL_GPIO_WritePin(Ready_Z_GPIO_Port, Ready_Z_Pin, GPIO_PIN_RESET);
	HAL_Delay(1000);
	position_mm_Z = 0.0;
	counterZ = 0;
	i_Z = 0;
	__HAL_TIM_SET_COUNTER(&htim4, 0);
	HAL_Delay(1000);
	moveToPosZ(30);
	return 1;
}

uint8_t moveToPosZ(double Zpos){
	counterZ = __HAL_TIM_GET_COUNTER(&htim4);
	position_mm_Z = (double) ((counterZ / 3855)+(i_Z*17));
	double delta = Zpos - position_mm_Z;
	while(abs(delta)>2){
		if(delta > 0){
			__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_4, 360);
			HAL_GPIO_WritePin(Direction_Z_GPIO_Port, Direction_Z_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(Ready_Z_GPIO_Port, Ready_Z_Pin, GPIO_PIN_SET);
		}
		else if(delta < 0){
			__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_4, 125);
			HAL_GPIO_WritePin(Direction_Z_GPIO_Port, Direction_Z_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(Ready_Z_GPIO_Port, Ready_Z_Pin, GPIO_PIN_SET);
		}else{
			break;
		}
		counterZ = __HAL_TIM_GET_COUNTER(&htim4);
		if(counterZ > 61680 && delta>0 && once_Z){
			once_Z = 0;
		}else if (counterZ < 3855 && delta>0 && !once_Z){
			once_Z = 1;
			i_Z += 1;
		}else if (counterZ < 3855 && delta<0){
			once_Z = 0;
		}else if (counterZ > 61680 && delta<0 && !once_Z){
			once_Z = 1;
			i_Z -= 1;
		}
		position_mm_Z = (double) ((counterZ / 3855.0)+(i_Z*17));
		delta = Zpos - position_mm_Z;
		uint8_t send[30];
		sprintf(send, "%f \r\n %u \r\n", position_mm_Z, i_Z);
		ST_LINK_WRITE(send, sizeof(send));
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
		//			__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_4, y);
		//			i = 0;
		//		}
	}
	HAL_GPIO_WritePin(Direction_Z_GPIO_Port, Direction_Z_Pin, GPIO_PIN_RESET);
	__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_4, 200);
//	HAL_GPIO_WritePin(Ready_Z_GPIO_Port, Ready_Z_Pin, GPIO_PIN_RESET);
	return 1;
}

//void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim){
//	if(htim == &htim4){
//		counterZ = __HAL_TIM_GET_COUNTER(htim);
//
//		position_mm_Z = (int32_t) counterZ / 1104;
//	}
//}
//
//void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
//{
//	if(GPIO_Pin == Homing_Z_Pin){
//		if(HAL_GPIO_ReadPin(Homing_Z_GPIO_Port, Homing_Z_Pin)){
//			/* do something */
//			__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_4, 0);
//			HAL_GPIO_WritePin(Ready_Z_GPIO_Port, Ready_Z_Pin, GPIO_PIN_RESET);
//			homeDone = 1;
//		}
//		if(!HAL_GPIO_ReadPin(Homing_Z_GPIO_Port, Homing_Z_Pin)){ //check pin state
//			/* do something */
//			HAL_GPIO_WritePin(Ready_Z_GPIO_Port, Ready_Z_Pin, GPIO_PIN_RESET);
//			__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_4, 240);
//			HAL_GPIO_WritePin(Direction_Z_GPIO_Port, Direction_Z_Pin, GPIO_PIN_RESET);
//			HAL_GPIO_WritePin(Ready_Z_GPIO_Port, Ready_Z_Pin, GPIO_PIN_SET);
//		}
//	}
//}
