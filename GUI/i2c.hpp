#ifndef __I2C_H
#define __I2C_H
#include "headfile.h"


#ifdef __cplusplus
extern "C"
{
#endif
/***********************C++和C语言之间的转换接口*******************/

/*******************************extern***************************/

/****************************************************************/
#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
/***********************C++文件之间的接口********************/
class i2c_class
{
private:
	void SCL_Set();
	void SDA_Set();
	void SDA_Clr();
	void SCL_Clr();
	void IIC_Start(void);
	void IIC_Stop(void);
	void IIC_Wait_Ack(void);
	void Write_IIC_Command(unsigned char IIC_Command);
	void Write_IIC_Data(unsigned char IIC_Data);
	void Write_IIC_Byte(unsigned char IIC_Byte);

public:
	uint32_t SCL_Port;
	uint8_t SCL_Pin;
	uint32_t SDA_Port;
	uint8_t SDA_Pin;
	void WR_Byte(unsigned dat, unsigned cmd);
	i2c_class(uint32_t SCL_Port, uint8_t SCL_Pin, uint32_t SDA_Port, uint8_t SDA_Pin);
};
/****************************extern************************/

/**********************************************************/
#endif

#endif