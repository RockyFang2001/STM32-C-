#include "headfile.h"
#include "menu.hpp"

using namespace std;

/**
 * 函数“OLED_WR_Byte”用于使用硬件 I2C 通信将一个字节的数据或命令写入 OLED 显示屏。
 * 使用外设为：
 *              I2C1_BASE
 *              GPIO_PA7_I2C1SDA
 *              GPIO_PA6_I2C1SCL
 *
 * @param dat “dat”参数是要写入 OLED 显示屏的数据字节。它可以是 0x00 到 0xFF 之间的任何值。
 * @param cmd “cmd”参数是指示正在写入的数据是命令还是数据的标志。如果“cmd”非零，则表示数据是命令。如果“cmd”为零，则表示该数据是实际显示数据。
 */
void OLED_WR_Byte(unsigned dat, unsigned cmd)
{
    // 设置oled硬件的地址
    I2CMasterSlaveAddrSet(I2C1_BASE, IIC_SLAVE_ADDR >> 1, false);

    // 等待ack信号
    while (I2CMasterBusy(I2C1_BASE))
        ;

    if (cmd)
        I2CMasterDataPut(I2C1_BASE, 0x40); //  写数据
    else
        I2CMasterDataPut(I2C1_BASE, 0x00); // 写指令

    // 发送start信号
    I2CMasterControl(I2C1_BASE, I2C_MASTER_CMD_BURST_SEND_START);

    // 等待ack信号
    while (I2CMasterBusy(I2C1_BASE))
        ;

    //  写一个byte数据
    I2CMasterDataPut(I2C1_BASE, dat);

    // 等待ack信号
    while (I2CMasterBusy(I2C1_BASE))
        ;

    // 发送终止信号
    I2CMasterControl(I2C1_BASE, I2C_MASTER_CMD_BURST_SEND_FINISH);
}

/**
 * 函数 `menu_init()` 初始化菜单类的时钟、I2C、OLED 显示屏、按钮和编码器。
 */
void menu_class::menu_init()
{

    // 时钟使能
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA); // 使能oled iic 和 编码器 ok 键 引脚所在的端口
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB); // 使能编码器 ccw 相引脚所在的端口
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD); // 使能编码器 cw  相引脚所在的端口
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_I2C1);  // 使能硬件 iic 外设

    //等待外设使能完成
    while ( (SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOA) == false) );
    while ( (SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOB) == false) );
    while ( (SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOD) == false) );
    while ( (SysCtlPeripheralReady(SYSCTL_PERIPH_I2C1) == false) );

    // I2C初始化
    GPIOPinConfigure(GPIO_PA7_I2C1SDA);                     // 设置PA7引脚功能为 I2C1 SDA
    GPIOPinTypeI2C(GPIO_PORTA_BASE, GPIO_PIN_7);            // 设置PA7提供给 I2C1_BASE 外设使用
    GPIOPinConfigure(GPIO_PA6_I2C1SCL);                     // 设置PA6引脚功能为 I2C1 SCL
    GPIOPinTypeI2CSCL(GPIO_PORTA_BASE, GPIO_PIN_6);         // 设置PA6提供给 I2C1_BASE 外设使用
    I2CMasterInitExpClk(I2C1_BASE, SysCtlClockGet(), true); // 设置 iic 主机时钟为单片机主频

    // OLED初始化
    OLED_Init();
    OLED_Clear(0);

    // 按键和编码器初始化
    button_init();
}

/**
 * `menu_class` 构造函数初始化菜单的参数和面板，计算不同类型参数的长度，并确定菜单的最大页数。
 *
 * @param para_list 包含可调整参数的结构。
 * @param panel 指向 PANEL 结构数组的指针，其中每个结构代表要在菜单中显示的一个参数。每个 PANEL 结构都包含参数名称、大小和长度等信息。
 */
