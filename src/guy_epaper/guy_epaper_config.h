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
#define FLOYD_STEINBERG_DITHERING //默认使用性能更好的floyd steinberg抖动算法, 取消注释则使用bayer图案抖动算法

// ***********************************************************************

//设备设置: 取消对应设备就可以直接禁用掉对应设备
//如果程序不想支持此型号(为了节省flash),请注释掉这一行, 实测根本不会节省多少空间
#define READGUY_DEV_154A     0
#define READGUY_DEV_154B     1
#define READGUY_DEV_213A     2
#define READGUY_DEV_213B     3
#define READGUY_DEV_266A     4
#define READGUY_DEV_270B     5
#define READGUY_DEV_290A     6
#define READGUY_DEV_290B     7
#define READGUY_DEV_370A     8
#define READGUY_DEV_420A     9
#define READGUY_DEV_420B    10
#define MEPD_DEBUG_DISPLAY  11 //使用LCD显示屏幕来debug

#endif /* END OF FILE. ReadGuy project.
Copyright (C) 2023 FriendshipEnder. */