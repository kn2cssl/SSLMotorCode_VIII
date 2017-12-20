#ifndef __MOTOR_HPP__
#define __MOTOR_HPP__

#include "stm32f0xx_hal.h"
#include <stdbool.h>

static class Motor
{
	private:
		static const double GEAR_RATIO;
		static bool clockwise[2][2][2][6];
		static bool antiClockwise[2][2][2][6];
		static bool hal1,hal2,hal3,currentState,lastState,state1,state2,isSpinn;
		static signed short int direction, momentomRPM,PWM;
		static double PPR1,PPR2,timerOverFlow1,timerOverFlow2;
		//(PPR = Pulse Per Rotation)
		void Set_DutyCycle(unsigned char duty,unsigned char channel);
		void Read_Hal(void);
		void State_Hal(bool checkSpeed);
		void Clockwise(void);
		void Anti_Clockwise(void);
		void Free_Wheel(void);
		void Show_Hal(void);
		void Set_Direction(void);
		void Check_Speed(void);
		void Init_Clockwise();
		void Init_Anti_Clockwise();
		void Start_Timers();
	public:
		void Init(bool _isSpinn = false);
		void Hal_Changed(void);
		void Time_Passed(bool fastTimer);
		void Calculate(void);
		void Set_PWM(signed int pwm);
		signed short int Get_MomentomRPM(void);
}motor;

#endif
