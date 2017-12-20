#ifndef __CONTROLLER_HPP__
#define __CONTROLLER_HPP__

struct ControllerVars
{
	float I;
	float Kp;
	float Ki;
	float alpha;
	float error;
	float errorP;
	signed short int PWM;
};

static class Controller
{
	private:
		static ControllerVars phase[3];
		static unsigned char phaseSelector;
		static signed short int SetedRPM;
		static signed short int momentomRPM;
		static float deltaTime;
		static const float timerPeriod;
		void Calculate_Error();
		void Calculate_I();
		void Calculate_PWM();
	public:
		void Init();
		void Set_MommentomRPM(signed short int RPM);
		void Set_RPM(signed short int RPM);
		signed short int Get_PWM();
}controller;

#endif 
