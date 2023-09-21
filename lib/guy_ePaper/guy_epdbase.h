/******************** F r i e n d s h i p E n d e r ********************
 * 本程序隶属于 Readguy 开源项目, 请尊重开源开发者, 也就是我FriendshipEnder.
 * 如果有条件请到 extra/artset/reward 中扫描打赏,否则请在 Bilibili 上支持我.
 * 项目交流QQ群: 926824162 (萌新可以进来问问题的哟)
 * 郑重声明: 未经授权还请不要商用本开源项目编译出的程序.
 * @file guy_epdbase.h
 * @author FriendshipEnder (f_ender@163.com), Bilibili: FriendshipEnder
 * @brief readguy EPD 驱动程序 公共基类 头文件. 所有驱动都要依赖此文件.
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

#ifndef _READGUY_EPDBASE_H_FILE
#define _READGUY_EPDBASE_H_FILE

#include <Arduino.h>
#include <SPI.h>
#define LGFX_USE_V1
#include <LovyanGFX.hpp>
#include "guy_epaper_config.h"
#define EPD_DRIVERS_NUM_MAX 12 //此选项请不要取消注释掉

class readguyEpdBase {
protected:
    SPIClass *_spi;
    // Pin definition
    int8_t RST_PIN ;
    int8_t DC_PIN  ;
    int8_t CS_PIN  ;
    int8_t BUSY_PIN;
    uint8_t in_trans=0;
    uint8_t _quality=0;  //灰度显示品质 0(默认)-高品质 1-低品质 部分屏幕支持高品质的连续刷灰度.
  #ifdef MEPD_DEBUG_WAVE
    uint16_t dat_combo = 0;   //dc引脚状态 0 command, 1 data
  #endif

public:
    readguyEpdBase(void);
    virtual ~readguyEpdBase(void);
    int  IfInit(SPIClass &c,int8_t cs,int8_t dc,int8_t rst,int8_t busy);
    void DigitalWrite(int pin, int value); 
    int  DigitalRead(int pin);
    void DelayMs(unsigned int delaytime);
    void BeginTransfer();
    void EndTransfer();
    void SpiTransfer(unsigned char data);
    //basic I/O operation
    void guy_epdCmd(unsigned char command);
    void guy_epdParam(unsigned char data);
    void guy_epdBusy(int32_t maxt);
    void Reset(uint32_t minTime = 20);
    void SetMemory(); //集成了0x44 0x45 0x4e 0x4f指令的函数. 此函数用于设置墨水屏内存写入方式

    virtual int drv_ID() const =0;   //返回驱动代号
    virtual void drv_init()=0; //初始化屏幕
    virtual void drv_fullpart(bool part)=0; //初始化慢刷功能
    void _display(const uint8_t *d){ drv_dispWriter([&](int n)->uint8_t{return d[n];}); }
    virtual void drv_dispWriter(std::function<uint8_t(int)>)=0; //按照显示函数刷新
    void drv_color(uint8_t c){ drv_dispWriter([=](int)->uint8_t{return c;}); } //单色刷新
    virtual void drv_sleep() =0; //开始屏幕睡眠
    virtual int  drv_width() const=0; //返回显示区域宽度, 即使旋转了也不能影响此函数输出
    virtual int  drv_height()const=0; //返回显示区域高度, 即使旋转了也不能影响此函数输出
    
    virtual int drv_supportGreyscaling() const { return 0; }
    virtual void drv_setDepth(uint8_t i){} //设置显示颜色深度, 不支持的话什么都不做
    /** @brief 获取某一像素颜色, 并转化为256阶灰度
     *  @param x, y 坐标
     *  @param gamma_on 是否对灰度值进行gamma校正(速度慢)
     *  @return uint32_t 颜色的灰度值
     */
    static int greysc(int got){ return (int)((got&0xf8)*299+(((got>>11)&0x1c)|((got<<5)&0xe0))*587+((got&0x1f00)>>5)*114)/978; }
    void drv_drawImage(LGFX_Sprite &sprbase,LGFX_Sprite &spr,uint16_t x,uint16_t y); //分步完成灰度刷新
    void drv_draw16grey(LGFX_Sprite &sprbase,LGFX_Sprite &spr,uint16_t x,uint16_t y);//省内存方式
    void drv_draw16grey_step(const uint8_t *buf, int step){ //分步完成灰度刷新
      drv_draw16grey_step([&](int n)->uint8_t{return buf[n];},step);
    }
    virtual void drv_draw16grey_step(std::function<uint8_t(int)> f, int step){ //分步完成灰度刷新
      drv_setDepth(step);
      drv_dispWriter(f);
    }
    void setGreyQuality(bool q) { _quality=!q; } //设置灰度的渲染画质. 高画质模式在某些屏幕某些情况下可能表现不好.
    void (*spi_tr_release)(void);
    void (*spi_tr_press)(void);
#ifdef MEPD_DEBUG_DISPLAY
    friend class LGFX;
#endif
};

#endif /* END OF FILE. ReadGuy project.
Copyright (C) 2023 FriendshipEnder. */
