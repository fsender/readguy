/******************** F r i e n d s h i p E n d e r ********************
 * 本程序隶属于 Readguy 开源项目, 请尊重开源开发者, 也就是我FriendshipEnder.
 * 如果有条件请到 extra/artset/reward 中扫描打赏,否则请在 Bilibili 上支持我.
 * 项目交流QQ群: 926824162 (萌新可以进来问问题的哟)
 * 郑重声明: 未经授权还请不要商用本开源项目编译出的程序.
 * 
 * @file ex02_demo.ino
 * @author FriendshipEnder (f_ender@163.com), Bilibili: FriendshipEnder
 * @version 1.0
 * @date 2023-09-19
 * @brief ReadGuy功能演示.
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
 * 用于演示BMP格式图片灰度显示.
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

ReadguyDriver guy;//新建一个readguy对象, 用于显示驱动.

void drawLines(); //声明一个函数, 用于显示一些线条. 此函数在后面的程序中会用到的

void setup(){

//                                           --------------------- 1 - 初始化和启动ReadGuy -------<
  Serial.begin(115200); //初始化串口

  //注意: 首次烧录此程序时, 这一步需要你打开手机联esp8266/32的 WiFi, 用于配网.
  //名称是 readguy 密码 12345678. 连接后访问 192.168.4.1 再在网页中完成后续步骤
  //后续启动就可以不用这个配置了
  guy.init(); //初始化readguy_driver 基础驱动库.

  //首次初始化完成之后, 以后再初始化就不需要配网了, 除非你抹除了芯片的flash

//                                          ------------------- 2 - 使用ReadGuy来显示字符串 ------<<

  guy.setFont(&FreeMonoBold9pt7b);  //设置显示的字体

  guy.setTextColor(0,1);  //设置显示的颜色. 0代表黑色, 1代表白色

  guy.drawString("Hello Readguy!",10,10); //用此函数将字符串显示到屏幕缓存内
  //guy.print("Hello Readguy!"); //使用这个函数也能显示出字符串, 但是需要提前使用setCursor确定显示坐标

  guy.display(READGUY_FAST);   // 快速刷新. 将屏幕缓存内的内容显示到墨水屏幕上
                               // 但是, 上电初始化之后的首次刷新必为慢速刷新
  //guy.display(READGUY_SLOW); // 慢速刷新. 

  guy.setCursor(10,30); //设置显示的坐标

  guy.print("Hello~"); //或者用print函数在屏幕上打印字符串, 数值, 字符等等... 两种函数都行

  guy.display(READGUY_SLOW); // 慢速刷新. 慢刷的对比度显著高于快速刷新, 而且可以消除残影


  guy.drawString(guy.SDinside()?"SD card OK.":"No SD card!",10,50); //检查readguy是否插入了SD卡

  guy.drawString("with " _READGUY_PLATFORM ".",10,70); //检查readguy运行在ESP8266上还是ESP32上.

  guy.display(); //不带参数时, 默认使用快速刷新


  guy.setTextColor(0);  //设置显示的颜色. 一个参数时,代表文本背景色为透明;
  //两个参数时, 代表文本背景色为后面的那个参数代表的颜色. 此次函数调用之后将显示为透明背景.

  guy.setCursor(10,80); //设置显示的坐标, 用于显示别的内容

  guy.setTextSize(2); //或者用此函数在屏幕上打印字符串.

  guy.print('a'); //使用print打印字符.

  guy.display(); //快刷


  guy.setTextSize(1); //恢复常规大小字符
  
  guy.setFont(&fonts::Font0); //设置小字体 [字体设定部分参考lovyanGFX的示例]
  
  guy.setCursor(10,110); //设置显示的坐标, 用于显示别的内容

  guy.print(926824162); //使用print打印数字.

  guy.print(' '); //使用print打印字符.

  guy.print(1.61834,5); //使用print打印浮点数字, 5代表显示5位数

  guy.display(); //快刷


  guy.setCursor(10,130); //设置显示的坐标, 用于显示别的内容

  guy.printf("Small font using %s. It's a long string in the E-paper. "
  "Readguy always print it in a new line","printf");
  //使用 print 或者 printf 显示长串字符串时, 后面的字符会自动显示到下一行.而 drawString 不会这样

  guy.display(); //快刷
  
  delay(2000);

//                                              ---------------------- 3 - 灰度显示功能演示 -----<<<

  guy.fillScreen(1);

  guy.display(READGUY_SLOW); // 慢速刷新. 慢刷的对比度显著高于快速刷新, 而且可以消除残影

  for(int i=1;i<16;i++){ //灰度测试, 循环设置不同灰度

    guy.setDepth(i); //设置灰度的颜色深度. 可接受的值为1~15.(从白到黑)
    //注意1. 在此函数内参数越接近0越白, 越接近15越黑. 输入参数为0或者>15时则自动设为15(最黑).
    //这和其他绘图函数中的 "0黑1白" 表示方法相反.
    //注意2. 此函数需要在程序中调用display()函数刷新屏幕之后才会生效, 且此次刷新必须是快刷.
    //若该函数在两次display()函数之间被多次调用, 则最终呈现的灰度依据最后一次调用的setDepth()函数.
    // OK: guy.setDepth(4); guy.print("hello"); guy.display();
    //     guy.setDepth(8); guy.print("world"); guy.display();
    // NOT OK: guy.setDepth(4); guy.print("hello");
    //         guy.setDepth(8); guy.print("world"); guy.display();
    // 最终呈现两个print函数等效于只调用一次 guy.setDepth(8);

    guy.fillRect(10,i*10,20,10,0); //显示填充矩形.
    //注意, 此处最后一个参数0代表是黑色, 黑色的深度则是由前面的setDepth函数设定的.
    //即使您希望使用setDepth(1)之后显示一个浅灰色的矩形, 也需要将这个参数设为0(黑色)

    guy.setCursor(32,i*10);

    guy.printf("Grey%d",i);

    guy.display(); //快刷显示.

  }
  delay(2000);

//                                            --------------------- 4 - 显示BMP格式图片文件 ----<<<<
  // [此部分更多可以参考lovyanGFX的示例]

  guy.fillScreen(1);  //清屏
  
  guy.display(); //显示白屏,用于将来显示图片.
  
  LGFX_Sprite sp(&guy); //创建一个Sprite (可以存储一些像素, 快速读写)

  sp.createFromBmpFile(guy.guyFS(),"/test.bmp"); //从文件创建BMP图像信息.
  //使用guy.guyFS()函数返回可用的文件系统. 默认为SD卡. 当SD卡不可用时, 自动切换到内置的LittleFS.

  //显示打开的图片信息(宽度和高度), 和剩余内存
  Serial.printf("[%lu] sp.w: %d, h: %d, res: %d.\n",millis(),sp.width(),sp.height(),ESP.getFreeHeap());

  guy.drawImage(sp,10,10); //使用抖动像素的方式显示图片(不是灰度, 只有黑点和白点的那种显示效果)

  guy.display(); //自从1.2.0更新之后, drawImage不再刷屏, 此处需要额外调用display函数刷屏

  Serial.printf("[%lu] drawn dithering bmp.\n",millis()); //显示信息

  delay(2000);


  guy.setGreyQuality(1); //设置灰度刷新方式. 对于支持连续刷灰度的屏幕才有效.
  // 1(默认)为连续刷新, 0为循环调用 setDepth+display 来刷新 (可能会有白边)
  //如果连续刷新效果不好, 请将此处改为0再试一次.

  guy.draw16grey(sp,10,10); //使用16级灰度的方式显示图片 需要的时间比较长

  sp.deleteSprite(); //关闭图片文件, 释放图片占用的大量内存

  Serial.printf("[%lu] drawn 16-layer greyscale bmp.\n",millis()); //显示信息

  delay(5000);
  
//                                            ---------------------- 5 - 其他屏幕功能测试 ---<<<<<

  guy.setFont(&FreeMonoBold9pt7b); //设置文本字体
  guy.setTextColor(0); //设置文本颜色
  guy.fillScreen(1); //用白色清屏.
  guy.display(READGUY_SLOW); //慢刷. 注意, 进行慢刷操作之后, 所有之前显示的灰度内容均会被重新刷成纯黑色
  //不管是浅灰色还是深灰色, 进行慢刷之后只有黑白色. 原来的非白色像素(浅灰色,深灰色和黑色等) 会全刷成白色.
  
  guy.drawString("Rotation 0",10,12); //默认旋转方向为0. 实际的默认方向取决于屏幕IC. 大多数屏幕IC是竖屏.
  drawLines(); //调用 画线函数, 绘制屏幕上的一些线条.并显示.

  guy.fillScreen(1); //用白色清屏.
  guy.setRotation(1);                 //设置旋转方向, 旋转完成之后会对画面进行裁切.
  guy.drawString("setRotation 1",12,10); //设置旋转方向
  drawLines();
  
  guy.fillScreen(1); //用白色清屏.
  guy.setRotation(2); //方向2和方向0 的宽度和高度相同.旋转了180度.
  guy.drawString("Rot 2",12,10); //设置旋转方向
  drawLines();
  
  guy.fillScreen(1); //用白色清屏.
  guy.setRotation(3); //方向3和方向1 的宽度和高度也是相同的.旋转了180度.
  guy.drawString("Rotate 3",15,10); //设置旋转方向
  drawLines();


  guy.drawString("Sleeping...",10,30);  //此部分程序演示如何使用屏幕睡眠(降低耗电量)

  guy.invertDisplay();                  //对屏幕上的一切都进行反色处理

  guy.display();                        //即将进入睡眠状态

  guy.sleepEPD();                      //进入睡眠模式
  //注意, 如果没有设置RST引脚, 则实际上不会进入睡眠模式. 睡眠模式依赖RST引脚的复位信号.
  
  delay(3000);

  guy.setTextColor(1);  //设置文本颜色为白色,因为被反色的屏幕的当前像素颜色以黑色像素为主
  guy.drawString("Wake Up! ~\\(^_^)/~",10,50); //退出睡眠状态

  guy.display(READGUY_SLOW);  //使用慢刷 来唤醒处于低功耗状态下的屏幕.

//                                   ------------------ 6 - 可以利用灰度来达到的一些显示效果 --<<<<<<
  
  guy.fillScreen(1);  //清屏
  
  guy.display(FILL_WHITE,READGUY_SLOW); //慢刷清屏. 左侧的FILL_WHITE表示 不写入屏幕缓存, 直接刷全白
  //可以改为FILL_BLACK来设置写入缓存全黑.
  //以上的方式均不会修改屏幕缓存中的内容. 右侧的false表示全屏慢刷.

  guy.setTextColor(0);  //设置显示的颜色. 0代表黑色, 一个参数代表黑白显示.
                        //注意这个函数不是设定显示灰度的函数!
  
  guy.setDepth(0); //恢复黑色显示, 此语句等效于setDepth(15)
  guy.setFont(&FreeMonoBold9pt7b); //设置文本字体
  guy.drawString("I love Readguy!",10,10); //设置显示坐标并显示
  guy.display(); //setDepth()函数需要调用display()函数刷新屏幕之后才会生效

  guy.setDepth(4); //把灰度深度设为一个浅色, 方便勾勒阴影.
  guy.drawString("I love Readguy!",12,12); //错开2像素并减淡颜色灰度,可以实现阴影效果
  guy.display();

  const char *st="Programmed by";
  guy.setDepth(7); //设置颜色深度
  guy.drawString(st,11,31); //设置显示坐标
  guy.display();
  guy.setDepth(4);          //这里有一点需要注意: 如果显示的位置原本就已经显示过灰度数据了
  guy.drawString(st,10,31); //(其实这里的像素在内存里存储的还是黑色), 即使新设定的颜色深度比之前的颜色深,
  guy.drawString(st,11,30); //也不会覆盖这里原来的浅灰色. 此处的灰度设定只对原来是纯白色的像素有效.
  guy.display();            //如果要更改这个像素点的颜色, 只能全部刷白屏之后再重新刷这些灰度颜色.
  guy.setDepth(15);         //(最好是慢刷白屏,如果要坚持使用快刷,也需要把灰度设置为全黑色对应的15再刷)
  guy.drawString(st,11,32); //
  guy.drawString(st,12,31); //此处的函数展现的是一个浮雕效果.
  guy.display();

  const char str[]="FriendshipEnder"; //实现类似于渐变色的效果
  guy.setCursor(8,50);
  for(int i=0;i<15;i++){ //在 for 循环内循环修改显示深度
    guy.setDepth(15-i);
    guy.print(str[i]);
    guy.display(); //注意每次使用 setDepth 之后需要再刷一次屏, 灰度设置才能生效
  }

  guy.setDepth(6);
  guy.drawString("Follow me~~",14,68);
  guy.display();

  guy.setDepth(2);
  guy.drawString("Follow me~~",16,70);
  guy.display();

  guy.setDepth(5);
  guy.setTextColor(0);
  guy.drawString("on Bilibili!",14,90);
  guy.display();

  guy.setDepth(6); //绘制外框
  guy.drawRoundRect(8,86,guy.textWidth("on Bilibili!")+14,28,7,0);
  guy.display();

  guy.setDepth(15); //恢复到正常黑色
  guy.fillRoundRect(10,88,guy.textWidth("on Bilibili!")+10,24,5,0);
  guy.setTextColor(1);
  guy.drawString("on Bilibili!",16,92);
  guy.display();

  delay(5000);

  guy.sleepEPD(); //进入睡眠模式

//                                    ------------------ 7 - 屏幕以外的功能: 比如wifi和按钮 -<<<<<<<

  guy.ap_setup(); //启动ESP芯片内置 WiFi 热点, 启动后即可在wifi列表上查看readguy

  guy.server_setup(); //启动服务器功能, 默认地址为192.168.4.1

}

int bright=128; //设置亮度

void loop(){
  guy.server_loop(); //让服务器保持服务
  delay(10);
  if(bright%7==0){
    Serial.printf("getBtn: %d\n",guy.getBtn()); //每隔一段时间, 检测一下按键.
  }
  if(bright==511) bright=0;
  else bright++;
  guy.setBright(bright>=256?511-bright:bright);
}

void drawLines(){ //声明一个函数, 用于显示一些线条. 此函数在后面的程序中会用到的

  guy.drawFastHLine(0,0,20);    //使用 drawFastHLine 绘制横向线段
  guy.drawFastVLine(0,0,20);    //使用 drawFastVLine 绘制纵向线段
  guy.drawLine(     0,0,10,10); //使用 drawLine      绘制任意线段
  
  guy.drawFastHLine(guy.width()-20,0,20);
  guy.drawFastVLine(guy.width()- 1,0,20);
  guy.drawLine(     guy.width()- 1,0,guy.width()-20,20);

  guy.drawFastHLine(0,guy.height()- 1,20);
  guy.drawFastVLine(0,guy.height()-20,20);
  guy.drawLine(     0,guy.height()- 1,20,guy.height()-20);
  
  guy.drawFastHLine(guy.width()-20,guy.height()- 1,20);
  guy.drawFastVLine(guy.width()- 1,guy.height()-20,20);
  guy.drawLine(     guy.width()- 1,guy.height()- 1,guy.width()-20,guy.height()-20);

  guy.display();
  delay(1000);
}/* END OF FILE. ReadGuy project.
Copyright (C) 2023 FriendshipEnder. */