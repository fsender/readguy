/******************** F r i e n d s h i p E n d e r ********************
 * 本程序隶属于 Readguy 开源项目, 请尊重开源开发者, 也就是我FriendshipEnder.
 * 如果有条件请到 extra/artset/reward 中扫描打赏,否则请在 Bilibili 上支持我.
 * 项目交流QQ群: 926824162 (萌新可以进来问问题的哟)
 * 郑重声明: 未经授权还请不要商用本开源项目编译出的程序.
 * 
 * @file ex01_helloWorld.ino
 * @author FriendshipEnder (f_ender@163.com), Bilibili: FriendshipEnder
 * @version 1.1
 * @date create: 2023-09-19
 * last modify: 2023-11-06
 * @brief ReadGuy最基础的HelloWorld显示.
 * 
 * @note 食用方法:
 * 首先, 你需要有一颗ESP8266或者ESP32开发板, 一个墨水屏, 以及1~3个按钮.
 * 如果有一张SD卡或者是屏幕前置光, 那就更好啦, 哈哈哈哈~
 * 不过如果你有一个成品阅读器, 那你可以社牛的问一下硬件的作者(一般在oshwhub能找到)
 * 问到哪个引脚对应哪个GPIO.
 * 其实看原理图就可以知道哪个GPIO连接到哪个外设了.
 * 
 * 其次, 你需要把开发板连接到电脑, 烧录这个程序. 克隆这个项目之后, 在platformIO里打开,
 * 直接编译! 上传! (用arduino IDE的, 把lib文件夹内的三个子文件夹拷贝到你的arduino库
 * 目录里, 一般在C:\Users\$username$\OneDrive\文档\Arduino\libraries)
 * 不成功的和不会的, 建议再看一遍ESP8266和ESP32项目的构建流程.
 * 
 * 烧录完成之后, 用手机连接设备的WiFi网络 "readguy". 密码12345678. 
 * 连接上之后访问 http://192.168.4.1 最后根据网页的提示操作就可以
 * 你需要知道你的哪个引脚对应哪个GPIO, 才能使用这个库 (带来的不便请谅解nia~)
 * 配置好了, 代码就开始正常运行了.
 * 
 * 以下是看不懂代码的解决方法. 仔细看就行了
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
 * 关于图形函数:
 * 用过 LovyanGFX / Adafruit GFX / GxEPD2 / TFT_eSPI 库的用户, 可以直接套用他们的接口函数
 * 用过 U8G2 的用户, 接口函数和U8G2不一样的,
 * 请参考上述库的示例程序.
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
//就像是你在C语言里面总是用的 #include <stdio.h> 一样,
//为了能在这里使用readguy库, 你需要在这里调用 #include "readguy.h" 来包含此库
//在platformio中, 还要额外 #include <Arduino.h> 来确保你用的是Arduino环境.

#include <Arduino.h>    //arduino功能基础库. 在platformIO平台上此语句不可或缺
#include "readguy.h"    //包含readguy_driver 基础驱动库

ReadguyDriver guy;//新建一个readguy对象, 用于显示驱动.
//所有对墨水屏的操作都是对guy这个对象进行的操作.

void setup(){ //Arduino的setup函数. 这个函数在上电之后仅执行一次.

//                                           --------------------- 1 - 初始化和启动ReadGuy -------<
  Serial.begin(115200); //初始化串口

  //注意: 首次烧录此程序时, 这一步需要你打开手机联esp8266/32的 WiFi, 用于配网.
  //名称是 readguy 密码 12345678. 连接后访问 192.168.4.1 再在网页中完成后续步骤
  //后续启动就可以不用这个配置了
  guy.init(); //初始化readguy_driver 基础驱动库.

  //首次初始化完成之后, 以后再初始化就不需要配网了, 除非你抹除了芯片的flash
  //完成之后会让下一次刷屏 全屏慢速刷新一次, 之后的刷屏即可自由定义是全刷还是局刷.

  guy.setFont(&FreeMonoBold9pt7b);  //设置显示的字体
  //字体可以参考LovyanGFX的示例程序.

  guy.setTextColor(0,1);  //设置显示的颜色. 0代表黑色, 1代表白色
  //此函数的作用是设置显示颜色. 左边的0代表前景色(文字颜色: 黑色), 右边的1代表背景色(白色)
  //类似于 guy.setTextColor(0) 的用法说明此颜色为透明背景.

  guy.drawString("Hello Readguy!",10,10); //用此函数将字符串显示到屏幕缓存内
  //调用此函数并不会立即刷屏, 而是会将文本字符串写入到屏幕缓存, 在下一次调用display()函数之后就会显示出来.

  //也可以用print函数来显示.
  //guy.setCursor(10,10);        //设置显示的坐标
  //guy.print("Hello Readguy!"); //使用这个函数也能显示出字符串, 但是需要提前使用setCursor确定显示坐标

  guy.display(true); // 快速刷新. 将屏幕缓存内的内容显示到墨水屏幕上. 可简写为 guy.display(), 效果一样.
  //guy.display(false); // 慢速刷新. 

  //想知道更多内容, 欢迎移步到其他示例.
}

void loop(){
  
  //什么也不做, 毕竟刷新墨水屏要消耗墨水屏的阳寿.
  //盖姐说它们也是有阳寿的. 刷多了会老化.
  delay(1);

}/* END OF FILE. ReadGuy project.
Copyright (C) 2023 FriendshipEnder. */