#include "stdafx.h"

//定义运行的全局变量

CString g_cascade_name = "..\\classifier\\haarcascade_frontalface_alt.xml";

double g_scale_factor = 1.1; //搜索窗口比例

int g_min_neighbors = 3; //最小相邻矩形

int g_flags = 0;  //是否做边缘检测 ，0表示做，-1表示不做

int g_min_size_w = 20;  //检测窗口最小宽度

int g_min_size_h = 20;   //检测窗口最小高度