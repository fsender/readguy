/**
 * @file main.cpp
 * @author ex01_helloWorld
 * @brief 基础的实例. 同时注入了少许功能性驱动代码.
 * http://www.lilygo.cn/prod_view.aspx?TypeId=50036&Id=1318
 * 项目基于 TTGO T-block Ep-bk (带有背光功能的ESP32电子墨水屏模块)
 * 
 * 注意屏幕所用的 EPD_RESET 其实是接在 GPIO38 上面, 手册写的 GPIO34 是错误的
 * GPIO38 为 SENSOR_CAPN 引脚(可能是供内部使用),
 * 在标准的 ESP32-WROOM 和 WROVER 模组上未引出此 GPIO.
 * 可能影响一部分功能的正常使用. 不过无伤大雅
 * 
 * @version 0.1
 * @date 2022-11-23
 * 
 * @copyright Copyright (c) 2022
 * 
 */