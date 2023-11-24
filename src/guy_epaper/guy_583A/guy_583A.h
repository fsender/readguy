/******************** F r i e n d s h i p E n d e r ********************
 * 本程序隶属于 Readguy 开源项目, 请尊重开源开发者, 也就是我FriendshipEnder.
 * 如果有条件请到 extra/artset/reward 中扫描打赏,否则请在 Bilibili 上支持我.
 * 项目交流QQ群: 926824162 (萌新可以进来问问题的哟)
 * 郑重声明: 未经授权还请不要商用本开源项目编译出的程序.
 * @file guy_583A.h
 * @author FriendshipEnder (f_ender@163.com), Bilibili: FriendshipEnder
 * @brief 墨水屏 驱动库函数 头文件模板
 *        使用前, 先查找替换 583A 改为你需要的驱动名称
 *        比如3英寸的屏幕驱动程序应该叫 guy_dev_300a
 *        此时需要将 583A 替换为300A.
 *        替换好之后, 改文件名, 大小写也要注意改 (文件名最好不要出现大写)
 *        最后把这个brief替换掉
 * @version 1.0
 * @date 2023-11-11

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
#if (!defined(_GUY_EPD583A_H_FILE) && (defined(READGUY_DEV_583A)))
#define _GUY_EPD583A_H_FILE

namespace guydev_583A{
constexpr int GUY_D_WIDTH       =600;  //驱动屏幕宽度
constexpr int GUY_D_HEIGHT      =448;  //驱动屏幕高度
constexpr int slowRefTime       =1600; //驱动屏幕慢刷时间, 单位毫秒
constexpr int fastRefTime       =560;  //驱动屏幕快刷时间, 单位毫秒

class drv : public readguyEpdBase {
public:
#ifdef READGUY_583A_DUAL_BUFFER
  drv();
  virtual ~drv();
#endif
  int drv_ID() const { return READGUY_DEV_583A; }
  void drv_init(); //初始化屏幕
  void drv_fullpart(bool part); //切换慢刷/快刷功能
  void drv_dispWriter(std::function<uint8_t(int)> f,uint8_t m=3); //按照函数刷新
  void drv_sleep() ; //开始屏幕睡眠
  int drv_width() const { return GUY_D_WIDTH;  }; //返回显示区域宽度
  //int drv_panelwidth() const { return GUY_D_WIDTH;  }; //返回缓存的数据宽度
  int drv_height() const{ return GUY_D_HEIGHT; }; //返回显示区域高度
  void drv_setDepth(uint8_t i); //设置显示颜色深度
  int drv_supportGreyscaling() const { return (_quality&1)?16:-16; } //暂不支持灰度功能移植
  void drv_draw16grey_step(std::function<uint8_t(int)> f, int step);
private:
  uint8_t epd_PowerOn=1;  //是否上电. 睡眠则设为0
  uint8_t epdFull=0;      //是partical模式/快速刷新模式 0快刷, 1慢刷
  uint8_t iLut=15;        //颜色深度
  uint8_t greyscalingHighQuality = 0;
  static const unsigned char *luts[10];
  static const unsigned char *luts_fast[10];
  static const unsigned char lut_20_LUTC[];
  static const unsigned char lut_20_LUTCFAST[];
  static const unsigned char lut_21_LUTB[];
  static const unsigned char lut_21_LUTBFAST[];
  static const unsigned char lut_22_LUTW[];
  static const unsigned char lut_22_LUTWFAST[];
  static const unsigned char lut_23_LUTG1[];
  static const unsigned char lut_24_LUTG2[];
  static const unsigned char lut_25_LUTR0[];
  static const unsigned char lut_26_LUTR1[];
  static const unsigned char lut_27_LUTR2[];
  static const unsigned char lut_28_LUTR3[];
  static const unsigned char lut_29_LUTXON[];
  static const unsigned char customGrey[];
  void sendLut(int lutid);
  void epd_init();
  void epd_sendZoneInfo();
  void epd_sendWriter(std::function<uint8_t(int)> f);
#ifdef READGUY_583A_DUAL_BUFFER
  uint8_t *buf_2nd=nullptr; //second Buffer for luts
#endif
};
} 
#endif /* END OF FILE. ReadGuy project.
Copyright (C) 2023 FriendshipEnder. */
