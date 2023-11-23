#include "headfile.h"
#include "start.hpp"
using namespace std;
string str_test = "hello";

void setup()
{
	menu.menu_init();
	//	adc_init();
	Timer0_Init();
	// QEI_Init();
	pwm5.pwm_init();
	pwm6.pwm_init();
	pwm7.pwm_init();
	// ConfigureUART_0();
	// ConfigureUART_1();
	ConfigureUART_2();
	ConfigureUART_3();
}

void loop()
{
	menu.show_para();
	menu.show_page();
	
}
