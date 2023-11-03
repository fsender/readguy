/******************** F r i e n d s h i p E n d e r ********************
 * 本程序隶属于 Readguy 开源项目, 请尊重开源开发者, 也就是我FriendshipEnder.
 * 如果有条件请到 extra/artset/reward 中扫描打赏,否则请在 Bilibili 上支持我.
 * 项目交流QQ群: 926824162 (萌新可以进来问问题的哟)
 * 郑重声明: 未经授权还请不要商用本开源项目编译出的程序.
 * @file guy_image.h
 * @author FriendshipEnder (f_ender@163.com), Bilibili: FriendshipEnder
 * @brief guy_image 基础功能 头文件. 
 * @version 1.0
 * @date 2023-11-01

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

#ifndef _GUY_IMAGE_H_FILE
#define _GUY_IMAGE_H_FILE

#include <Arduino.h>
#include <SPI.h>
#include <FS.h>

#define LGFX_USE_V1
#include <LovyanGFX.hpp>
#include "readguy.h"


class readguyImage{
  public:
    readguyImage(ReadguyDriver &_guy):guy(&_guy){}

    /** @brief 显示图像. use16grey: 为true则使用16灰度,false则不用
     *  @note 需要提前设置绘制参数, 直接在类的外部 设置此类的成员函数即可.
     *        对于不会C++的朋友们, 可以看示例.
     *  @param baseFs, filename 文件系统和文件名. 必须指定.
     *  @param x, y 显示在屏幕的什么地方
     * 
     *  @param use16grey 0: 使用单色抖动 1:使用16灰度 2:使用16灰度但不抖动
     * */
    void drawImageFile(bool use16grey = 0);

    /** @brief 将图像绘制到系统缓存内. 必须预分配内存, 并且需要知道分配的大小.
     *  @note 需要提前设置绘制参数, 直接在类的外部 设置此类的成员函数即可.
     *        对于不会C++的朋友们, 可以看示例.
     *  @param baseFs, filename 文件系统和文件名. 必须指定.
     *  @param x, y 显示在屏幕缓存的什么地方
     *  @param w, h 开辟的缓存宽度和高度. 最好是和图片的大小相匹配.
     *  @param exPool 外部内存位置, 必须指定. 此处的pool为1字节=1像素(8bit灰度)
     *  @param exPoolSize 外部内存大小, 必须指定. 太小会无法显示.
     *  @return 0:正常显示, 1:文件打不开 2:文件格式不支持 3:内存不足 4:其他问题 */
    uint8_t drawImageToBuffer();

    /// @brief 获取文件的扩展名.
    static uint8_t getExName(const char* fname, char* exname, size_t exname_len);

    fs::FS *baseFs = nullptr;///     // /  //要绘制的图片所属的文件系统
    const char *filename = nullptr; // /  //要绘制的图片的文件名和文件路径
    int32_t x = 0;        ////     // /  //绘制位置坐标X
    int32_t y = 0;       ////     // /  //绘制位置坐标Y
    int32_t w = 0x7fffffff;      // /  //绘制图片的最大宽度, 无需更改
    int32_t h = 0x7fffffff;     // /  //绘制图片的最大高度, 无需更改
    int32_t offsetx = 0;       // /  //从图片的哪个坐标点开始绘制图片,一般无需更改
    int32_t offsety = 0;      // /  //从图片的哪个坐标点开始绘制图片, 一般无需更改
    float scalex = 1.0f;     // /  //横坐标缩放水平. >1为放大, <1为缩小
    float scaley = 0.0f;    // /  //纵坐标缩放水平. 0代表与scalex相同.
    lgfx::v1::textdatum::textdatum_t datum=lgfx::v1::textdatum::top_left;
    uint8_t background = 1; //  //背景颜色. 0黑 1白, 默认白色.
    uint8_t enableFloyd = 1;   //是否开启floyd抖动算法. (抗锯齿) 默认开启
    uint8_t *exPool = nullptr;//额外的内存池, 外部缓存
    uint32_t exPoolSize = 0; //额外的内存池 外部缓存的字节数
  private:              //  //私有部分. 以下函数和成员只能是类内成员能调用
    ReadguyDriver *guy;//  //readguy类型指针. 用于沟通硬件.

    /// @brief 设置绘制函数. 此函数不建议单独调用.
    uint8_t drawImgHandler(int r, LGFX_Sprite *spr);

    uint8_t format = 0; //图片格式
    int32_t _h = 0; //内存池的像素高度 可能是内部内存池也可能是外部的
    //分多次完成显示, 次数为 h/_h.
    uint8_t *_pool = nullptr; //内存池, 可能是内部内存池也可能是外部的
    int16_t *floyd = nullptr; //用于显示图片
    uint8_t *readBuff = nullptr;
    uint8_t *writeBuff = nullptr;
};

#endif /* END OF FILE. ReadGuy project.
Copyright (C) 2023 FriendshipEnder. */
