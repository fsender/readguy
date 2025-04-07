# readguy 自由的墨水屏阅读器驱动库

[![arduino-library-badge](https://www.ardu-badge.com/badge/readguy.svg?)](https://www.ardu-badge.com/readguy)

## ReadGuy 2.0 预告

1. 内置驱动,无其他 不跨平台 多平台

2. 自定义脚本驱动 不跨平台 多平台

3. 内置 不自定义 带脚本 跨平台 数据存eeprom或者nvs. 支持从文件系统加载

4. 文件系统接口优化

5. 灰度显示优化

6. 按键逻辑优化: 双按键:按键1点按下一个 长按上一个 长按过程中点按2则功能5 按键2点按确定 长按取消
三按键 左键点按松开时候结算: 上一个, 左键长按也是上一个 右键点按长按都是下一个 按住左键按中间功能5 中键点按确定 长按取消

7. 支持pre_init post_init 等设备限定的脚本,在初始化设备时和设备休眠时都可以执行类似功能. 此外readguy2.0的二进制是兼容1.x的

8. 摒弃默认刷屏方式使用dispWriter函数 速度太慢, 改为使用两种示例, 正常读取显示和函数读取显示都支持调用

敬请期待

<img src="https://github.com/fsender/readguy/blob/1.4.2/extra/artset/readguy_theme3.png?raw=true" width="30%" height="auto">

**版本1.5.0正式发布！欢迎分享、star和fork~** 上面的图是项目看板娘, 盖. 可爱的盖姐在等你哟~

**即将发布7个全新的屏幕驱动: 欢迎支持! (详见后面的驱动表格)**

欢迎克隆, 项目交流QQ群: 926824162 (萌新可以进来问问题的哟), 项目的 Bilibili 主页: [BV1f94y187wz](https://www.bilibili.com/video/BV1f94y187wz/) 记得三连+关注我这个宝藏up主哦~

注意, 有问题一定要先加群问, 先不要提issue, 提了作者 FriendshipEnder 也不会看的.

**依赖的库: [LovyanGFX](https://github.com/lovyan03/LovyanGFX)**

*依赖的环境: Arduino-ESP8266 或者 Arduino-ESP32. 现已支持使用最新版本的Arduino-ESP32库(2.x和3.x)编译!*

## 在**所有**受支持屏幕上都显示**16级灰度**内容！

### 一次编译即可使程序兼容多种屏幕！

<img src="https://github.com/fsender/readguy/blob/1.4.2/extra/artset/readguy_girl.png?raw=true" width="25%" height="auto">

<img src="https://github.com/fsender/readguy/blob/1.4.2/extra/artset/view.jpg?raw=true" width="auto" height="auto">

#### 目前支持ESP芯片平台、多种屏幕型号、1～3个按钮操控、SD卡、前置光亮度控制！

#### 固件在同芯片平台下完全兼容不同硬件设计！

#### 可以在内存极少的情况下以抖动方式或者16级灰度方式渲染主流格式图片！

#### *由于内存过低，ESP8266上暂不支持PNG格式显示。*

#### 可以使用wifi配网来配置硬件信息。

有不少人吐槽我这个库的WiFi功能可不可以关掉 就是把WiFi独立出来 其实是可以的, 教程如下

其中的WiFi功能, 其实是可以禁掉的. 只要你提前配置成功, 那么就可以摆脱WiFi配网配引脚功能. 

使用方法: 打开文件[guy_driver_config.h](src/guy_driver_config.h), 随后便根据注释来选择性的开启或关闭一些系统功能.

----

## 支持芯片：esp8266/esp32/esp32s3/esp32s2/esp32c3

Supported chips: esp8266/esp32/esp32s3/esp32s2/esp32c3

## 支持屏幕：1.54寸价签屏，2.13寸价签屏，2.66，2.9，4.2均支持

Supported displays: 1.54-inch, 2.13-inch, 2.66-inch, 2.9-inch, 4.2-inch.

| # |屏幕名称           |  驱动   | 分辨率 | 灰度 |连续灰度刷新|慢刷时间|快刷时间|
|:-:|:----------------:|:-------:|:-----:|:----:|:---------:|:----:|:-----:|
| 1 |1.54寸标准         |原创自研 |200*200| 16阶 | 不支持    | 1.3s | 0.35s |
| 2 |1.54寸Lilygo       |原创自研 |200*200| 16阶 | 支持      | 2.3s | 0.6s |
| 3 |2.13寸汉朔价签     |原创自研 |122*250| 16阶 | 不支持    | 1.6s | 0.31s |
| 4 |2.13寸低分辨率版   |原创自研 |104*212| 16阶 | 支持      | 1.12s | 0.17s |
| 5 |2.66寸Vusion价签   |和4号相同|152*296| 16阶 | 支持      | 1.12s | 0.17s |
| 6 |2.7寸佳显触摸屏    |和2号相同|176*264| 16阶 | 支持      | 2.3s | 0.6s |
| 7 |2.9寸A01           |和1号相同|128*296| 16阶 | 不支持    | 1.8s | 0.55s |
| 8 |2.9寸T94背光       |和2号相同|128*296| 16阶 | 支持      | 2.3s | 0.6s |
| 9 |3.7寸高dpi墨水屏   |原创自研 |280*480| 16阶 | 不支持    | 1.3s | 0.5s |
|10 |4.2寸HINK黑白价签  |原创自研 |400*300| 16阶 | 支持      | 1.72s | 0.38s |
|11 |4.2寸WF丝印        |原创自研 |400*300| 16阶 | 不支持    | 3.6s | 0.8s |
|12 |2.13寸三色         |和4号相同|122*250| 16阶 | 支持      | 未知 | 未知 |
|13 |2.66寸三色价签     |和4号相同|152*296| 16阶 | 支持      | 2.48s | 0.65s |
|14 |M5 Core.Ink 1.54寸 |即将支持|200*200|/|/|/|/|
|15 |3.7寸低DPI版墨水屏 |即将支持|416*240|/|/|/|/|
|16 |4.26寸高分辨率墨水屏|即将支持|800*480|/|/|/|/|
|17 |5.83寸墨水屏幕     |原创自研|600*448| 16阶 | 支持       | 1.7s  | 0.8s |
|18 |5.83寸GDEQ0583T31  |即将支持|640*480|/|/|/|/|
|19 |7.5寸三色墨水屏幕   |即将支持|800*480|/|/|/|/|
|20 |10.2寸GDEQ102T90   |即将支持|960*640|/|/|/|/|

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

---

## 配置方法-使用网页配网配置引脚

1. 首次烧录完成后, 按下板子的复位按键. 

2. 打开手机或者电脑的WiFi, 然后找到名称为 `readguy` 的网络, 连接. 密码为12345678.

3. 连接完成后, 打开该手机的浏览器, 访问192.168.4.1 如果手机提示该网络无法访问因特网, 则选择保持连接.

4. 加载完成之后, 根据你的硬件配置引脚. 网页会提供输入各个引脚的输入框, 根据自己的硬件情况输入引脚即可. 如果你正在使用甘草和半糖的开发板, 点击最上方的自定义线序, 可以快捷设置引脚.

5. 点击下方的OK!来确定配置, 直到屏幕闪烁并显示〝口口口口〞后, 即可进入下一步

6. 根据网页的按键说明提示, 依次按下按键, 直到屏幕刷新出4位数字验证码后, 将验证码填写到网页上, 最后点击确定!即可完成配置.

7. 如果刚才的引脚配置有误 (如按下按键无响应, 或者屏幕不显示), 则需要从该步骤的第一步重新开始.

## 使用方法-按钮操控方式：

1. 烧录成功后，即刻出现一个wifi热点“readguy”，（wifi密码为12345678）

2. 连接此wifi之后访问192.168.4.1之后就可以对其中的初始设定项目更改。

3. 根据网页内容设置引脚之后就可以尽情使用！

4. 单按键操作说明

- 单击：向右移动

- 双击：向左移动

- 长按半秒：选择/确定

- 三连击：界面内返回，键盘输入状态下可以快速切换英文大小写

5. 双按键操作说明：

- 按键1单击：向右移动

- 按键1长按：向左移动

- 按键2单击：确定

- 按键2长按半秒：界面内返回，键盘输入状态下可以快速切换英文大小写

6. 三按键操作说明：(只不过, GUI功能还在研发编写中...)

- 按键1：向左/向上，

- 按键2单击：确定，

- 按键3：向右/向下。

- 按键2长按半秒：界面内返回，键盘输入状态下可以快速切换英文大小写

7. 关于菜单控件: 在菜单模式下若处于最后一个菜单选项,再按 向右/向下 功能键则会进入滑动条(选择菜单选项).

---

## 项目思路:

1. 启动时, 通过WiFi进行初始化配置

2. 配置好之后, 程序会按照配置里面的数据进行对屏幕的初始化等工作

3. 如果屏幕初始化成功, 屏幕上将会显示一段文本, 然后在web上输入此文本即可完成显示设备验证

4. 按键初始化也需要验证. 只需要按下对应按键就可以了, 可以自由选择是低电平按下or高电平按下

5. sd卡和背光初始化是可选的

6. 设备最好都要支持littlefs来存放书籍文件

## ESP32 串口固件烧录方法:

1. 在压缩包内准备好以下三个文件:

如果压缩包内没有, 请浏览群文件内更大的压缩包, 其中应当右这些文件

```
bootloader.bin
partitions.bin     
ota_data_initial.bin      
firmware.bin       
```

2. 打开烧录软件 flash_download_tool_3.9.2.exe 烧录选项: flash 80MHz, DIO模式

```
bootloader.bin            0x1000
partitions.bin            0x8000
ota_data_initial.bin      0xe000
firmware.bin              0x10000
```

3. 等待烧录完成

## 项目编译注意事项

文件结构更改, 现在它更像一个 "库文件夹" 而不是一个 "项目文件夹". 因此使用时请按照以下说明, 自己编写程序

### 对于使用platformIO的用户:

1. 新建一个`PlatformIO`项目, 克隆`readguy`并放置于`lib`文件夹(用户库文件夹), 并在lib文件夹下额外克隆[`LovyanGFX`](https://github.com/lovyan03/LovyanGFX)库.(`LovyanGFX`库也需要放到`lib`文件夹内), *详见下图**下**方红框*.

2. 将库的`platformio.ini`(位于项目的`extra/platformio/platformio.ini`)替换掉新建项目内的`platformio.ini`. 默认编译的是`ESP8266`平台. *其实这一步可有可无, 只是这里提供了一种编译一定能成功的示例*

3. 如果编译目标是`ESP32`, 则需要更改`platformio.ini`内的 `[default_envs]` (在`PlatformIO.ini`文件内有说明), 而且还需要额外增加一个`components`文件夹 *(文件结构直接看图吧)*.

4. 注意编译目标是`ESP32`时, 此文件内的默认的ESP32环境并非纯arduino, 而是`ESP-IDF arduino component`. *此处`platformio.ini`里的framework是`espidf,arduino`, 这说明正在使用`ESP-IDF arduino component`. 这样可玩性更高涅!*

**如果想要使用纯arduino环境, 需要更改platformio.ini, 并将framework更改为`arduino`. 此时可以跳过 5~6 步骤**.

5. 编译目标是`ESP32`且使用`ESP-IDF arduino component`时, 需要单独安装[`LittleFS`](https://github.com/joltwallet/esp_littlefs)库来实现相关功能. 在项目内新建文件夹`components`, 在`components`文件夹内放入刚刚克隆的`LittleFS`库. *详见下图**上**方红框*. (使用纯arduino时跳过此步骤)

<img src="https://github.com/fsender/readguy/blob/1.4.2/extra/artset/build_dir_structure.png?raw=true" width="20%" height="auto">

6. 把`readguy`的`extra`文件夹内的`platformio`文件夹内所有文件都放在当前目录. 这些文件包含推荐的ESP32 sdkconfig, flash分区表等文件. 使用`pio run -t menuconfig`可以更改项目的menuconfig数据. (使用纯arduino时跳过此步骤)

7. 打开`examples`, 找到示例程序, 然后将源代码文件拷贝到项目的`src`文件夹内即可. *可以根据需要自己编写程序*.(***这不是废话??! \<憨色声线\>***)

8. 爽快的编译上传吧! 祝你 `0 error(s), 0 warning(s)`,加油~

### 对于使用 arduinoIDE 的用户:

1. 克隆项目之后, **首先你刚克隆完可能文件名是readguy-main.zip, 你需要把克隆好的文件重命名为readguy.zip** 再通过Arduino IDE添加到库, 或者解压拷贝到arduino IDE的libraries(arduino IDE的默认第三方库位置是C:\\Users\\$你的用户名$\\OneDrive\\documents\\Arduino\\libraries)文件夹内.

2. 现在打开arduino IDE, 找到examples, 选择需要的示例进行运行.

3. 打开库管理器, 找到[`LovyanGFX`](https://github.com/lovyan03/LovyanGFX)库并安装. 安装不了的用上述的1~2步骤克隆`LovyanGFX`并安装到`arduino IDE `内.

4. 爽快的编译上传吧! 祝你 `0 error(s), 0 warning(s)`,加油~

### 如何自定义功能

此库可以选择将一些功能关闭来减少flash的消耗.

其中的WiFi功能, 其实是可以禁掉的. 只要你提前配置成功, 那么就可以摆脱WiFi配网配引脚功能. 

使用方法: 打开文件[`guy_driver_config.h`](src/guy_driver_config.h), 随后便根据注释来选择性的开启或关闭一些系统功能.

**不推荐的做法!**: 其中有些屏幕用不到, 也可以通过`guy_epaper_config`来配置.

使用方法: 打开文件[`guy_epaper_config.h`](src/guy_epaper/guy_epaper_config.h), 随后便可以设置不加载哪些屏幕的驱动程序.

此操作可以节约flash和RAM消耗, **但是实际上此操作并不能节省太多的flash.** 为确保编译获得的程序兼容性, 应尽量不要更改这个文件.

- 还有一部分功能可以通过更改这两个文件来进行编辑 (如你想自己实现一个SD卡驱动)

- 但是如果自己更改了WiFi配置引脚的功能, ***这样编译出的程序就不能实现跨硬件运行了***

*有一些宏定义的组合是没有检验是否能够通过编译的. 如果有问题请提issue或者群里反馈*

## ESP32 项目配置 (使用 PlatformIO + Arduino as ESP-IDF component 环境)

menuconfig 内容:

``` ini
FREERTOS_HZ = 1000
ESP32_DEFAULT_CPU_FREQ_MHZ = 240
ESP32_BROWNOUT_DET_LVL = 0
ESP_PHY_REDUCE_TX_POWER=y
FATFS_API_ENCODING_UTF_8 = true
; SDK_TOOLCHAIN_SUPPORTS_TIME_WIDE_64_BITS = true
; SPI_MASTER_IN_IRAM = true      ; 该选项能提高SPI传输速率
; SPI_MASTER_ISR_IN_IRAM = true
```

---

Copyright © 2022-2025 FriendshipEnder. All Rights reserved.

版权声明：需要经过作者@friendshipender的许可才能商用。 可以联系邮箱playyinzhe@qq.com询问商用事宜

