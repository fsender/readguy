/******************** F r i e n d s h i p E n d e r ********************
 * 本程序隶属于 Readguy 开源项目, 请尊重开源开发者, 也就是我FriendshipEnder.
 * 如果有条件请到 extra/artset/reward 中扫描打赏,否则请在 Bilibili 上支持我.
 * 项目交流QQ群: 926824162 (萌新可以进来问问题的哟)
 * 郑重声明: 未经授权还请不要商用本开源项目编译出的程序.
 * @file guy_420b.h
 * @author FriendshipEnder (f_ender@163.com), Bilibili: FriendshipEnder
 * @brief 4.2英寸WF丝印墨水屏 驱动库函数 头文件
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
#if (!defined(_GUY_EPD420B_H_FILE) && (defined(READGUY_DEV_420B)))
#define _GUY_EPD420B_H_FILE

namespace guydev_420B{
// Display resolution
constexpr int GUY_D_WIDTH        =400;
constexpr int GUY_D_HEIGHT       =300;
constexpr int CUSTOM_LUT_DISABLE =48;

class drv : public readguyEpdBase {
public:
    drv();
    int drv_ID() const { return READGUY_DEV_420B; }
    void drv_init(); //初始化屏幕
    //void drv_draw16grey(uint8_t *d16bit);
    void drv_fullpart(bool part); //切换慢刷/快刷功能
    void drv_dispWriter(std::function<uint8_t(int)> f); //按照函数刷新
    void drv_sleep() ; //开始屏幕睡眠
    int drv_width() const { return GUY_D_WIDTH;  }; //返回显示区域宽度
    int drv_height() const{ return GUY_D_HEIGHT; }; //返回显示区域高度
    void drv_setDepth(uint8_t i) {
      part_mode=1;
      if(i) customLut = i>15?45:i*3;
      else customLut = CUSTOM_LUT_DISABLE;
    } //设置显示颜色深度
    int drv_supportGreyscaling() const { return 16; }

private:
    uint8_t part_mode = 0;
    //uint8_t Power_is_on = 0; //初始为未上电
    
    static const PROGMEM unsigned char lut_vcom0[];
    static const PROGMEM unsigned char lut_ww[];
    static const PROGMEM unsigned char lut_bw[];
    static const PROGMEM unsigned char lut_bb[];
    static const PROGMEM unsigned char lut_wb[];
    // experimental partial screen update LUTs with balanced charge
    // LUTs are filled with zeroes
    static const PROGMEM unsigned char lutFast_vc0[];
    static const PROGMEM unsigned char lutFast_w_w[];
    static const PROGMEM unsigned char lutFast_b_w[];
    static const PROGMEM unsigned char lutFast_w_b[];
    static const PROGMEM unsigned char lutFast_b_b[];

    const unsigned char *guy_lutArray[16];
    void Init(uint8_t pt);
    void sendArea();
    void sendAreaRaw();

    void SendLuts(uint8_t lutOption); //0:慢刷, 1:快刷, 2:四阶灰度
    uint8_t customLut, customGreyscale; //customLut 是灰度刷新时的自定义lut
    void epd_display();
    //void (*send)(int);
    std::function<void(int)> send; //此处不能用 void (*send)(int); 是因为lambda函数是std的
    static const PROGMEM uint8_t greyTable[16];
};
}
#endif /* END OF FILE. ReadGuy project.
Copyright (C) 2023 FriendshipEnder. */
