#include "Motor.hpp"
#include "main.h"
#include "stm32f0xx_hal.h"
#include <stdbool.h>

extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim16;
extern TIM_HandleTypeDef htim17;

const double Motor::GEAR_RATIO = 3.6;
//const double Motor::GEAR_RATIO = 3.8; //Old Version

bool Motor::clockwise[2][2][2][6];
bool Motor::antiClockwise[2][2][2][6];
bool Motor::hal1 = false,Motor::hal2 = false,Motor::hal3 = false,Motor::currentState = false,Motor::lastState = false,Motor::state1 = false,Motor::state2 = false,Motor::isSpinn = false;
signed short int Motor::direction = 1, Motor::momentomRPM = 0, Motor::PWM = 0;
double Motor::PPR1 = 0,Motor::PPR2 = 0,Motor::timerOverFlow1 = 0,Motor::timerOverFlow2 = 0;

void Motor::Set_DutyCycle(unsigned char duty,unsigned char channel)
{
	switch (channel)
	{
		case 0:
			htim3.Instance -> CCR3 = duty;
		break;
		case 1:
			htim3.Instance -> CCR4 = duty;
		break;
		case 2:
			htim1.Instance -> CCR1 = duty;
		break;
		case 3:
			htim1.Instance -> CCR2 = duty;
		break;
		case 4:
			htim1.Instance -> CCR3 = duty;
		break;
		case 5:
			htim1.Instance -> CCR4 = duty;
		break;
	};
}
void Motor::Read_Hal(void)
{
	hal3 = false;
	hal1 = false;
	hal2 = false;
	if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_6) == GPIO_PIN_SET)
		hal3 = true;
	if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_5) == GPIO_PIN_SET)
		hal1 = true;
	if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_7) == GPIO_PIN_SET)
		hal2 = true;
	currentState = hal2;
}
void Motor::State_Hal(bool checkSpeed)
{
	Show_Hal();
	if (!checkSpeed)
		return;
	Set_Direction();
	Check_Speed();
}
void Motor::Clockwise(void)
{
	unsigned short int uPWM = PWM;
	if (PWM < 0)
		uPWM = PWM * -1;
	Free_Wheel();

	if(clockwise[(unsigned char)hal1][(unsigned char)hal2][(unsigned char)hal3][0])
		Set_DutyCycle(uPWM,0);
	
	if(clockwise[(unsigned char)hal1][(unsigned char)hal2][(unsigned char)hal3][1])
		Set_DutyCycle(uPWM,1);
	
	if(clockwise[(unsigned char)hal1][(unsigned char)hal2][(unsigned char)hal3][2])
		Set_DutyCycle(uPWM,2);
	
	if(clockwise[(unsigned char)hal1][(unsigned char)hal2][(unsigned char)hal3][3])
		Set_DutyCycle(uPWM,3);
	
	if(clockwise[(unsigned char)hal1][(unsigned char)hal2][(unsigned char)hal3][4])
	  Set_DutyCycle(uPWM,4);
	
	if(clockwise[(unsigned char)hal1][(unsigned char)hal2][(unsigned char)hal3][5])
		Set_DutyCycle(uPWM,5);
}
void Motor::Anti_Clockwise(void)
{
	unsigned short int uPWM = PWM;
	if (PWM < 0)
		uPWM = PWM * -1;
	Free_Wheel();
	
	if(antiClockwise[(unsigned char)hal1][(unsigned char)hal2][(unsigned char)hal3][0])
		Set_DutyCycle(uPWM,0);
	
	if(antiClockwise[(unsigned char)hal1][(unsigned char)hal2][(unsigned char)hal3][1])
		Set_DutyCycle(uPWM,1);
	
	if(antiClockwise[(unsigned char)hal1][(unsigned char)hal2][(unsigned char)hal3][2])
		Set_DutyCycle(uPWM,2);
	
	if(antiClockwise[(unsigned char)hal1][(unsigned char)hal2][(unsigned char)hal3][3])
		Set_DutyCycle(uPWM,3);
	
	if(antiClockwise[(unsigned char)hal1][(unsigned char)hal2][(unsigned char)hal3][4])
	  Set_DutyCycle(uPWM,4);
	
	if(antiClockwise[(unsigned char)hal1][(unsigned char)hal2][(unsigned char)hal3][5])
		Set_DutyCycle(uPWM,5);
}
void Motor::Free_Wheel(void)
{
	Set_DutyCycle(0,0);
	Set_DutyCycle(0,1);
	Set_DutyCycle(0,2);
	Set_DutyCycle(0,3);
	Set_DutyCycle(0,4);
	Set_DutyCycle(0,5);
}
void Motor::Show_Hal(void)
{
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_5,(GPIO_PinState) hal2);
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_4,(GPIO_PinState) hal3);
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_3,(GPIO_PinState) hal1);
}
void Motor::Set_Direction(void)
{
	if (hal1 && !hal3)
	{
		if (hal2)
		{
			if (state2)
				direction = (-1);
			else
				state1 = true;
		}
		else
		{
			if (state1)
				direction = 1;
			else
				state2 = true;
		}
	}
	else 
	{
		state1 = false;
		state2 = false;
	}
}
void Motor::Check_Speed(void)
{
	if(!lastState && currentState)
  {	
		htim17.Instance  -> CNT = 0;
		timerOverFlow1 = 0;
		htim16.Instance -> CNT = 0;
		timerOverFlow2= 0;
  }
	else if(lastState && !currentState )
	{
		  double deltaTime1 = htim17.Instance  -> CNT;
		  double deltaTime2 = htim16.Instance -> CNT;
			PPR1 = (deltaTime1 + timerOverFlow1 * (double)65000.0) * (double)16.0;
			timerOverFlow1 = 0;
			PPR2 = (deltaTime2 + timerOverFlow2 * (double)65000.0) * (double)16.0;
			timerOverFlow2 = 0;
	}
  lastState = currentState;
}
void Motor::Init_Clockwise()
{
	clockwise[1][0][0][0] = false;
	clockwise[1][0][0][1] = false;
	clockwise[1][0][0][2] = true;
	clockwise[1][0][0][3] = false;
	clockwise[1][0][0][4] = false;
	clockwise[1][0][0][5] = true;
	
	
	clockwise[1][1][0][0] = false;
	clockwise[1][1][0][1] = true;
	clockwise[1][1][0][2] = true;
	clockwise[1][1][0][3] = false;
	clockwise[1][1][0][4] = false;
	clockwise[1][1][0][5] = false;
	
	
	clockwise[0][1][0][0] = false;
	clockwise[0][1][0][1] = true;
	clockwise[0][1][0][2] = false;
	clockwise[0][1][0][3] = false;
	clockwise[0][1][0][4] = true;
	clockwise[0][1][0][5] = false;
	
	
	clockwise[0][1][1][0] = false;
	clockwise[0][1][1][1] = false;
	clockwise[0][1][1][2] = false;
	clockwise[0][1][1][3] = true;
	clockwise[0][1][1][4] = true;
	clockwise[0][1][1][5] = false;
		
		
	clockwise[0][0][1][0] = true;
	clockwise[0][0][1][1] = false;
	clockwise[0][0][1][2] = false;
	clockwise[0][0][1][3] = true;
	clockwise[0][0][1][4] = false;
	clockwise[0][0][1][5] = false;
		
		
	clockwise[1][0][1][0] = true;
	clockwise[1][0][1][1] = false;
	clockwise[1][0][1][2] = false;
	clockwise[1][0][1][3] = false;
	clockwise[1][0][1][4] = false;
	clockwise[1][0][1][5] = true;
}
void Motor::Init_Anti_Clockwise()
{
	antiClockwise[1][0][0][0] = true;
	antiClockwise[1][0][0][1] = false;
	antiClockwise[1][0][0][2] = false;
	antiClockwise[1][0][0][3] = true;
	antiClockwise[1][0][0][4] = false;
	antiClockwise[1][0][0][5] = false;
		
		
	antiClockwise[1][1][0][0] = true;
	antiClockwise[1][1][0][1] = false;
	antiClockwise[1][1][0][2] = false;
	antiClockwise[1][1][0][3] = false;
	antiClockwise[1][1][0][4] = false;
	antiClockwise[1][1][0][5] = true;
	
		
	antiClockwise[0][1][0][0] = false;
	antiClockwise[0][1][0][1] = false;
	antiClockwise[0][1][0][2] = true;
	antiClockwise[0][1][0][3] = false;
	antiClockwise[0][1][0][4] = false;
	antiClockwise[0][1][0][5] = true;
		
		
	antiClockwise[0][1][1][0] = false;
	antiClockwise[0][1][1][1] = true;
	antiClockwise[0][1][1][2] = true;
	antiClockwise[0][1][1][3] = false;
	antiClockwise[0][1][1][4] = false;
	antiClockwise[0][1][1][5] = false;
		
		
	antiClockwise[0][0][1][0] = false;
	antiClockwise[0][0][1][1] = true;
	antiClockwise[0][0][1][2] = false;
	antiClockwise[0][0][1][3] = false;
	antiClockwise[0][0][1][4] = true;
	antiClockwise[0][0][1][5] = false;
		
		
	antiClockwise[1][0][1][0] = false;
	antiClockwise[1][0][1][1] = false;
	antiClockwise[1][0][1][2] = false;
	antiClockwise[1][0][1][3] = true;
	antiClockwise[1][0][1][4] = true;
	antiClockwise[1][0][1][5] = false;
}
void Motor::Start_Timers()
{
	HAL_TIM_Base_Start(&htim1);
	HAL_TIM_Base_Start(&htim3);
	HAL_TIM_Base_Start_IT(&htim16);
	HAL_TIM_Base_Start_IT(&htim17);
	HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_3);
	HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_4);
	HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_3);
	HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_4);
}
void Motor::Init(bool _isSpinn)
{
	isSpinn = _isSpinn;
	Init_Clockwise();
	Init_Anti_Clockwise();
	Read_Hal();
	State_Hal(!isSpinn);
	Start_Timers();
}
void Motor::Hal_Changed(void)
{
	Read_Hal();
	State_Hal(!isSpinn);
	if (PWM < 0 )
		Anti_Clockwise();
	else 
		Clockwise();
}
void Motor::Time_Passed(bool fastTimer)
{
	if(fastTimer)
	{
		if (timerOverFlow1 > 30)
		{
			timerOverFlow1 = 0;
			PPR1=0;
		}
		else
		{
			timerOverFlow1++;
		}			
  }
	else if(!fastTimer)
	{
		if (timerOverFlow2 > 10)
		{
			timerOverFlow2 = 0;
			PPR2=0;
		}
		else
		{
			timerOverFlow2++;
		}			
  }
}
void Motor::Calculate(void)
{
	double rotationTime1 = PPR1 * ((double)1/(double)1500000);
	double rotationTime2 = PPR2 * ((double)1/(double)500000);
	double momentomRPM1 = 0 , momentomRPM2 = 0;
	
	if (PPR1 != 0)
		momentomRPM1 = ((double)60 / rotationTime1) / GEAR_RATIO;
	else 
		momentomRPM1 = 0;
	if (PPR2 != 0)
		momentomRPM2 = ((double)60 / rotationTime2) / GEAR_RATIO;
	else 
		momentomRPM2 = 0;
	if (((momentomRPM1 + momentomRPM2) / (double)2) < 500)
		momentomRPM = momentomRPM2;
	else 
		momentomRPM = momentomRPM1;
	if (direction < 0)
		momentomRPM *= -1;
}
void Motor::Set_PWM(signed int pwm)
{
	PWM = pwm;
	Hal_Changed();
}
signed short int Motor::Get_MomentomRPM(void)
{
	return momentomRPM;
}
