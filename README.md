# readguy 自由的墨水屏阅读器驱动库

版本1.0正式发布！欢迎分享、star和fork~

欢迎克隆, 项目交流QQ群: 926824162 (萌新可以进来问问题的哟), 项目的 Bilibili 主页: 

依赖的库: [LovyanGFX](https://github.com/lovyan03/LovyanGFX)

依赖的环境: Arduino-ESP8266 或者 Arduino-ESP32.

## 在**所有**受支持屏幕上都显示**16级灰度**内容！

### 目前支持ESP芯片平台、多种屏幕型号、1～3个按钮操控、SD卡、前置光亮度控制！

### 一次编译即可使程序兼容多种屏幕！

### 固件在同芯片平台下完全兼容不同硬件设计！

### 可以在内存极少的情况下以抖动方式或者16级灰度方式渲染主流格式图片！

### *由于内存过低，ESP8266上暂不支持PNG格式显示。*

### 可以使用wifi配网来配置硬件信息。

----

## 支持芯片：esp8266/esp32/esp32s3/esp32s2/esp32c3

Supported chips: esp8266/esp32/esp32s3/esp32s2/esp32c3

## 支持屏幕：1.54寸价签屏，2.13寸价签屏，2.66，2.9，4.2均支持

Supported displays: 1.54-inch, 2.13-inch, 2.66-inch, 2.9-inch, 4.2-inch.

可以自己在代码中加入你想要的屏幕型号

You can add the screen model you want in the code.

支持sd卡读取

SD is available

仅需1-3个gpio按键

Only 1 button is needed and can be added to maximum 3 READGUY_buttons.

可选的ds1302，ds1307，ds3231 rtc (现在还不支持)

Using a real-time clock module is optional. (Not supported now)

可选的背光灯调节

Light-brightness-control is optional.

## 即将可以提供的功能：(其实现在都还没有涅)

1. WiFi功能完善

2. web配网/文件管理

3. 自定义布局的时钟（需要bmp图片和一个布局json文件）

4. 心知天气

5. 资讯查看+二维码查看

6. 相册

7. txt阅读器带进度指示

8. sd卡固件更新/切换

## 使用方法-按钮操控方式：

1. 烧录成功后，即刻出现一个wifi热点“readguy”，（wifi密码为12345678）

2. 连接此wifi之后访问192.168.4.1之后就可以对其中的初始设定项目更改。

3. 尽情使用吧！

4. 单按键操作说明

- 单击：向右移动

- 双击：向左移动

- 长按半秒：选择/确定

- 三连击：界面内返回，键盘输入状态下可以快速切换英文大小写

5. 双按键操作说明：

按键1单击：向右移动

按键1长按：向左移动

按键2单击：确定

按键2长按半秒：界面内返回，键盘输入状态下可以快速切换英文大小写

6. 三按键操作说明：(只不过, GUI功能还在研发编写中...)

按键1：向左/向上，

按键2单击：确定，

按键3：向右/向下。

按键2长按半秒：界面内返回，键盘输入状态下可以快速切换英文大小写

7. 关于菜单控件: 在菜单模式下若处于最后一个菜单选项,再按 向右/向下 功能键则会进入滑动条(选择菜单选项).

---

## 项目思路:

1. 启动时, 通过WiFi进行初始化配置

2. 配置好之后, 程序会按照配置里面的数据进行对屏幕的初始化等工作

3. 如果屏幕初始化成功, 屏幕上将会显示一段文本, 然后在web上输入此文本即可完成显示设备验证

4. 按键初始化也需要验证. 只需要按下对应按键就可以了, 可以自由选择是低电平按下or高电平按下

5. sd卡和背光初始化是可选的

6. 设备最好都要支持littlefs来存放书籍文件

---

Copyright © 2022-2023 FriendshipEnder. All Rights reserved.

版权声明：需要经过作者@friendshipender的许可才能商用。

