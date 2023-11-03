/******************** F r i e n d s h i p E n d e r ********************
 * 本程序隶属于 Readguy 开源项目, 请尊重开源开发者, 也就是我FriendshipEnder.
 * 如果有条件请到 extra/artset/reward 中扫描打赏,否则请在 Bilibili 上支持我.
 * 项目交流QQ群: 926824162 (萌新可以进来问问题的哟)
 * 郑重声明: 未经授权还请不要商用本开源项目编译出的程序.
 * 
 * @file ex06_Image.ino
 * @author FriendshipEnder (f_ender@163.com), Bilibili: FriendshipEnder
 * @version 1.0
 * @date 2023-11-01
 * @brief ReadGuy 图片显示功能演示.
 * 1. 在运行过ex01或者ex02的开发板上 编译烧录本程序.
 * 2. 将该项目data文件夹内的所有文件放置于SD卡的根目录上.
 * 
 * @note 重要消息: 这是一个实验性功能. 可能你所使用的LGFX库版本较旧而无法通过编译.
 * 如果你的项目中无法成功编译源码中的setBuffer, 请更改LovyanGFX库的函数!
 * 位于文件 LovyanGFX/src/lgfx/v1/LGFX_Sprite.hpp
 * 第155行 void setBuffer 函数:
 * 修改为如下内容并保存
 * 
   ``` C++
    void setBuffer(void* buffer, int32_t w, int32_t h, color_depth_t bpp = rgb565_2Byte)
    {
      deleteSprite();
      if (bpp != 0) {
        _write_conv.setColorDepth(bpp);
        _read_conv = _write_conv;
        _panel_sprite.setColorDepth(bpp);
      }

      _panel_sprite.setBuffer(buffer, w, h, &_write_conv);
      _img = _panel_sprite.getBuffer();

      _sw = w;
      _clip_r = w - 1;
      _xpivot = w >> 1;

      _sh = h;
      _clip_b = h - 1;
      _ypivot = h >> 1;
    }
   ```
 * 完成后请再次尝试编译
 * [已经向lovyan03/LovyanGFX发布issue, 等待解决]
 * 
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

//在这里包含程序需要用到的库函数
#include <Arduino.h>    //arduino功能基础库. 在platformIO平台上此语句不可或缺
#include "readguy.h" //包含readguy_driver 基础驱动库
#include "guy_image.h" //包含 readguy_image 图片功能库. 此库文件放在项目文件夹内.

constexpr const char * BMP_FILE = "/bmp.bmp"; //在此更改你的图片文件路径名
constexpr const char * JPG_FILE = "/jpg.jpg"; //在此更改你的图片文件路径名
constexpr const char * PNG_FILE = "/png.png"; //在此更改你的图片文件路径名

ReadguyDriver guy;//新建一个readguy对象, 用于显示驱动.

void setup(){

// 1 - 初始化和启动ReadGuy                         --------------------- 1 - 初始化和启动ReadGuy -------<
  Serial.begin(115200); //初始化串口

  guy.init(); //初始化readguy_driver 基础驱动库.

// 2 - 显示图片 方式1    --------------------- 2 - 显示图片 方式1:使用readguy提供的drawImage方法. ------<<

  //此功能可以在Sprite绘制图片, 再使用内置的drawImage方法来显示出这个sprite.
  //此方法消耗的内存颇多, 需要整个sprite才能成功绘制.
  guy.fillScreen(1);  //清屏
  guy.display(); //显示白屏,用于将来显示图片.
  LGFX_Sprite sp(&guy); //创建一个Sprite (可以存储一些像素, 快速读写)
  sp.setColorDepth(lgfx::v1::color_depth_t::grayscale_8bit); //设置sprite的色彩深度
  sp.createSprite(80,80); //从文件创建BMP图像信息.
  sp.drawBmpFile(guy.guyFS(),BMP_FILE,0,0); //显示JPG文件
  Serial.printf("[%lu] sp.w: %d, h: %d, res: %im.\n",millis(),sp.width(),sp.height(),ESP.getFreeHeap());
  guy.drawImage(sp,9,9); //使用抖动像素的方式显示图片(不是灰度, 只有黑点和白点的那种显示效果)
  Serial.printf("[%lu] drawn dithering bmp.\n",millis()); //显示信息
  guy.display(); //1.2.0版本之后, drawImage不再刷屏, 需要额外调用display函数刷屏.
  delay(2000);

  //0-关闭连续刷屏 开启16阶灰度抖动
  guy.setGreyQuality(0); //部分屏幕允许使用不同的灰度显示模式.
  guy.draw16grey(sp,9,9); //使用16级灰度的方式显示图片 需要的时间比较长
  delay(2000);

  //1-开启连续刷屏 开启16阶灰度抖动 (默认值)
  guy.setGreyQuality(1); //部分屏幕允许使用不同的灰度显示模式.
  guy.draw16grey(sp,9,9); //使用16级灰度的方式显示图片 需要的时间比较长
  delay(2000);
  
  //2-关闭连续刷屏 关闭16阶灰度抖动
  guy.setGreyQuality(2); //部分屏幕允许使用不同的灰度显示模式.
  guy.draw16grey(sp,9,9); //使用16级灰度的方式显示图片 需要的时间比较长
  delay(2000);
  
  //3-开启连续刷屏 关闭16阶灰度抖动
  guy.setGreyQuality(3); //部分屏幕允许使用不同的灰度显示模式.
  guy.draw16grey(sp,9,9); //使用16级灰度的方式显示图片 需要的时间比较长
  delay(2000);

  sp.deleteSprite(); //关闭图片文件, 释放图片占用的大量内存
  Serial.printf("[%lu] drawn 16-layer greyscale bmp.\n",millis()); //显示信息
  delay(2000);


// 3 - 显示图片 方式2                --------------------- 3 - 显示图片 方式2:使用readguyImage类. -----<<<

  //readguy驱动程序的内部有一个屏幕缓存, 可以用于存储显示引擎显示的内容, 在刷屏时 直接发送此缓存的内容.
  //显示引擎会通过显示函数, 来改写屏幕缓存的内容. (显示就是这样显示的. 类似于print, drawPixel, fillRect...)

  //此绘制方式可以做到几乎不消耗额外的内存, 仅需要屏幕缓存即可显示图片.
  //适合用在esp8266或者消耗的内存较多的项目上.
  //显示原理比较复杂, 因为显示函数的限制, 图片不能直接显示到屏幕缓存中.
  //大体上说就是用屏幕缓存作为显示图片的中间内存, 再分阶段解码图片, 最后通过自定义刷屏函数, 完成刷新图片
  //使用此方法, 因为屏幕缓存被复用作图片解码中间内存, 屏幕缓存上原本的内容会因为图片解码而发生改变.

  //不需要指定额外的外部内存.

  //显示BMP格式
  readguyImage im(guy);          //定义一个绘制器, 此类中的函数用于绘制图片.
                                 //所有的绘制图片的参数均需要放入此结构内
                                 //直接更改im内的数据即可设置绘制参数. (就像结构体一样用它)
  im.baseFs=&guy.guyFS();        //在此处就是设置文件系统.

  im.filename=BMP_FILE;          //在此直接设置文件路径和文件名.

  guy.display(FILL_WHITE,false); //将屏幕全刷成白屏. 为了即将的图片刷新.
  //建议在使用drawImageFile函数之前, 使用慢刷刷白屏, 可以保证显示效果清晰可见.

  im.drawImageFile();            //显示BMP格式.图片. im会自动识别文件扩展名并绘制.
  //此处不再需要调用 guy.display() 函数即可显示.
  //此时显示缓存已经清空(因为显示缓存内原本是用来解码图片用的)
  //此时直接使用guy.display()将刷白屏.

  delay(2000);


  //显示JPG格式
  im.filename=JPG_FILE;          //在此直接设置文件路径和文件名.

  im.scalex=0.3333333f;          //设置横坐标图片缩放
  im.scaley=0.3333333f;          //设置纵坐标图片缩放.
  im.offsetx=70;                 //设置显示偏移. 此处的70说明 JPG图像应该从图片文件的(70,40)坐标处开始解码
  im.offsety=40;                 //设置显示偏移Y轴方向.

  im.background=0;               //设置背景颜色, 0黑1白, 此处设为背景色为黑色.

  guy.display(FILL_WHITE,false); //将屏幕全刷成白屏. 为了即将的图片刷新.

  im.drawImageFile();            //显示JPG格式.图片. im会自动识别文件扩展名并绘制.
  delay(2000);

  
#ifndef ESP8266  //显示PNG格式. ESP8266不支持PNG格式
  im.filename=PNG_FILE;          //PNG图片. ESP8266的内存不够, 所以不能显示PNG格式~(T_T)~

  im.x=10;                       //设置在哪里绘制图片. 在此编辑图片绘制起始点的 X, Y 坐标
  im.y=5 ;                       //
  im.scalex=400.0f/1280.0f;
  im.scaley=300.0f/576.0f;

  guy.display(FILL_WHITE, false); //显示. 此处的功能就是将显示缓存输出到屏幕上

  im.drawImageFile();            //显示JPG格式.图片. ESP8266可能不会绘制.
  delay(2000);

#endif

  //开启灰度显示, 然后显示BMP格式
  im.filename=BMP_FILE;          //在此直接设置文件路径和文件名.

  im.scalex=0.75f;            //设置横坐标图片缩放
  im.scaley=0.5625f;          //设置纵坐标图片缩放.
  im.offsetx=0;                 //设置显示偏移. 此处的70说明 JPG图像应该从图片文件的(70,40)坐标处开始解码
  im.offsety=0;                 //设置显示偏移Y轴方向.

  im.background=1;               //设置背景颜色, 0黑1白, 此处设为背景色为黑色.

#ifdef ESP8266
#define MEM_POOL 10000
#else
#define MEM_POOL 60000
#endif

  uint8_t *buffer = new uint8_t [ MEM_POOL ]; //分配缓存
  im.exPool=buffer;     //设置外部缓存内存地址
  im.exPoolSize=MEM_POOL; //设置外部缓存内存大小
  
  guy.setGreyQuality(1);         //设置灰度模式为默认灰度显示模式
  guy.display(FILL_WHITE,false); //将屏幕全刷成白屏. 为了即将显示灰度图.

  im.enableFloyd=0; //禁用掉抖动算法.

  im.drawImageFile(1);            //显示JPG格式.图片.

  delay(2000);

  guy.display(FILL_WHITE,false); //将屏幕全刷成白屏. 为了即将显示灰度图.

  im.enableFloyd=1; // 重新启用抖动算法.

  im.drawImageFile(1);            //显示JPG格式.图片.

  delay(5000);

  delete []buffer;
  im.exPool=nullptr;     //用完之后将这个内存地址置为null, 避免重复使用导致内存不可访问.
  im.exPoolSize=0;



/*
// 灰度测试代码 -------------------------------------------------------------------------- 灰度测试代码

  im.filename=BMP_FILE;          //在此直接设置文件路径和文件名.

  guy.setGreyQuality(0);

  guy.display(FILL_WHITE, false);

#ifdef ESP8266
#define MEM_POOL 10000
#else
#define MEM_POOL 60000
#endif
  uint8_t *buffer = new uint8_t [ MEM_POOL ]; //分配缓存
  im.exPool=buffer;     //设置外部缓存内存地址
  im.exPoolSize=MEM_POOL; //设置外部缓存内存大小
  
  im.enableFloyd = 0;
  im.drawImageFile(true);            //显示BMP格式.图片. im会自动识别文件扩展名并绘制.
  //此处不再需要调用 guy.display() 函数即可显示.
  //此时显示缓存已经清空(因为显示缓存内原本是用来解码图片用的)
  //此时直接使用guy.display()将刷白屏.

  delay(3000);

  guy.display(FILL_WHITE,false);

  im.enableFloyd = 1;
  guy.setGreyQuality(1);
  im.drawImageFile(true);            //显示BMP格式.图片. im会自动识别文件扩展名并绘制.

  delay(3000);

  //显示JPG格式
  im.filename=JPG_FILE;          //在此直接设置文件路径和文件名.

  im.scalex=1.3333333f;
  im.scaley=1.3333333f;

  //im.scalex=0.3333333f;          //设置横坐标图片缩放
  //im.scaley=0.3333333f;          //设置纵坐标图片缩放.
  //im.offsetx=70;                 //设置显示偏移. 此处的70说明 JPG图像应该从图片文件的(70,40)坐标处开始解码
  //im.offsety=40;                 //设置显示偏移Y轴方向.

  im.background=0;               //设置背景颜色, 0黑1白, 此处设为背景色为黑色.

  guy.display(FILL_WHITE, false);

  im.drawImageFile();            //显示JPG格式.图片. im会自动识别文件扩展名并绘制.
  delete []buffer;

// 灰度测试代码 -------------------------------------------------------------------------- 灰度测试代码
*/






