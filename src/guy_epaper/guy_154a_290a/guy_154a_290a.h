/******************** F r i e n d s h i p E n d e r ********************
 * 本程序隶属于 Readguy 开源项目, 请尊重开源开发者, 也就是我FriendshipEnder.
 * 如果有条件请到 extra/artset/reward 中扫描打赏,否则请在 Bilibili 上支持我.
 * 项目交流QQ群: 926824162 (萌新可以进来问问题的哟)
 * 郑重声明: 未经授权还请不要商用本开源项目编译出的程序.
 * @file guy_154a_290a.h
 * @author FriendshipEnder (f_ender@163.com), Bilibili: FriendshipEnder
 * @brief 1.54英寸和2.9英寸墨水屏 驱动库函数头文件
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
#if ((!defined(_GUY_EPD154A290A_H_FILE)) && \
  (defined(READGUY_DEV_154A) || defined(READGUY_DEV_290A)))
#define _GUY_EPD154A290A_H_FILE

namespace guydev_154A_290A{
extern const unsigned char lut_slow[];
extern const unsigned char lut_fast[];
extern const unsigned char lut_grey_update[];

class drvBase : public readguyEpdBase {
public:
    virtual int drv_ID() const=0;
    void drv_init(); //初始化屏幕
    void drv_fullpart(bool part); //切换慢刷/快刷功能
    void drv_dispWriter(std::function<uint8_t(int)> f); //按照函数刷新
    void drv_sleep() ; //开始屏幕睡眠
    int drv_width() const { return epdWidth;  }; //返回显示区域宽度
    int drv_height() const{ return epdHeight; }; //返回显示区域高度
    void drv_setDepth(uint8_t i); //设置显示颜色深度
    int drv_supportGreyscaling() const { return 16; }
protected:
  int epdWidth;
  int epdHeight;
  int idleSlowRf;
  int idleFastRf;
private:
    int  Init(const unsigned char* lut);
    const unsigned char* lut;
    uint8_t iLut = 15;
    uint8_t sleeping=1;

    void SetLut(const unsigned char* lut);
};

#ifdef READGUY_DEV_154A
class dev154A : public drvBase {
  public:
    dev154A(){ epdWidth=200; epdHeight=200; idleSlowRf=1300; idleFastRf=350; }
    int drv_ID() const { return READGUY_DEV_154A; }
};
#endif
#ifdef READGUY_DEV_290A
class dev290A : public drvBase {
  public:
    dev290A(){ epdWidth=128; epdHeight=296; idleSlowRf=1800; idleFastRf=550; }
    int drv_ID() const { return READGUY_DEV_290A; }
};
#endif

}

#endif /* END OF FILE. ReadGuy project.
Copyright (C) 2023 FriendshipEnder. */
