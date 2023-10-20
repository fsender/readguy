/******************** F r i e n d s h i p E n d e r ********************
 * 本程序隶属于 Readguy 开源项目, 请尊重开源开发者, 也就是我FriendshipEnder.
 * 如果有条件请到 extra/artset/reward 中扫描打赏,否则请在 Bilibili 上支持我.
 * 项目交流QQ群: 926824162 (萌新可以进来问问题的哟)
 * 郑重声明: 未经授权还请不要商用本开源项目编译出的程序.
 * @file guy_epaper_config.h
 * @author FriendshipEnder (f_ender@163.com), Bilibili: FriendshipEnder
 * @brief EPD驱动基础配置文件. 用户可以根据自己对库的编译需求来修改此文件.
 * @version 1.0
 * @date 2023-09-21

 * @attention
 * Copyright (c) 2022-2023 FriendshipEnder
 * 
 * Apache License, Version 2.0
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 *     http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef _GUY_E_PAPER_CONFIG_H_FILE
#define _GUY_E_PAPER_CONFIG_H_FILE

//#define MEPD_DEBUG_WAVE 16 //取消此行注释来监视SPI的通信数据 (用于debug), 可以查看主控和屏幕的通信数据
//#define MEPD_DEBUG_WAITTIME //显示墨水屏的刷新时间, 单位是毫秒
#define FLOYD_STEINBERG_DITHERING //默认使用更好的floyd steinberg抖动算法,取消注释则用bayer图案抖动算法
#define FLOYD_DITHERING_16GREY //使用更好的floyd steinberg抖动算法显示16阶灰度,取消注释则使用阈值填充

// ***********************************************************************

//设备设置: 取消对应设备就可以直接禁用掉对应设备
//非常不建议用户在应用程序中禁用掉设备. 实测根本不会节省很多的空间. 如果要节省代码占用的flash空间,
//请在字库上下功夫.
//如果程序不想支持此型号(为了节省flash),请注释掉这一行, 实测每个设备只能节省100字节左右的flash空间
#define READGUY_DEV_154A     0 //1.54寸标准, 甘草酸不酸使用的1.54默认屏幕型号. 1.54汉朔价签也选这个
#define READGUY_DEV_154B     1 //1.54寸Lilygo, lilygo的1.54触摸和1.54背光墨水屏. GDEH0154D67
#define READGUY_DEV_213A     2 //2.13寸汉朔价签, 价格便宜, 兼容性好. 夏襄居士单词卡用 GDE0213B1
#define READGUY_DEV_213B     3 //2.13寸三色, 分辨率低212x104, 部分价签也用这个屏 易老化
#define READGUY_DEV_266A     4 //2.66寸Vusion价签, 黑白双色.
#define READGUY_DEV_270B     5 //2.7寸佳显触摸墨水屏, 佳显官方店有售: 2.7寸触摸墨水屏.
#define READGUY_DEV_290A     6 //2.9寸A01, 甘草酸不酸2.9寸A01型号. 显示性能一般
#define READGUY_DEV_290B     7 //2.9寸T94背光, 甘草酸不酸2.9寸T94型号. 显示性能更好, 支持前置光
#define READGUY_DEV_370A     8 //3.7寸高DPI墨水屏, 480x280分辨率, 显示效果非常好
#define READGUY_DEV_420A     9 //4.2寸HINK黑白价签, 汉朔价签, 汉朔三色价签通用. 速度快, 但效果一般
#define READGUY_DEV_420B    10 //4.2寸WF丝印, 部分价签也用此型号屏, 排线有WF开头的丝印
#define MEPD_DEBUG_DISPLAY  11 //使用LCD显示屏幕来debug. 前往ctg_stack_c_defines.h修改兼容的屏幕.

#endif /* END OF FILE. ReadGuy project.
Copyright (C) 2023 FriendshipEnder. */