//        --------------------- 4 - 显示图片 使用readguyImage类的externalBuffer绘制到主屏幕缓存. -----<<<

  //实现原理: 通过一个额外的缓存, 显示引擎可以分多次将图片显示在这个额外的内存中, 再将这块内存写入到缓存内.
  //由于用了外部内存, 所以可以分多次显示一张大图片, 相比于分配整张大图片的内存, 要更节省内存.代价是速度更慢

  //readguy驱动程序的内部有一个屏幕缓存, 如果使用这个缓存来显示图片, 屏幕上原本的东西也会变得混乱.(上个示例)
  //因此, readguyImage类提供了使用外部内存的显示功能.
  //分为多次, 每次将图片的一小部分显示到外部内存中, 再写入到屏幕缓存内.

  //需要提供额外分配好的内存, 但此函数不会更改显示区域之外的显示缓存.

  guy.fillScreen(1);  //将屏幕全刷成白屏. 为了即将的图片刷新.

  for(int i=1;i<10;i++){ //在屏幕上绘画线段. 本质是更改显示缓存.
    guy.drawLine(i*guy.width()/10,0,0,i*guy.height()/10,0);
    guy.drawLine(i*guy.width()/10,guy.height()-1,guy.width()-1,i*guy.height()/10,0);
  }
  guy.drawLine(guy.width(),0,0,guy.height(),0);

  guy.display(false); //刷新屏幕, 显示绘画的线段

  //im.baseFs=&guy.guyFS();        //直接更改im内的数据即可设置绘制参数. 在此处就是设置文件系统.
  im.filename=BMP_FILE;          //在此直接设置文件路径和文件名.
  im.background=0;               //设置背景颜色, 0黑1白, 此处设为背景色为黑色.
  
  im.x=15;           //设置将要显示的横坐标
  im.y=15;           //设置将要显示的纵坐标
  im.w=180;          //设置将要显示区域的宽度
  im.h=270;          //设置将要显示区域的高度
  im.offsetx=0;      //如果不从左上角开始解码图片, 从哪个坐标开始显示/解码图片?
  im.offsety=0;
  im.scalex=0.5625f; //设置横坐标缩放.
  im.scaley=0.5625f; //设置纵坐标缩放. 为0代表与横坐标缩放相同.

  buffer = new uint8_t [ 60*270 ]; //分配缓存
  im.exPool=buffer;     //设置外部缓存内存地址
  im.exPoolSize=60*270; //设置外部缓存内存大小

  im.drawImageToBuffer(); //将图片写入到屏幕缓存. im会自动识别文件扩展名并绘制.

  guy.display(); //显示缓存.此时应当能看到图片.

  delete []buffer;

}

void loop(){
  delay(1);
}/* END OF FILE. ReadGuy project.
Copyright (C) 2023 FriendshipEnder. */


