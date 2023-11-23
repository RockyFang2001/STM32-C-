#include "headfile.h"
#include "setpara.hpp"
using namespace std;
setpara_struct setpara;
int Param_init_value;
/*************************************说明************************************/
// PANEL结构体成员变量type的参数赋值需要开启实时类型判断（RTTI）功能，才能查看变量类型
/*****************************************************************************/
float watch_value;
PARAM para_list[100]{
    /************************可调参数列表*************************/
    {"runspeed", setpara.runspeed, 2000, 10},
    {"stopspeed", setpara.slowspeed, 160, 10},
    {"stop_area", setpara.stop_area, 8, 1},
    {"slow_area", setpara.data_area, 10, 1},
    {"X.ki", setpara.x_ki, -5, 1},
    {"Y.ki", setpara.y_ki, 5, 1},
    {"Y.mid", setpara.y_mid, 1820, 10},
    {"Y.maxturn", setpara.y_maxturn, 1000, 1},
    {"X.mid", setpara.x_mid, 1610, 10},
    {"X.maxturn", setpara.x_maxturn, 1000, 1},

    /************************************************************/
};

PANEL panel[100]{
    /**********************显示参数列表***********************/
    {"T", &Watch_T},
    {"wv", &watch_value},
    {"poi", &mycar.point},
    {"a_x", &mycar.inser_x[mycar.point]},
    {"a_y", &mycar.inser_y[mycar.point]},
    {"p_x", &mycar.point_x},
    {"p_y", &mycar.point_y},
    {"x_in", &x_axis.input},
    {"y_in", &y_axis.input},
    {"x_out", &x_steer.output},
    {"y_out", &y_steer.output},
    /********************************************************/
};

menu_class menu(para_list, panel);
