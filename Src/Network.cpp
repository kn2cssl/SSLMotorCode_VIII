#include "Network.hpp"
#include "stm32f0xx_hal.h"

extern UART_HandleTypeDef huart1;

unsigned char Network::rxBuffer[4];
unsigned char Network::txBuffer[4];
signed short int Network::SRPM;
signed short int Network::RPM;
const unsigned char Network::START_BYTE = 0xAA;
const unsigned char Network::STOP_BYTE = 0x55;

void Network::Init()
{
	HAL_UART_Receive_IT(&huart1,rxBuffer,4);
}
void Network::Receive_Ended()
{
	if (rxBuffer[0] == START_BYTE && rxBuffer[3] == STOP_BYTE)
	{
		SRPM = rxBuffer[1]* 256 + rxBuffer[2];
		txBuffer[0] = START_BYTE;
		txBuffer[1] = ((unsigned short int)(RPM))/256;
		txBuffer[2] = ((unsigned short int)(RPM)) % 256;
		txBuffer[3] = STOP_BYTE;
		HAL_UART_Transmit_IT(&huart1,txBuffer,4);
	}
}
void Network::Transmit_Ended()
{
	HAL_UART_Receive_IT(&huart1,rxBuffer,4);
}
signed short int Network::Read_SRPM()
{
	return SRPM;
}
void Network::Set_RPM(signed short int _RPM)
{
	RPM = _RPM;
}

