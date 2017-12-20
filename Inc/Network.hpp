#ifndef __NETWORK_HPP__
#define __NETWORK_HPP__

static class Network
{
	private:
		static unsigned char rxBuffer[4];
		static unsigned char txBuffer[4];
		static signed short int SRPM;
		static signed short int RPM;
		static const unsigned char START_BYTE;
		static const unsigned char STOP_BYTE;
	public:
		void Init();
		void Receive_Ended();
		void Transmit_Ended();
		signed short int Read_SRPM();
		void Set_RPM(signed short int _RPM);
}network;

#endif
