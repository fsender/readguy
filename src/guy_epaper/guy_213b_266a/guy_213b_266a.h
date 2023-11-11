/******************** F r i e n d s h i p E n d e r ********************
 * 本程序隶属于 Readguy 开源项目, 请尊重开源开发者, 也就是我FriendshipEnder.
 * 如果有条件请到 extra/artset/reward 中扫描打赏,否则请在 Bilibili 上支持我.
 * 项目交流QQ群: 926824162 (萌新可以进来问问题的哟)
 * 郑重声明: 未经授权还请不要商用本开源项目编译出的程序.
 * @file guy_213b_266a.h
 * @author FriendshipEnder (f_ender@163.com), Bilibili: FriendshipEnder
 * @brief 2.13英寸B型墨水屏和 2.66英寸墨水屏 驱动库函数 头文件
 * 增加了对三色屏幕的支持
 * @version 1.1
 * @date create: 2023-09-21
 * last modify: 2023-11-11

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
#if (!defined(_GUY_EPD213B_266A_H_FILE) && \
  (defined(READGUY_DEV_213B) || defined(READGUY_DEV_213B3C) \
|| defined(READGUY_DEV_266A) || defined(READGUY_DEV_266A3C)))
#define _GUY_EPD213B_266A_H_FILE

namespace guydev_213B_266A{
class drv_base : public readguyEpdBase {
public:
  drv_base();
  virtual int drv_ID() const=0;
  void drv_init(); //初始化屏幕
  void drv_fullpart(bool part); //切换慢刷/快刷功能
  void drv_dispWriter(std::function<uint8_t(int)> f,uint8_t m=3); //按照函数刷新
  void drv_sleep() ; //开始屏幕睡眠
  int drv_width() const { return epdWidth;  }; //返回显示区域宽度
  int drv_height() const{ return epdHeight; }; //返回显示区域高度
  void drv_setDepth(uint8_t i); //设置显示颜色深度
  int drv_supportGreyscaling() const { return (_quality&1)?16:-16; }
  void drv_draw16grey_step(std::function<uint8_t(int)> f, int step);
protected:
  int epdWidth;
  int epdHeight;
  void epd_init();
  void send_zoneInfo();
  void SendLuts(bool part_lut);
  uint8_t part_mode = 1;
  uint8_t Power_is_on = 2; //初始为未上电
  uint8_t greyLut=15;
  uint8_t greyHQ=4; // greyHQ==3 时 为高品质刷新灰度 否则为常规方式刷新灰度
  uint8_t refTime=0;
  uint8_t isBWR=0; //三色2.66将此屏幕数值改为1.
  int16_t slowRefTime;
  int16_t fastRefTime;
  const uint8_t *guy_lutArray[5];
};
#ifdef READGUY_DEV_213B
class dev213B : public drv_base {
  public:
    dev213B(){ epdWidth=104; epdHeight=212; slowRefTime=1300; fastRefTime=200; }
    int drv_ID() const { return READGUY_DEV_213B; }
};
#endif
#ifdef READGUY_DEV_213B3C
class dev213B3C : public drv_base {
  public:
    dev213B3C(){ epdWidth=104; epdHeight=212; isBWR=1; slowRefTime=2800; fastRefTime=800; }
    int drv_ID() const { return READGUY_DEV_213B3C; }
};
#endif
#ifdef READGUY_DEV_266A
class dev266A : public drv_base {
  public:
    dev266A(){ epdWidth=152; epdHeight=296; slowRefTime=1300; fastRefTime=200; }
    int drv_ID() const { return READGUY_DEV_266A; }
};
#endif
#ifdef READGUY_DEV_266A3C
class dev266A3C : public drv_base {
  public:
    dev266A3C(){ epdWidth=152; epdHeight=296; isBWR=1; slowRefTime=2800; fastRefTime=800; }
    int drv_ID() const { return READGUY_DEV_266A3C; }
};
#endif
} 
#endif /* END OF FILE. ReadGuy project.
Copyright (C) 2023 FriendshipEnder. */
