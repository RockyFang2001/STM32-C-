#ifndef __BUTTON_H
#define __BUTTON_H
#include "headfile.h"
#define TRIG2_TIME 20
#define LONG_TRIG_TIME 100

#ifdef __cplusplus
extern "C"
{
#endif
    /***********************C++和C语言之间的转换接口*******************/
    void button_init(); // 按键，编码器GPIO初始化
    void button_isr();  // ok 键中断服务函数
    void encoder_isr(); // 编码器 cw 中断服务函数
/*******************************extern***************************/

/****************************************************************/
#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
/***********************C++文件之间的接口********************/
#include "timer.hpp"
using namespace std;
class button;
class button : public GPIO_CLASS
{
public:
    button(uint32_t GPIO_Port, uint8_t GPIO_Pin)
        : GPIO_CLASS(GPIO_Port, GPIO_Pin){};
    // 按键状态
    enum State
    {
        NONE = 0,   // 无状态
        TRIG,       // 按键已触发，但未确定状态
        LONG,       // 长按状态
        SHORT,      // 短按状态
        SHORT_TWICE // 双击状态
    };
    bool trig;                         // 该标志位显示按键的高低电平状态，Ti工程师吃屎大的，在定时器中断调用readpin会出现乱码
    State button_state;                // 按键状态
    void callback(uint32_t IntStatus); // 按键的中断服务函数
    void long_test();                  // 按键轮询
		
};
/****************************extern************************/
extern button ok;
extern button cw;
extern button ccw;
/**********************************************************/
#endif

#endif