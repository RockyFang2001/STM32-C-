#include "headfile.h" 
#include "i2c.hpp"
/**
 * 这是初始化 SCL 和 SDA 引脚和端口的 I2C 类的构造函数。
 *
 * @param SCL_Port 选择 I2C 时钟信号 (SCL) 的 GPIO 端口。
 * @param SCL_Pin  表示微控制器上 I2C 时钟线 (SCL) 的引脚号。
 * @param SDA_Port 选择 I2C 数据线 (SDA) 的 GPIO 端口。
 * @param SDA_Pin  连接到微控制器的 I2C 数据线 (SDA) 的引脚号。
 */
i2c_class::i2c_class(
	uint32_t SCL_Port, // SCL_Port
	uint8_t SCL_Pin,   // SCL_Pin
	uint32_t SDA_Port, // SDA_Port
	uint8_t SDA_Pin	   // SDA_Pin
)
{
	this->SCL_Port = SCL_Port;
	this->SCL_Pin = SCL_Pin;
	this->SDA_Port = SDA_Port;
	this->SDA_Pin = SDA_Pin;
};

void i2c_class::SCL_Set()
{
	GPIOPinWrite(SCL_Port, SCL_Pin, SCL_Pin);
}

void i2c_class::SDA_Set()
{
	GPIOPinWrite(SDA_Port, SDA_Pin, SDA_Pin);
}

void i2c_class::SCL_Clr()
{
	GPIOPinWrite(SCL_Port, SCL_Pin, 0);
}

void i2c_class::SDA_Clr()
{
	GPIOPinWrite(SDA_Port, SDA_Pin, 0);
}

/**********************************************
//IIC Start
**********************************************/
void i2c_class::IIC_Start()
{
	SCL_Set();
	SDA_Set();
	SDA_Clr();
	SCL_Clr();
}

/**********************************************
//IIC Stop
**********************************************/
void i2c_class::IIC_Stop()
{
	SCL_Set();
	SDA_Clr();
	SDA_Set();
}

/**********************************************
// IIC Wait Ack
**********************************************/
void i2c_class::IIC_Wait_Ack()
{

	SCL_Set();
	SCL_Clr();
}

/**********************************************
// IIC Write byte
**********************************************/

void i2c_class::Write_IIC_Byte(unsigned char IIC_Byte)
{
	unsigned char i;
	unsigned char m, da;
	da = IIC_Byte;
	SCL_Clr();
	for (i = 0; i < 8; i++)
	{
		m = da;
		m = m & 0x80;
		if (m == 0x80)
			SDA_Set();
		else
			SDA_Clr();
		da = da << 1;
		SCL_Set();
		SCL_Clr();
	}
}

/**********************************************
// IIC Write Command
**********************************************/
void i2c_class::Write_IIC_Command(unsigned char IIC_Command)
{
	IIC_Start();
	Write_IIC_Byte(IIC_SLAVE_ADDR); // Slave address,SA0=0
	IIC_Wait_Ack();
	Write_IIC_Byte(0x00); // write command
	IIC_Wait_Ack();
	Write_IIC_Byte(IIC_Command);
	IIC_Wait_Ack();
	IIC_Stop();
}

/**********************************************
// IIC Write Data
**********************************************/
void i2c_class::Write_IIC_Data(unsigned char IIC_Data)
{
	IIC_Start();
	Write_IIC_Byte(IIC_SLAVE_ADDR); // D/C#=0; R/W#=0
	IIC_Wait_Ack();
	Write_IIC_Byte(0x40); // write data
	IIC_Wait_Ack();
	Write_IIC_Byte(IIC_Data);
	IIC_Wait_Ack();
	IIC_Stop();
}

void i2c_class::WR_Byte(unsigned dat, unsigned cmd)
{
	if (cmd)
	{
		Write_IIC_Data(dat);
	}
	else
	{
		Write_IIC_Command(dat);
	}
}
