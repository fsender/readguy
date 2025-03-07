/******************** F r i e n d s h i p E n d e r ********************
 * 本程序隶属于 Readguy 开源项目, 请尊重开源开发者, 也就是我FriendshipEnder.
 * 如果有条件请到 extra/artset/reward 中扫描打赏,否则请在 Bilibili 上支持我.
 * 项目交流QQ群: 926824162 (萌新可以进来问问题的哟)
 * 郑重声明: 未经授权还请不要商用本开源项目编译出的程序.
 * 
 * @file ex06_Image.ino
 * @author FriendshipEnder (f_ender@163.com), Bilibili: FriendshipEnder
 * @version 1.0
 * @date create: 2023-11-01
 * last modify: 2024-12-04
 * 
 * @note 重要消息: 这是一个实验性功能. 可能你所使用的LGFX库版本较旧而无法通过编译.
 * 
 * (ESPxxxx系列可无视此行) 对于不支持fs::FS的设备 (如PC) 来说, 请前往 guy_image.h 文件并更改第34行的注释
 * 
 * 如果你的项目中无法成功编译源码中的setBuffer, 请更改LovyanGFX库的函数!
 * 位于文件 LovyanGFX/src/lgfx/v1/LGFX_Sprite.hpp
 * 第155行 void setBuffer 函数:
 * 添加为如下内容并保存 (不建议修改库里原有的函数, 保证库的兼容性)
 * 
   ``` C++
    void setBuffer(void* buffer, int32_t w, int32_t h, color_depth_t bpp)
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
 * @brief ReadGuy 图片显示功能演示.
 * - 演示如何将比较大的图片通过多种方法显示到屏幕上.
 * - 运行的会很缓慢, 因为示例的图片文件比较大.
 * 1. 在运行过ex01或者ex02的开发板上 编译烧录本程序.
 * 2. 将该项目data文件夹内的所有文件放置于SD卡的根目录上.
 * 就是SD卡内放data文件夹内的所有文件, 不能额外套文件夹.
 * 如果你的SD卡插入电脑上显示为可移动磁盘, 那么双击SD卡目录就要看到这个文件夹里的文件
 * 
 * 将根目录下的data文件夹 上传到LittleFS之后运行效果更佳
 * 或者可以准备一张SD卡,并准备在卡的根目录下放置data文件夹内的文件.
 * 就是SD卡内放data文件夹内的所有文件, 不能额外套文件夹.
 * 如果你的SD卡插入电脑上显示为可移动磁盘, 那么双击SD卡目录就要看到这个文件夹里的文件
 * 
 * 默认的文件系统为SD卡. 当没有插入SD卡时, 会读取LittleFS文件系统.
 * 没有条件准备SD卡的, 可以烧录LittleFS文件系统.
 * 
 * 对于ESP8266 Arduino 用户, 在项目草图文件夹内新建一个data文件夹, 并放入文件 (示例已提供data文件夹)
 * 再在 arduinoIDE 的工具选项里选择 ESP8266 LittleFS Data Upload.
 * 没有这个选项的需要参考以下文档安装ESP8266 Sketch upload tool
 * https://randomnerdtutorials.com/install-esp8266-nodemcu-littlefs-arduino/
 * 
 * 对于ESP32 Arduino 用户, 也要在项目草图文件夹内放一个data文件夹, 并把文件放入其中 (示例已提供data文件夹)
 * 再在 arduinoIDE 的工具选项里选择 ESP32 Sketch data upload, 最后选择LittleFS.
 * 没有这个选项的需要参考以下文档安装ESP32 LittleFS upload tool
 * https://randomnerdtutorials.com/esp32-littlefs-arduino-ide/
 * 
 * 对于PlatformIO 用户, 需要进入platformIO扩展界面, 选择Upload Filesystem Image, 上传项目文件.
 * ESP8266和ESP32都要用这种方法.
 * 
 * {0} 代码食用注意事项:
 * 这一部分的代码很难读, 或者按维莫斯小姐的说法, 很 "抽象" .
 * 如果你要开始一行一行读取下去, 不妨先在这里停顿一下, 看一下一些需要你提前知道的东西.
 * 
 * {1} 画布, 或者叫sprite. 是一块专门存储图像的数据结构.
 * 画布需要大量的内存来存放图像的像素, 但是你可以对画布进行任何图像操作, 就像是在墨水屏上能进行的操作一样多.
 * 画布类型的数据结构至少需要包含3个要素: 宽度, 高度, 每像素需要的字节数.
 * 画布需要的内存为 宽度*高度*每像素需要的字节数. (1bit图像需要8分之一字节)
 * 在画布上可以进行几乎任何图像操作, 就像是在墨水屏上能进行的操作一样多.
 * 而且你还可以获取像素的颜色 (这和你创建的画布的颜色格式有关).
 * 画布可以随时被快速的显示到墨水屏上, 显示完了之后再使用display方法, 你刚才创建的画布就显示出来了.
 * 
 * {2} 屏幕缓存.    你就从来没好奇过, 墨水屏上怎么呈现图像的? 我好奇过.
 * 我翻看了里面那些错综复杂又是深奥的C++骚操作又是类继承又是虚函数指针...反正就是抽象而且难懂的的源代码,
 * 简而言之, 墨水屏显示的原理就是墨水屏驱动里面也有个画布, 不过这个画布是1bit的(每8像素消耗1字节内存).
 * 所有对墨水屏进行的操作其实都是对那个内部的画布进行的操作.
 * 在你使用墨水屏的display方法时, 呃, 墨水屏的驱动程序可以读取到这个屏幕缓存的所有字节.
 * 并且根据驱动手册的方法将这个画布上的字节发送到屏幕上.
 * 发送完成之后再通过一些代码, 墨水屏就会刷屏, 将这个屏幕缓存里的内容显示出来了.
 * 维莫斯小姐: 苏滴嘶内~
 * 下面的库其实用了一些更加骚气的方法, 直接读取了这块画布, 从而能允许你在ESP8266上也可以...显示不小的图片
 * 
 * {3} 灰度显示. ReadGuy这个库的一大特色就是支持灰度显示.
 * 欸, 可是屏幕缓存画布只有1bit啊, 怎么显示灰度啊?
 * 不用急, 原理也不难.
 * 16级灰度颜色的刷屏分为15个步骤. 因为白色不需要刷.
 * 在开始刷屏之前, 务必进行一次慢刷, 确保所有的白色像素都足够白.
 * 第一次刷屏, 通常会刷最浅的灰色.
 * 此时, 首先将屏幕缓存内所有需要显示为这个颜色的像素全写入为黑色(只是名义上的黑色, 实际上是设定的颜色).
 * 再初始化墨水屏的刷屏功能.
 *  - 此处需要额外设定刷屏的颜色的参数. 此参数设定是可以由驱动程序自动完成的, 不过也可以通过你调用函数完成.
 *  - 用户只需要用简单的setDepth函数设置颜色即可. 用户设定的颜色深度通常用于显示字符串或者形状等GUI控件.
 *  - 刷图程序的灰度图片刷新功能会自动控制刷屏颜色, 不需要手动调用.
 * 最后刷屏. 因为之前刷图程序已经设定过刷的颜色了, 所以此时刷屏呈现的颜色就是之前设定的颜色.
 * 第2-15次刷屏原理类似.
 * 每次追加黑色像素, 追加的像素就是来显示灰度的像素. 而原本的黑色像素就是之前那些灰度颜色的像素.
 * 
 * \*(^_^)*/   /** 我相信你应该还不懂. 我就举个例子吧.
 * 比如要显示一个渐变色, 从左到右为白色→黑色的渐变图.
 * 首先进行一次慢刷, 确保要刷图的地方都是纯洁的白色.
 * ≡=-(1)-=≡
 * 筛选出最浅的灰色设为灰1, (灰X代表灰度颜色, 灰15代表黑色)
 * 再把图片内所有颜色和灰1最接近的颜色写入到屏幕缓存内.
 * 此时屏幕缓存内的"黑色"部分都是与灰1最接近的颜色, 其他颜色均为"白色"部分.
 * 刷好了之后, 进行一次刷屏. (系统自动调用setDepth(1)来保证接下来图片的黑色部分其实是灰1).
 * ≡=-(2)-=≡
 * 筛选出比最浅的灰色深一点的颜色设为灰2, 把图片内和灰2最接近的颜色写入屏幕缓存, 但保持原本代表灰1的"黑色"不变.
 * (也就是说, 把灰2的像素写入之后, 屏幕缓存的黑色像素只多不少. 写入过程中, 只能是白→黑, 不存在黑→白)
 * 此时屏幕缓存内的"黑色"部分包含了与灰2最接近的颜色, 和刚才刷完的与灰1最接近的颜色. 其他颜色均为"白色"部分.
 * 刷好了之后, 进行一次刷屏. (系统自动调用setDepth(2)来保证接下来图片的黑色部分其实是灰2)
 * 注意屏幕显示, 原本的灰1并没有颜色变化, 变化的都是本次追加的"灰2"代表的"黑色".
 * 追加的像素会显示为新设定的颜色.
 * ≡=-(3)-=≡
 * 对于灰3, 和灰2一样, 追加显示到屏幕缓存内.
 * (也就是说, 屏幕缓存的黑色像素还是只多不少.    写入过程中, 只能是白→黑, 不存在黑→白)
 * 此时屏幕缓存内的"黑色"部分包含了与灰3, 灰2和灰1最接近的颜色. 其他颜色均为"白色"部分.
 * 设置颜色深度, 刷屏.
 * 对于灰4~灰15, 都这么做
 * ≡=-(5)-=≡
 * 刷完灰15之后, 灰度图就刷完了.
 * 
 * {4} 自定义刷屏函数. 到底是什么决定了刷屏时的每一像素是什么颜色? 是黑是白?
 * 其实刷屏的本质就是通过调用一个外部函数.
 * 该函数可以根据输入的像素坐标位置来决定输出的像素颜色.
 * 为了简化程序调用过程并提高调用速度, 此处的像素坐标位置参数为一个整数(而不是两个)
 * 至于该怎么调用这个函数, 并不是你需要了解的事情.
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

  guy.setRotation(3); //设置旋转方向. 因为此库的功能不随旋转而发生变化.

// 2 - 显示图片 方式1    --------------------- 2 - 显示图片 方式1:使用readguy提供的drawImage方法. ------<<

  //此功能可以在 画布{1}(Sprite) 绘制图片, 再使用内置的drawImage方法来显示出这个sprite.
  //此方法消耗的内存颇多, 需要为整个sprite分配所有内存才能成功绘制.
  //由于目前版本LovyanGFX的限制, 不能分配4bit的画布, 只能分配大小至少为8bit的画布.

  guy.fillScreen(1);  //清屏
  //guy.display(); //显示白屏,用于将来显示图片.
  LGFX_Sprite sp(&guy); //创建一个 画布(Sprite) (可以存储一些像素, 快速读写)
  sp.setColorDepth(lgfx::v1::color_depth_t::grayscale_8bit); //设置画布sprite的色彩深度
  sp.createSprite(80,80); //为刚刚创建的画布对象分配内存 (像素宽度*像素高度). 此处为80(宽)*80(高)像素
  sp.drawBmpFile(guy.guyFS(),BMP_FILE,0,0); //从文件创建BMP图像信息. 文件系统内必须要有这个BMP图像.
  //默认的文件系统为SD卡. 当没有插入SD卡时, 会读取LittleFS文件系统.
  //没有条件准备文件系统的, 可以烧录文件系统.
  Serial.printf("[%lu] sp.w: %d, h: %d, res: %im.\n",millis(),sp.width(),sp.height(),ESP.getFreeHeap());
  guy.drawImage(sp,9,11); //使用抖动像素的方式显示图片(不是灰度, 只有黑点和白点的那种显示效果)
  //sp为要显示的画布, 后面的9 11都是要显示的目标坐标位置. 此处代表将画布显示在 屏幕缓存{2} 的第9列, 第11行.
  Serial.printf("[%lu] drawn dithering bmp.\n",millis()); //显示信息
  guy.display(); //1.2.0版本之后, drawImage不再刷屏, 需要额外调用display函数刷屏.
  delay(2000);


  guy.display(FILL_WHITE,READGUY_FAST); //在保持屏幕缓存不变的时候快速刷新白屏.
  guy.drawImage(sp,0,0,guy.width(),guy.height()); //绘画的画布可以被放大或者缩小到任意宽度和高度.
  //此处的参数调用表示将会在屏幕坐标(0,0)开始显示, 显示的画布宽度缩放到屏幕宽度, 画布高度缩放到屏幕高度.
  guy.display(); //调用display函数刷屏.
  delay(2000);

  guy.display(FILL_WHITE,READGUY_FAST); //在保持屏幕缓存不变的时候快速刷新白屏
  guy.fillScreen(1); //白屏清屏(清屏幕缓存)
  guy.drawImage(sp,10,10,65,50); //缩放: 缩小到65X50
  guy.display(); //调用display函数刷屏.
  delay(2000);


  //以下函数可以允许你用灰度显示画布.
  //在刷灰度之前, 该函数可以对刷屏区域自动慢刷白屏.
  //0-关闭连续刷屏 开启16阶灰度抖动. 连续刷屏并非所有的屏幕都支持. 所以此处使用了一个函数检验.
  if(guy.supportGreyscaling()==-16){ //此函数返回-16代表支持连续刷屏. 返回16代表支持灰度, 返回0代表仅黑白.
    guy.fillScreen(1); //白屏清屏(清屏幕缓存)
    guy.setGreyQuality(0); //设置刷屏方式:0-关闭连续刷屏, 开启16阶灰度抖动.
    guy.draw16grey(sp,9,9); //使用16级灰度的方式显示图片 需要的时间比较长
    delay(2000);
  }

  //1-开启连续刷屏 开启16阶灰度抖动 (默认值)
  guy.fillScreen(1); //白屏清屏(清屏幕缓存)
  guy.setGreyQuality(1); //设置刷屏方式:1-开启连续刷屏, 开启16阶灰度抖动.    在初始化时, 这是默认值.
  guy.draw16grey(sp,12,12,120,120); //使用16级灰度的方式显示图片. 后两个参数120代表缩放之后的宽度高度
  delay(2000);
  
  //2-关闭连续刷屏 关闭16阶灰度抖动. 连续刷屏并非所有的屏幕都支持. 所以此处使用了一个函数检验.
  if(guy.supportGreyscaling()==-16){ //此函数返回-16代表支持连续刷屏. 返回16代表支持灰度, 返回0代表仅黑白.
    guy.fillScreen(1); //白屏清屏(清屏幕缓存)
    guy.setGreyQuality(2); //2-关闭连续刷屏, 关闭16阶灰度抖动
    guy.draw16grey(sp,9,9,50,100); //使用16级灰度的方式显示图片. 参数50和100分别代表宽度缩放和高度缩放
    delay(2000);
  }
  
  //3-开启连续刷屏 关闭16阶灰度抖动
  guy.fillScreen(1); //白屏清屏(清屏幕缓存)
  guy.setGreyQuality(3); //3-打开连续刷屏, 关闭16阶灰度抖动
  guy.draw16grey(sp,3,3,100,50); //使用16级灰度的方式显示图片 参数100和50分别代表宽度缩放和高度缩放
  delay(2000);

  //灰度显示画布到此显示完了
  sp.deleteSprite(); //关闭画布, 释放图片占用的大量内存
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
#ifdef FS_POINTER                //对于不支持fs::FS的设备来说, 请前往 guy_image.h 文件并更改第34行的注释
  im.baseFs=&guy.guyFS();        //在此处就是设置文件系统.
#endif

  im.filename=BMP_FILE;          //在此直接设置文件路径和文件名.

  guy.display(FILL_WHITE,READGUY_SLOW); //将屏幕全刷成白屏. 为了即将的图片刷新.
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

  guy.display(FILL_WHITE,READGUY_SLOW); //将屏幕全刷成白屏. 为了即将的图片刷新.

  im.drawImageFile();            //显示JPG格式.图片. im会自动识别文件扩展名并绘制.
  delay(2000);

  
#ifndef ESP8266                  //显示PNG格式. ESP8266不支持PNG格式.
  im.filename=PNG_FILE;          //PNG图片. ESP8266的内存不够, 所以不能显示PNG格式~(T_T)~

  im.x=10;                       //设置在哪里绘制图片. 在此编辑图片绘制起始点的 X, Y 坐标
  im.y=5 ;                       //
  im.scalex=400.0f/1280.0f;
  im.scaley=300.0f/576.0f;

  guy.display(FILL_WHITE, READGUY_SLOW);//显示. 此处的功能就是将显示缓存输出到屏幕上

  im.drawImageFile();            //显示PNG格式.图片. ESP8266可能不会绘制.
  delay(2000);

#endif

  //开启灰度显示, 然后显示BMP格式
  im.filename=BMP_FILE;          //在此直接设置文件路径和文件名.

  im.scalex=0.75f;               //设置横坐标图片缩放
  im.scaley=0.5625f;             //设置纵坐标图片缩放.
  im.offsetx=0;                  //设置显示偏移. 此处的70说明 JPG图像应该从图片文件的(70,40)坐标处开始解码
  im.offsety=0;                  //设置显示偏移Y轴方向.

  im.background=1;               //设置背景颜色, 0黑1白, 此处设为背景色为黑色.

  //预分配内存, 可以用作显示缓存.
#ifdef ESP8266
#define MEM_POOL 10000
#else
#define MEM_POOL 60000
#endif

  uint8_t *buf = new uint8_t [MEM_POOL];  //分配缓存
  im.exPool=buf;                          //设置外部缓存内存地址
  im.exPoolSize=MEM_POOL;                 //设置外部缓存内存大小
  
  guy.setGreyQuality(1);                  //设置灰度模式为默认灰度显示模式
  guy.display(FILL_WHITE,READGUY_SLOW);   //将屏幕全刷成白屏. 为了即将显示灰度图.

  im.enableFloyd=0;                       //禁用掉抖动算法.

  im.drawImageFile(1);                    //显示JPG格式.图片.

  delay(2000);

  guy.display(FILL_WHITE,READGUY_SLOW);   //将屏幕全刷成白屏. 为了即将显示灰度图.

  im.enableFloyd=1;                       // 重新启用抖动算法.

  im.drawImageFile(1);                    //显示JPG格式.图片.

  delay(5000);

  delete [] buf;                          //缓存用完了, 释放内存.
  im.exPool=nullptr;                      //用完之后将这个内存地址置为null, 避免重复使用导致内存不可访问.
  im.exPoolSize=0;





//        --------------------- 4 - 显示图片 使用readguyImage类的externalBuffer绘制到主屏幕缓存. -----<<<

  //实现原理: 通过一个额外的缓存, 显示引擎可以分多次将图片显示在这个额外的内存中, 再将这块内存写入到缓存内.
  //由于用了外部内存, 所以可以分多次显示一张大图片, 相比于分配整张大图片的内存, 要更节省内存.代价是速度更慢

  //readguy驱动程序的内部有一个屏幕缓存, 如果使用这个缓存来显示图片, 屏幕上原本的东西也会变得混乱.(上个示例)
  //因此, readguyImage类提供了使用外部内存的显示功能.
  //分为多次, 每次将图片的一小部分显示到外部内存中, 再写入到屏幕缓存内.

  //需要提供额外分配好的内存, 但此函数不会更改显示区域之外的显示缓存.

  guy.fillScreen(1);                //将屏幕全刷成白屏. 为了即将的图片刷新.

  for(int i=1;i<10;i++){            //在屏幕上绘画线段. 本质是更改显示缓存.
    guy.drawLine(i*guy.width()/10,0,0,i*guy.height()/10,0);
    guy.drawLine(i*guy.width()/10,guy.height()-1,guy.width()-1,i*guy.height()/10,0);
  }
  guy.drawLine(guy.width(),0,0,guy.height(),0);

  guy.display(READGUY_SLOW);        //刷新屏幕, 显示绘画的线段

  //im.baseFs=&guy.guyFS();         //直接更改im内的数据即可设置绘制参数. 在此处就是设置文件系统.
  im.filename=BMP_FILE;             //在此直接设置文件路径和文件名.
  im.background=0;                  //设置背景颜色, 0黑1白, 此处设为背景色为黑色.
  
  im.x=15;                          //设置将要显示的横坐标
  im.y=15;                          //设置将要显示的纵坐标
  im.w=180;                         //设置将要显示区域的宽度
  im.h=270;                         //设置将要显示区域的高度
  im.offsetx=0;                     //如果不从左上角开始解码图片, 从哪个坐标开始显示/解码图片?
  im.offsety=0;
  im.scalex=0.5625f;                //设置横坐标缩放.
  im.scaley=0.5625f;                //设置纵坐标缩放. 为0代表与横坐标缩放相同.

  buf = new uint8_t [ 60*270 ];     //分配缓存
  im.exPool=buf;                    //设置外部缓存内存地址
  im.exPoolSize=60*270;             //设置外部缓存内存大小

  im.drawImageToBuffer();           //将图片写入到屏幕缓存. im会自动识别文件扩展名并绘制.

  guy.display();                    //显示缓存.此时应当能看到图片.

  delete [] buf;                    //缓存用完了, 释放内存.
  im.exPool=nullptr;                //用完之后将这个内存地址置为null, 避免重复使用导致内存不可访问.
  im.exPoolSize=0;

}

void loop(){
  delay(1);
}/* END OF FILE. ReadGuy project.
Copyright (C) 2023 FriendshipEnder. */


