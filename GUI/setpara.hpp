#ifndef __SETPARA_H
#define __SETPARA_H
#include "headfile.h"
#define NAME_SIZE 15

#ifdef __cplusplus
extern "C"
{
#endif
    /***********************对cpp文件提供C语言的接口*******************/
    /***********************可调参数*******************/
    typedef struct setpara_struct
    {
        int point;
        int x_lh;
        int x_ll;
        int x_rh;
        int x_rl;
        int y_lh;
        int y_ll;
        int y_rh;
        int y_rl;
        int runspeed;
        int slowspeed;
        int stop_area;
        int data_area;
        int x_mid;
        int x_maxturn;
        int x_kp;
        int x_ki;
        int x_kd;
        int y_mid;
        int y_maxturn;
        int y_kp;
        int y_ki;
        int y_kd;
        int setpwm;
        int runtime;
    } setpara_struct;
    /*******************************extern***************************/
    extern setpara_struct setpara;
/****************************************************************/
#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
/***********************C++文件之间的接口********************/
using namespace std;
extern int Param_init_value;
typedef struct PARAM
{
public:
    char name[NAME_SIZE]; // 可调参数名字
    uint32_t precision;   // 可调参数精度
    int &value;           // 可调参数传引用

    // 缺省构造,声明一个结构体数组的时候调用该函数
    PARAM()
        : value(Param_init_value)
    {
        strcpy(this->name, "");
    }

    // 带默认形参值的构造函数，参数初始化值默认为0，参数调整精度默认为1
    PARAM(const char *mname, int &mpara, int init_value = 0, uint32_t mprecision = 1)
        : value(mpara), precision(mprecision)
    {
        if (strlen(mname) > NAME_SIZE)
        {
            strncpy(this->name, mname, NAME_SIZE - 1);
            strcat(this->name, " ");
        }
        else
        {
            strcpy(this->name, mname);
            uint8_t i = NAME_SIZE - strlen(mname);
            while (i--)
                strcat(this->name, " ");
        }
        mpara = init_value;
    }
};

typedef struct PANEL
{
public:
    char name[NAME_SIZE];  // 显示参数的名称
    uint8_t len;           // 显示参数数字的位数
    uint8_t size;          // 显示参数名称的位数
    void *value;           // 显示参数的地址
    const type_info &type; // 显示参数的类型
                           //    float gain;
    // 缺省构造，默认类型初始化为int类型地址
    PANEL() : type(typeid(int *))
    {
        strcat(this->name, "");
        this->size = 0;
    }

    // 带默认形参值的构造函数,显示参数数字长度默认值为3
    // 通过函数模板实现任意类型参数传递
    template <typename Type>
    PANEL(const char *mname, Type *mvalue, uint8_t mlen = 3)
        : value(mvalue), type(typeid(mvalue)), len(mlen)
    {
        if (strlen(mname) >= NAME_SIZE)
        {
            strncpy(this->name, mname, NAME_SIZE - 1);
            this->size = 15;
        }
        else
        {
            strcpy(this->name, mname);
            this->size = strlen(mname);
        }
    }
};

/****************************extern************************/
extern PARAM para_list[100];
extern PANEL panel[100];
extern float watch_value;
/**********************************************************/
#endif

#endif