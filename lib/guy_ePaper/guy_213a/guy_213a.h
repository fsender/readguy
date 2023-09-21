/******************** F r i e n d s h i p E n d e r ********************
 * 本程序隶属于 Readguy 开源项目, 请尊重开源开发者, 也就是我FriendshipEnder.
 * 如果有条件请到 extra/artset/reward 中扫描打赏,否则请在 Bilibili 上支持我.
 * 项目交流QQ群: 926824162 (萌新可以进来问问题的哟)
 * 郑重声明: 未经授权还请不要商用本开源项目编译出的程序.
 * @file guy_213a.h
 * @author FriendshipEnder (f_ender@163.com), Bilibili: FriendshipEnder
 * @brief 2.13英寸墨水屏 驱动库函数 头文件
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
#include "../guy_epdbase.h"
#if (!defined(_GUY_EPD213A_H_FILE) && (defined(READGUY_DEV_213A)))
#define _GUY_EPD213A_H_FILE

namespace guydev_213A{
// Display resolution
/* the resolution is 122x250 in fact */
/* however, the logical resolution is 128x250 */
constexpr int GUY_D_WIDTH       =128;
constexpr int EPD_REAL_WIDTH  =122; //适用于很多场景的实际宽度
constexpr int GUY_D_HEIGHT      =250;

// EPD2IN13 commands
extern const unsigned char lut_slow[];
extern const unsigned char lut_fast[];

class drv : public readguyEpdBase {
public:
  int drv_ID() const { return READGUY_DEV_213A; }
  void drv_init(); //初始化屏幕
  void drv_fullpart(bool part); //切换慢刷/快刷功能
  void drv_dispWriter(std::function<uint8_t(int)> f); //按照函数刷新
  void drv_sleep() ; //开始屏幕睡眠
  int drv_width() const { return EPD_REAL_WIDTH;  }; //返回显示区域宽度
  //int drv_panelwidth() const { return GUY_D_WIDTH;  }; //返回缓存的数据宽度
  int drv_height() const{ return GUY_D_HEIGHT; }; //返回显示区域高度
  void drv_setDepth(uint8_t i){ epdFull=0; iLut = i?(i>15?15:i):15; } //设置显示颜色深度
  int drv_supportGreyscaling() const { return 16; }
private:
  void epd_Init(void);
  uint8_t epdFull;     //是partical模式/快速刷新模式 0快刷, 1慢刷
  uint8_t epd_PowerOn = 0; //是否上电
  uint8_t iLut = 15; //颜色深度(灰度模式用)
 // uint8_t dc_d = 0;        //dc引脚状态 0 command, 1 data
  static const PROGMEM uint8_t _ed_lut_full[];
  static const PROGMEM uint8_t _ed_lut_part[];

};
} 
#endif /* END OF FILE. ReadGuy project.
Copyright (C) 2023 FriendshipEnder. */
