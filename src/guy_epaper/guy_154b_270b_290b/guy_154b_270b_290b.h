/******************** F r i e n d s h i p E n d e r ********************
 * 本程序隶属于 Readguy 开源项目, 请尊重开源开发者, 也就是我FriendshipEnder.
 * 如果有条件请到 extra/artset/reward 中扫描打赏,否则请在 Bilibili 上支持我.
 * 项目交流QQ群: 926824162 (萌新可以进来问问题的哟)
 * 郑重声明: 未经授权还请不要商用本开源项目编译出的程序.
 * @file guy_154b_270b_290b.h
 * @author FriendshipEnder (f_ender@163.com), Bilibili: FriendshipEnder
 * @brief 1.54英寸B型 2.7英寸B型和2.9英寸B型墨水屏 驱动库函数头文件
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
#if ((!defined(_GUY_EPD154B270B290B_H_FILE)) && (defined(READGUY_DEV_154B)\
  || defined(READGUY_DEV_270B) || defined(READGUY_DEV_290B)))
#define _GUY_EPD154B270B290B_H_FILE

namespace guydev_154B_270B_290B{
// Display resolution

class drvSSD168x : public readguyEpdBase {
public:
  virtual int drv_ID() const =0;
  void drv_init(); //初始化屏幕
  void drv_fullpart(bool part); //切换慢刷/快刷功能
  void drv_dispWriter(std::function<uint8_t(int)> f); //按照函数刷新
  void drv_sleep() ; //开始屏幕睡眠
  int drv_width() const { return epdWidth;  }; //返回显示区域宽度
  int drv_height() const{ return epdHeight; }; //返回显示区域高度
  int drv_supportGreyscaling() const { return (_quality&1)?16:-16; }
  void drv_setDepth(uint8_t i); //设置显示颜色深度, 不支持的话什么都不做
  void drv_draw16grey_step(std::function<uint8_t(int)> f, int step);
protected:
  // Display resolution
  int epdWidth;
  int epdHeight;
private:
  uint8_t _part;
  uint8_t epd_PowerOn = 0;
  uint8_t iLut=15;
  uint8_t greyScaling=0;
  //static const unsigned char _WF_PARTIAL_2IN9[48] ;
  static const unsigned char WS_20_30[48] ;
  static const unsigned char VSH_table[32];

  void epd_init();
  void DisplayFrame(void);
	void SetLut();
};

#ifdef READGUY_DEV_154B
class dev154B : public drvSSD168x {
  public:
    dev154B(){ epdWidth=200; epdHeight=200; }
    int drv_ID() const { return READGUY_DEV_154B; }
};
#endif
#ifdef READGUY_DEV_270B
class dev270B : public drvSSD168x {
  public:
    dev270B(){ epdWidth=176; epdHeight=264; }
    int drv_ID() const { return READGUY_DEV_270B; }
};
#endif
#ifdef READGUY_DEV_290B
class dev290B : public drvSSD168x {
  public:
    dev290B(){ epdWidth=128; epdHeight=296; }
    int drv_ID() const { return READGUY_DEV_290B; }
};
#endif
}
#endif /* END OF FILE. ReadGuy project.
Copyright (C) 2023 FriendshipEnder. */