menu_class::menu_class(PARAM *para_list, PANEL *panel)
{
    // 传入可调参数列表结构体
    mpara_list = para_list;
    // 传入显示参数列表结构体
    mpanel = panel;
    // 初始化ui参数
    paralist_size = 0;
    panel_size = 0;
    paraselect = 0;
    short_size = 0;
    long_size = 0;
    huge_size = 0;
    cross_size = 0;
    panel_line = PANEL_LINE;
    paraStatus = true;

    // 得到可调参数列表的长度
    while (this->mpara_list[++paralist_size].precision != 0)
        ;

    // 得到显示参数列表的长度，同时遍历全部参数，做参数列表的动态规划
    // 密集显示的条件，是LONG或者SHORT长度的参数，同时参数数字长度小于3
    while (this->mpanel[panel_size].len != 0)
    {
        // 计算名称长度为SHORT，且显示数字长度小于等于3的参数数量，并且压入SHORT参数位置堆栈
        if (this->mpanel[panel_size].size <= SHORT_LEN && this->mpanel[panel_size].len <= 3)
        {
            short_str[short_size] = panel_size;
            short_size++;
            uint8_t count = strlen(this->mpanel[panel_size].name);
            while (count < SHORT_LEN)
            {
                strcat(this->mpanel[panel_size].name, " ");
                count++;
            }
            strcat(this->mpanel[panel_size].name, ":");
        }
        // 计算名称长度为LONG，且显示数字长度小于等于3的参数数量，并且压入LONG参数位置堆栈
        else if (this->mpanel[panel_size].size > SHORT_LEN && this->mpanel[panel_size].size <= LONG_LEN && this->mpanel[panel_size].len <= 3)
        {
            long_str[long_size] = panel_size;
            long_size++;
            uint8_t count = strlen(this->mpanel[panel_size].name);
            while (count < LONG_LEN)
            {
                strcat(this->mpanel[panel_size].name, " ");
                count++;
            }
            strcat(this->mpanel[panel_size].name, ":");
        }
        // 其他情况归为超长参数
        else
        {
            huge_str[huge_size] = panel_size;
            huge_size++;
            strcat(this->mpanel[panel_size].name, ":");
        }
        panel_size++;
    }
    // 计算LONG和SHORT长度参数显示在一行的行数
    cross_size = fmin(long_size, short_size);

    // 考虑重合行数为奇数
    if ((short_size - cross_size) % 2 != 0 && cross_size != 0)
        cross_size--;

    // 考虑只有SHORT和HUGE长度的显示参数
    if (long_size == 0 && short_size != 0 && short_size % 2 != 0)
        max_page = ((short_size + 1) / 2 + long_size + huge_size - 1) / panel_line;
    else
        max_page = ((short_size - cross_size) / 2 + long_size + huge_size - 1) / panel_line;
}

/**
 * “menu_class”类中的“Printf”函数接收格式字符串和变量参数，使用“vsnprintf”将它们格式化为字符串，然后在 OLED 显示屏上的指定坐标处显示结果字符串。
 *
 * @param x 文本在 OLED 显示屏上打印的起始位置的 x 坐标。
 * @param y 函数“Printf”中的参数“y”表示OLED显示屏上文本的垂直位置。它指定将打印文本的行号。
 * @param fmt fmt 参数是一个格式字符串，指定输出字符串的格式。它可以包含由后续参数中指定的值替换的格式说明符。格式说明符以“%”字符开头，后跟可选标志、宽度、精度和长度修饰符，以及
 */
void menu_class::Printf(uint8_t x, uint8_t y, const char *fmt, ...)
{
    char printf_str[21];
    va_list ap;
    va_start(ap, fmt);
    vsnprintf(printf_str, 21, fmt, ap);
    OLED_P6x8Str(x, y, (uint8_t *)printf_str);
}

/**
 * `menu_class` 类中的函数 `PrintfW` 用于在 OLED 显示屏上的指定位置打印格式化字符串。
 *
 * @param x x 参数是显示屏上打印文本的起始列位置。
 * @param y 参数“y”表示显示器上文本的垂直位置。它确定将打印文本的行。
 * @param fmt fmt
 * 参数是一个格式字符串，指定输出字符串的格式。它可以包含格式说明符，这些格式说明符将被格式字符串后面传递的相应参数替换。格式说明符以“%”字符开头，后跟可选标志、宽度、精度和长度修饰符，
 */
void menu_class::PrintfW(uint8_t x, uint8_t y, const char *fmt, ...)
{
    char printf_str[21];
    va_list ap;
    va_start(ap, fmt);
    vsnprintf(printf_str, 21, fmt, ap);
    OLED_P6x8StrW(x, y, (uint8_t *)printf_str);
}

/**
 * 函数“type_convert”将“PANEL”指针作为输入，并根据“PANEL”的类型返回转换后的值。
 *
 * @param pan 参数“pan”是指向“PANEL”对象的指针。
 *
 * @return 给定 PANEL 对象的转换值。使用 typeid 运算符检查 PANEL 对象的类型，并根据类型转换并返回相应的值。如果类型不是指定类型之一，则函数返回 0。
 */
int menu_class::type_convert(PANEL *pan)
{
    if ((*pan).type == typeid(int *))
        return *(int *)(*pan).value;

    if ((*pan).type == typeid(float *))
        return (*(float *)(*pan).value);

    if ((*pan).type == typeid(uint32_t *))
        return (*(uint32_t *)(*pan).value);

    if ((*pan).type == typeid(uint16_t *))
        return (*(uint16_t *)(*pan).value);

    if ((*pan).type == typeid(uint8_t *))
        return (*(uint8_t *)(*pan).value);

    if ((*pan).type == typeid(int16_t *))
        return (*(int16_t *)(*pan).value);

    if ((*pan).type == typeid(int8_t *))
        return (*(int8_t *)(*pan).value);

    if ((*pan).type == typeid(double *))
        return (*(double *)(*pan).value);

    return 0;
}

