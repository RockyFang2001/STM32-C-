#ifndef __MENU_H
#define __MENU_H
#include "headfile.h"
#define LONG_LEN 8
#define SHORT_LEN 4
#define PANEL_LINE 5
#define IIC_SLAVE_ADDR 0x78 // OLED地址

#ifdef __cplusplus
extern "C"
{
#endif
    /***********************C++和C语言之间的转换接口*******************/
    void OLED_WR_Byte(unsigned dat, unsigned cmd);
/*******************************extern***************************/

/****************************************************************/
#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
/***********************C++文件之间的接口********************/
#include "setpara.hpp"
#include "gpio.hpp"
using namespace std;

class menu_class
{
private:
    PARAM *mpara_list;     // 传入的可调参数列表
    PANEL *mpanel;         // 传入的显示参数列表
    uint8_t max_page;      // 显示参数列表的最大页数
    int8_t paraselect;    // 用户选择的可调参数的索引
    uint8_t showPage;      // 显示参数列表显示的页数
    bool paraStatus;       // 1 选择可调参数
                           // 0 调节可调参数的大小
    bool all_clear;        //  清除屏幕的标志位
    uint8_t short_str[20]; // 用于记录短名称参数的索引
    uint8_t long_str[20];  // 用于记录长名称参数的索引
    uint8_t huge_str[20];  // 用于记录超长名称参数的索引
    uint8_t short_size;    // 短名称参数的数量
    uint8_t long_size;     // 长名称参数的数量
    uint8_t huge_size;     // 超名称长参数的数量
    uint8_t cross_size;    // 短名称参数和长名称参数显示在同一行的行数
    uint8_t panel_line;    // 规划显示参数的函数
    uint8_t paralist_size; // 可调参数列表的长度
    uint8_t panel_size;    // 显示参数列表的长度

    void Printf(uint8_t y, uint8_t x, const char *fmt, ...);
    void PrintfW(uint8_t y, uint8_t x, const char *fmt, ...);
    void show_line(uint8_t y, uint8_t index);
    int type_convert(PANEL *pan);

public:
    enum menu_state
    {
        NONE = 0,
        UP,
        DOWN,
        SWITCH,
        SELECT,
        LAUNCH
    };
    
    menu_state state;

    void show_page();
    void show_para();
    void button_event();
    menu_class(PARAM *para_list, PANEL *panel);
    void menu_init();
};
/****************************extern************************/
extern menu_class menu;
/**********************************************************/
#endif

#endif