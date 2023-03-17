/*
 * motor_master.c
 *
 *  Created on: Jun 20, 2022
 *      Author: Pascal
 */
#include "motor_master.h"

void initMotors(){
	initMotorX();
	initMotorZ();
}

uint8_t MoveToPos(int16_t posX, int16_t posZ){
	moveToPosX(posX);
	moveToPosZ(posZ);
	return 1;
}

uint8_t HomeMotors(uint8_t homeX, uint8_t homeZ){
	if(homeZ){
		homeMotorZ();
	}
	HAL_Delay(1000);
	if(homeX){
			homeMotorX();
		}
	return 1;
}