/**
 * 函数“show_line”在菜单的特定行上显示菜单项的名称和值。
 *
 * @param y 参数“y”表示该行将在屏幕或控制台上显示的垂直位置或行号。
 * @param index show_line 函数中的index 参数表示要显示的菜单项的索引。它用于访问`panel`数组中相应的菜单项。
 */
void menu_class::show_line(uint8_t y, uint8_t index)
{
    uint8_t count = 0;
    if (mpanel[index].size <= SHORT_LEN && mpanel[index].len <= 3)
    {
        while (short_str[count] != index) // 找到每个index在short_str中对应下标
            count++;
        if (count < short_size - cross_size) // 靠前的short松散地显示
        {
            if (count % 2 == 0) // 偶数下标显示在左边
            {
                PrintfW(0, y, mpanel[index].name);
                Printf((SHORT_LEN + 1) * 6, y, "%5d", type_convert(&mpanel[index]));
            }
            else // 奇数下标显示在右边
            {
                PrintfW(66, y, mpanel[index].name);
                Printf(66 + (SHORT_LEN + 1) * 6, y, "%5d", type_convert(&mpanel[index]));
            }
        }
        else // 靠后的short与long在同一行显示
        {
            PrintfW(0, y, mpanel[index].name);
            Printf((SHORT_LEN + 1) * 6, y, "%3d", type_convert(&mpanel[index]));
        }
    }
    else if (mpanel[index].size > SHORT_LEN && mpanel[index].size <= LONG_LEN && mpanel[index].len <= 3)
    {
        while (long_str[count] != index) // 找到每个index在long_str中对应下标
            count++;
        if (count < cross_size) // 靠前的long与short在同一行显示
        {
            PrintfW((SHORT_LEN + 5) * 6, y, mpanel[index].name);
            Printf((SHORT_LEN + LONG_LEN + 6) * 6, y, "%3d", type_convert(&mpanel[index]));
        }
        else // 靠后的long单独显示
        {
            PrintfW(0, y, mpanel[index].name);
            Printf((LONG_LEN + 1) * 6, y, "%5d", type_convert(&mpanel[index]));
        }
    }
    else // huge单独显示
    {
        PrintfW(0, y, mpanel[index].name);
        Printf((mpanel[index].size + 1) * 6, y, "%8d", type_convert(&mpanel[index]));
    }
}

/**
 * 函数“show_page()”以特定格式在 OLED 屏幕上显示不同类型的字符串（短、长、大）。
 */
void menu_class::show_page()
{
    // 密集显示SHORT参数
    uint8_t count = 0;
    for (; count < short_size - cross_size; count++)
    {
        if ((count / 2) / panel_line == showPage)
            show_line((int)(count / 2) % panel_line, (int)short_str[count]);
    }

    // 密集显示SHORT和LONG交叠区域
    uint8_t line = count / 2;
    for (; count < short_size; count++, line++)
    {
        if (line / panel_line == showPage)
        {
            show_line((int)line % panel_line, (int)short_str[count]);
            show_line(line % panel_line, long_str[count - (short_size - cross_size)]);
        }
    }

    // 显示单独一行的LONG区域
    for (uint8_t i = cross_size; i < long_size; i++, line++)
    {
        if (line / panel_line == showPage)
            show_line(line % panel_line, long_str[i]);
    }

    // 当SHORT独立一行时，跳过一行
    if (long_size == 0 && short_size != 0 && short_size % 2 != 0)
        line++;

    // 显示单独一行的HUGE区域
    for (uint8_t i = 0; i < huge_size; i++, line++)
    {
        if (line / panel_line == showPage)
            show_line(line % panel_line, huge_str[i]);
    }

    // 清屏标志位，协同刷新和调参
    if (all_clear)
    {
        OLED_Clear(0);
        all_clear = false;
    }
}

/**
 * 此功能在 OLED 屏幕上显示参数名称和值，并允许用户在满足特定条件时减小值。
 */
void menu_class::show_para()
{
    if (paraselect <= paralist_size - 1 && paraselect >= 0)
    {
        if (paraselect == paralist_size - 1)
        {
            OLED_ClearLine(7);
        }

        if (paraselect <= paralist_size - 2)
        {
            Printf(0, 7, mpara_list[paraselect + 1].name);
            Printf(96, 7, "%5d", mpara_list[paraselect + 1].value);
        }

        if (paraselect == 0)
        {
            OLED_ClearLine(5);
        }

        if (paraselect >= 1)
        {
            Printf(0, 5, mpara_list[paraselect - 1].name);
            Printf(96, 5, "%5d", mpara_list[paraselect - 1].value);
        }

        if (paraStatus)
        {
            PrintfW(0, 6, mpara_list[paraselect].name);
            Printf(96, 6, "%5d", mpara_list[paraselect].value);
        }
        else
        {
            Printf(0, 6, mpara_list[paraselect].name);
            PrintfW(96, 6, "%5d", mpara_list[paraselect].value);
        }
    }
}
