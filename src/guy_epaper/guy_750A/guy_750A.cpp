/******************** F r i e n d s h i p E n d e r ********************
 * 本程序隶属于 Readguy 开源项目, 请尊重开源开发者, 也就是我FriendshipEnder.
 * 如果有条件请到 extra/artset/reward 中扫描打赏,否则请在 Bilibili 上支持我.
 * 项目交流QQ群: 926824162 (萌新可以进来问问题的哟)
 * 郑重声明: 未经授权还请不要商用本开源项目编译出的程序.
 * @file guy_750A.cpp
 * @author FriendshipEnder (f_ender@163.com), Bilibili: FriendshipEnder
 * @brief 墨水屏 驱动库函数 源代码模板
 *        使用前, 先查找替换 750A 改为你需要的驱动名称
 *        比如3英寸的屏幕驱动程序应该叫 guy_dev_300a
 *        此时需要将 750A 替换为300A.
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
#include <stdlib.h>
#include "guy_750A.h"
#ifdef READGUY_DEV_750A
namespace guydev_750A{
//以下代码均为我 FriendshipEnder 原创, 呵呵哒~~

void drv::drv_init(){ //初始化屏幕
  //add driver code...
}
void drv::drv_fullpart(bool part){ //初始化慢刷功能
  if(lastRefresh) return;
  //add driver code...
}
void drv::drv_setDepth(uint8_t i){
  epdFull=0; iLut = i?(i>15?15:i):15;  //如果需要, 改成自己的代码
}
void drv::drv_dispWriter(std::function<uint8_t(int)> f,uint8_t m){ //单色刷新
  if(m&1){//stage 1
    if(lastRefresh) drv_dispWriter(f,2);
    //add driver code...
  }
  if(m&2){//stage 2
    uint32_t ms=millis()-lastRefresh;
    uint32_t u=epdFull?slowRefTime:fastRefTime;  //全刷:局刷 busy时间
    if(ms<u) guy_epdBusy(u-ms);   //对于busy电平为低电平忙碌,高电平正常的屏幕则改为ms-u
    lastRefresh=0;
    //add driver code...
  }
}
void drv::drv_sleep() { //开始屏幕睡眠
  if(RST_PIN>=0){ //RST_PIN<0 无法唤醒
    //add driver code...
  }
  epd_PowerOn = 0;
  epdFull = 1; //强制设置为慢刷新模式
}
}
#endif /* END OF FILE. ReadGuy project.
Copyright (C) 2023 FriendshipEnder. */


