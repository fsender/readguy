/******************** F r i e n d s h i p E n d e r ********************
 * 本程序隶属于 Readguy 开源项目, 请尊重开源开发者, 也就是我FriendshipEnder.
 * 如果有条件请到 extra/artset/reward 中扫描打赏,否则请在 Bilibili 上支持我.
 * 项目交流QQ群: 926824162 (萌新可以进来问问题的哟)
 * 郑重声明: 未经授权还请不要商用本开源项目编译出的程序.
 * @file guy_template.h
 * @author FriendshipEnder (f_ender@163.com), Bilibili: FriendshipEnder
 * @brief 墨水屏 驱动库函数 头文件模板
 *        使用前, 先查找替换 template 改为你需要的驱动名称
 *        比如3英寸的屏幕驱动程序应该叫 guy_dev_300a
 *        此时需要将 template 替换为300A.
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
#if (!defined(_GUY_EPDtemplate_H_FILE) && (defined(READGUY_DEV_template)))
#define _GUY_EPDtemplate_H_FILE

namespace guydev_template{
constexpr int GUY_D_WIDTH       =200;  //驱动屏幕宽度
constexpr int GUY_D_HEIGHT      =200;  //驱动屏幕高度
constexpr int slowRefTime       =2000; //驱动屏幕慢刷时间, 单位毫秒
constexpr int fastRefTime       =500;  //驱动屏幕快刷时间, 单位毫秒

class drv : public readguyEpdBase {
public:
  /** @brief 返回驱动程序ID. 此函数不需要在 cpp 文件内重写
   *  @return int 直接返回对应宏定义就可以  */
  int drv_ID() const { return READGUY_DEV_template; }
  /// @brief 初始化屏幕 不过大多数时候此函数只需要初始化启动变量就行
  // 比如将模式设为慢刷, 设置为未上电状态 这样下次刷新必为全屏慢刷
  void drv_init();
  /// @brief 切换慢刷/快刷功能
  /// @param part 为1则为快刷, 为0则为慢刷
  void drv_fullpart(bool part);
  /** @brief 刷屏函数. 程序接口按照此函数刷新
  /   @param f 读取像素数据的函数. 这个函数用于替代屏幕缓存数组.
  /  因为有时候屏幕缓存数组不能满足一些显示场景, 比如存储空间复用, 缩放显示等
  /   @param m 刷新模式: 
  /   1-仅执行前半部分 执行前半部分之后将会向屏幕发送数据后立即退出. (不等busy信号)
  /   2-仅执行后半部分  执行后半部分之后会进行屏幕刷新完之后该执行的操作
  /   3-完整刷屏: 执行1部分->等待busy信号->执行2部分  */
  void drv_dispWriter(std::function<uint8_t(int)> f,uint8_t m=3);
  /// @brief 开始屏幕睡眠/低功耗模式
  void drv_sleep() ; 
  /// @brief 返回显示区域宽度
  int drv_width() const { return GUY_D_WIDTH;  }; 
  /// @brief 返回显示区域高度
  int drv_height() const{ return GUY_D_HEIGHT; };
  /** @brief 设置显示颜色深度. 只有在受支持 屏幕上才可以设置灰度
  /   @param i 有效值 1~16 0必须为无效 */
  void drv_setDepth(uint8_t i);
  /** @brief 设置屏幕是否支持连续灰度刷新.
  /   @return 设置为 0 不支持灰度 16 支持灰度 -16 支持连续刷新灰度
  / 连续刷新灰度: 先刷深色部分 再刷浅色部分, 原来的深色部分每次刷新都会逐渐越来越深色.
  / 如果不提供连续刷新灰度接口 则使用setDepth函数 先刷浅色部分 再刷深色部分
  / 可以在支持连续刷新的屏幕上烧录范例程序查看效果. 通常都是好于非连续刷新的灰度 */
  int drv_supportGreyscaling() const { return 16; }
  // 在支持连续灰度刷新的屏幕上 还要额外实现一个函数用于连续刷新接口 
  /// @brief 设置连续刷新功能函数. 范例可以看guy_420a文件内的示例,分步执行连续刷灰度
  //void drv_draw16grey_step(std::function<uint8_t(int)> f, int step);
private:
  uint8_t epd_PowerOn=1;  //是否上电. 睡眠则设为0
  uint8_t epdFull=0;      //是partical模式/快速刷新模式 0快刷, 1慢刷
  uint8_t iLut=15;        //颜色深度 1-15均为有效. 慢刷模式中 此数值为15.
};
} 
#endif /* END OF FILE. ReadGuy project.
Copyright (C) 2023 FriendshipEnder. */
