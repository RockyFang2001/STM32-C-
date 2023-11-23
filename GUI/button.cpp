#include "headfile.h"
#include "button.hpp"

button ccw(GPIO_PORTB_BASE, GPIO_PIN_5);
button cw(GPIO_PORTD_BASE, GPIO_PIN_1);
button ok(GPIO_PORTA_BASE, GPIO_PIN_2);

/**
 * 函数“button_event”处理按钮按下、双击、长按等不同的按钮事件，并根据当前状态和按钮状态执行相应的操作。
 */
void menu_class::button_event()
{
    // 按键轮询
    ok.long_test();

    if (state == menu_class::UP)
    {
        /*************编码器正转*************/
        if (paraStatus)
        {
            if (paraselect >= 0)
                paraselect--;
            if (paraselect < 0)
                paraselect = paralist_size - 1;
        }
        else
            mpara_list[paraselect].value -= mpara_list[paraselect].precision;
        state = menu_class::NONE;
        /***********************************/
    }

    if (state == menu_class::DOWN)
    {
        /*************编码器反转*************/
        if (paraStatus)
        {
            if (paraselect <= paralist_size - 1)
                paraselect++;
            if (paraselect > paralist_size - 1)
                paraselect = 0;
        }
        else
            mpara_list[paraselect].value += mpara_list[paraselect].precision;
        state = menu_class::NONE;
        /***********************************/
    }

    if (ok.button_state == button::SHORT)
    {
        /*************按键短按*************/
        paraStatus ^= 1;
        ok.button_state = button::NONE;
        /*********************************/
    }
}

/**
 * 该函数初始化按钮和编码器以在 C++ 程序中进行中断处理。
 */
void button_init()
{
    /****按键触发****/
    // 关闭中断
    GPIOIntDisable(ok.get_port(), ok.get_pin());
    // 设置为输入模式
    GPIOPinTypeGPIOInput(ok.get_port(), ok.get_pin());
    // 双边沿中断触发
    GPIOIntTypeSet(ok.get_port(), ok.get_pin(), GPIO_BOTH_EDGES);
    // 中断使能
    GPIOIntEnable(ok.get_port(), ok.get_pin());

    /****编码器计数****/
    // 关闭中断
    GPIOIntDisable(cw.get_port(), cw.get_pin());
    // 设置为输入模式
    GPIOPinTypeGPIOInput(cw.get_port(), cw.get_pin());
    // 上升沿中断触发
    GPIOIntTypeSet(cw.get_port(), cw.get_pin(), GPIO_RISING_EDGE);
    // 中断使能
    GPIOIntEnable(cw.get_port(), cw.get_pin());

    /****编码器鉴相****/
    GPIOPinTypeGPIOInput(ccw.get_port(), ccw.get_pin());

    // 中断使能
    IntEnable(INT_GPIOD);
    IntEnable(INT_GPIOA);
    
    // 链接中断服务函数
    GPIOIntRegister(ok.get_port(), button_isr);
    GPIOIntRegister(cw.get_port(), encoder_isr);
}

/**
 * 函数button_isr是一个中断服务例程，它处理按钮按下并以中断状态作为参数调用回调函数。
 */
void button_isr()
{
    // 按键触发中断服务函数
    uint32_t getIntStatus;
    getIntStatus = GPIOIntStatus(ok.get_port(), true);
    GPIOIntClear(ok.get_port(), getIntStatus);
    ok.callback(getIntStatus);
}

/**
 * encoder_isr 函数是一个中断服务例程，用于处理编码器的计数并确定旋转方向。
 */
void encoder_isr()
{
    // 编码器计数中断服务函数
    uint32_t getIntStatus;
    getIntStatus = GPIOIntStatus(cw.get_port(), true);
    GPIOIntClear(cw.get_port(), getIntStatus);
    if (getIntStatus & cw.get_pin())
    {
        // 正转
        if (ccw.is_set())
            menu.state = menu_class::UP;
        // 反转
        else
            menu.state = menu_class::DOWN;
    }
}

/**
 * 回调函数检查按钮的状态并根据按钮信号的上升沿和下降沿更新其状态。
 *
 * @param IntStatus 参数IntStatus是一个无符号32位整数，表示中断状态。它用于确定按钮是否被按下。
 */
void button::callback(uint32_t IntStatus)
{

    if (IntStatus & get_pin())
    {
        // 按键状态标志位转换
        // 检测下降沿，按键被按下时刻
        if (is_reset())
        {
            // 下降沿触发时间
            rise_T = T;
            // 按键处于低电平
            trig = true;
            // 按键被触发，但未确定状态
            button_state = TRIG;
        }
        else
        {
            // 上升降沿触发时间
            fall_T = T;
            // 按键处于高电平
            trig = false;
        }

        // 检测双击，判据为两次按键时间小于TRIG2_TIME，调节该参数可以调节按键灵敏度
        if (T - fall_T <= TRIG2_TIME && is_reset() && button_state == TRIG)
        {
            button_state = SHORT_TWICE;
        }
    }
}

/**
 * 函数“long_test”检查按钮的状态，并根据按钮按下的持续时间确定是短按还是长按。
 */
void button::long_test()
{
    // 按键状态轮询

    // 检测短按，判据当下降沿一定时间后，一直保持高电平，没有再被触发
    if (T - fall_T > TRIG2_TIME && button_state == TRIG && !trig)
    {
        button_state = SHORT;
    }

    // 检测长按，触发一定时间后，一直保持高电平
    if (T - rise_T > LONG_TRIG_TIME && button_state == TRIG && trig)
    {
        button_state = LONG;
    }
}
