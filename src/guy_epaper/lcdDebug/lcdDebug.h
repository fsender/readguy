/******************** F r i e n d s h i p E n d e r ********************
 * 本程序隶属于 Readguy 开源项目, 请尊重开源开发者, 也就是我FriendshipEnder.
 * 如果有条件请到 extra/artset/reward 中扫描打赏,否则请在 Bilibili 上支持我.
 * 项目交流QQ群: 926824162 (萌新可以进来问问题的哟)
 * 郑重声明: 未经授权还请不要商用本开源项目编译出的程序.
 * @file lcdDebug.cpp
 * @author FriendshipEnder (f_ender@163.com), Bilibili: FriendshipEnder
 * @brief 调试用墨水屏模拟器 驱动头文件 可使用LCD屏幕仿真墨水屏使用或测试
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

#ifndef _LCD_DEBUG_H_FILE
#define _LCD_DEBUG_H_FILE

#include "../guy_epdbase.h"
#ifdef MEPD_DEBUG_DISPLAY
#include "ctg_stack_c_defines.h"

namespace EpdLcdDebug{

class drv : public readguyEpdBase {
public:
    int drv_ID() const { return MEPD_DEBUG_DISPLAY; }
    void drv_init(); //初始化屏幕
    void drv_fullpart(bool part); //切换慢刷/快刷功能
    void drv_dispWriter(std::function<uint8_t(int)> f); //单色刷新
    void drv_sleep() ; //开始屏幕睡眠
    int drv_width() const { return ips.width()-2*WHITE_GAP; } //返回显示区域宽度
    int drv_height() const{ return ips.height()-2*WHITE_GAP; } //返回显示区域高度
    void drv_setDepth(uint8_t i) { if(i<=15) depth=i; if(!i) depth=15;} //设置显示颜色深度
    int drv_supportGreyscaling() const { return 16; }
private:
    LGFX ips;
    int16_t depth=15;
    int16_t partMode =0;
};
} 
#endif
#endif /* END OF FILE. ReadGuy project.
Copyright (C) 2023 FriendshipEnder. */
