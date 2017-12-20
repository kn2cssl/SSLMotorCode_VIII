#include "main.h"
#include "Controller.hpp"
#include "stm32f0xx_hal.h"

ControllerVars Controller::phase[3];
signed short int Controller::SetedRPM;
signed short int Controller::momentomRPM;
float Controller::deltaTime;
const float Controller::timerPeriod = 0.000001;
unsigned char Controller::phaseSelector;

extern TIM_HandleTypeDef htim14;

void Controller::Calculate_Error()
{
	phase[phaseSelector].error = SetedRPM - momentomRPM;
	phase[phaseSelector].errorP = (float)SetedRPM * phase[phaseSelector].alpha - momentomRPM;
}
void Controller::Calculate_I()
{
	phase[phaseSelector].I += (phase[phaseSelector].Ki * phase[phaseSelector].error )* deltaTime;
	switch (phaseSelector)
	{
		case 0:
			phase[1].I = 0;
			phase[2].I = 0;
		break;
		case 1:
			phase[0].I = 0;
			phase[2].I = 0;			
		break;
		case 2:
			phase[0].I = 0;
			phase[1].I = 0;			
		break;
	};
}
void Controller::Calculate_PWM()
{
	phase[phaseSelector].PWM = phase[phaseSelector].Kp * phase[phaseSelector].errorP + phase[phaseSelector].I;
	if (phase[phaseSelector].PWM >= 200)
		phase[phaseSelector].PWM = 200;
	else if (phase[phaseSelector].PWM <= -200)
		phase[phaseSelector].PWM = -200;
}
void Controller::Set_MommentomRPM(signed short int RPM)
{
	momentomRPM = RPM;
}
void Controller::Init()
{
	HAL_TIM_Base_Start(&htim14);
	
	phase[0].Kp = 1.2;
	phase[0].Ki = 0.09;
	phase[0].alpha = 1;
	phase[0].I = 0;
	phase[0].PWM = 0;
	
	phase[1].Kp = 3.15;
	phase[1].Ki = 0.1;
	phase[1].alpha = 1;
	phase[1].I = 0;
	phase[1].PWM = 0;
	
	phase[2].Kp = 0.8;
	phase[2].Ki = 0.12;
	phase[2].alpha = 1;
	phase[2].I = 0;
	phase[2].PWM = 0;
	
	phaseSelector = 0;
	htim14.Instance -> CNT = 0;
}
void Controller::Set_RPM(signed short int RPM)
{
	if (RPM > 150 || RPM < - 200)
		phaseSelector = 1;
	else if (RPM > 40 || RPM < - 40)
		phaseSelector = 0;
	else 
		phaseSelector = 2;
	SetedRPM = RPM;	
}
signed short int Controller::Get_PWM()
{
	deltaTime = (htim14.Instance -> CNT) * timerPeriod;
	Calculate_Error();
	Calculate_I();
	Calculate_PWM();
	htim14.Instance -> CNT = 0;
	return phase[phaseSelector].PWM;
}
