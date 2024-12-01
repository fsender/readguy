/******************** F r i e n d s h i p E n d e r ********************
 * 本程序隶属于 Readguy 开源项目, 请尊重开源开发者, 也就是我FriendshipEnder.
 * 如果有条件请到 extra/artset/reward 中扫描打赏,否则请在 Bilibili 上支持我.
 * 项目交流QQ群: 926824162 (萌新可以进来问问题的哟)
 * 郑重声明: 未经授权还请不要商用本开源项目编译出的程序.
 * @file guy_button.h
 * @author FriendshipEnder (f_ender@163.com), Bilibili: FriendshipEnder
 * @brief 按键驱动库 头文件  
      guy_button.h - for Readguy project
      Copied from Button2 Library created by Lennart Hennigs.
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
 *
 * **************************************************************************
 *
  guy_button.h - for Readguy project
  Copied from Button2 Library created by Lennart Hennigs.
  
Original Button2 License:

MIT License

Copyright (c) 2017-2022 Lennart Hennigs 

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

 */
#ifndef _GUY_BUTTON_H_FILE
#define _GUY_BUTTON_H_FILE

#include "Arduino.h"
#include <stdint.h>
#if defined(ARDUINO_ARCH_ESP32) || defined(ESP8266)
  #include <functional>
#endif
#include <Arduino.h>

/////////////////////////////////////////////////////////////////


#define GUYBUTTON_empty          0 //没按下
#define GUYBUTTON_single_click   1 //单击
#define GUYBUTTON_double_click   2 //双击
#define GUYBUTTON_triple_click   3 //三击
#define GUYBUTTON_long_click     4 //长按
#define GUYBUTTON_xlong_click    5 //点击后接长按
#define GUYBUTTON_xxlong_click   6 //双击后接长按
#define GUYBTN_READ_TIMEOUT    100 //读取延时
#define GUYBTN_LOOP_TIMEOUT     10 //循环扫描延时

class guy_button{
  public:
    uint16_t min_debounce    ;   //去抖时间
    uint16_t long_press_ms   ;  //长按持续时间
    uint16_t double_press_ms ;  //双击识别间隔最大时间
    uint16_t long_repeat_ms  ;  //长按连按间隔时间
  protected:
    uint8_t pin = 255; //未定义引脚
    uint8_t state;
    uint8_t prev_state;
    uint8_t click_count = 0;
    uint8_t _pressedState;
    uint8_t scanDT; //是否为多个按钮, 可自己设置
    uint8_t last_click_type = GUYBUTTON_empty;
    volatile uint8_t lk = 255;
    //int id;
    unsigned int down_time_ms = 0;
    unsigned long click_ms;
    unsigned long down_ms;
    unsigned long long_clicked = 0;

    //bool longclick_detected_retriggerable;
    //uint16_t longclick_detected_counter = 0;
    bool was_pressed = false;
    bool longclick_detected = false;
    bool pressed_triggered = false;
    bool trig_mode = false; //长按连按触发模式

    typedef uint8_t (*std_U8_function_U8)(uint8_t, bool);
    std_U8_function_U8 get_state_cb = NULL;

  public:
    guy_button();
    /// @brief 初始化
    /// @param _pin 引脚ID. 传入的引脚在内部将会使用digitalRead函数实现
    /// @param activeLow 设置为true时, 当读取到低电平视为按下
    void begin(uint8_t _pin, bool activeLow = true){
      begin(_pin,[](uint8_t p, bool)->uint8_t { return digitalRead(p); },activeLow);
    }
    /// @brief 初始化
    /// @param _pin 引脚ID. 传入的引脚在内部将会使用digitalRead函数实现
    /// @param f 触发函数: 当activeLow为false时, 返回1表示按下, 0表示没按下 为true时相反
    /// @note 默认的 f 是 匿名函数 [](uint8_t p)->uint8_t { return digitalRead(p); }
    ///       如果自己指定了函数而且没有用到内置的参数, 那么 _pin 参数可能是没有任何用处的
    void begin(uint8_t _pin, std_U8_function_U8 f, bool activeLow  = true);
    /// @brief 设置长按连按触发模式
    /// @param trigMode 0:单次长按 1:连续长按
    void setLongRepeatMode(bool trigMode) { trig_mode = trigMode; }
    /// @brief 长按了多久按钮
    unsigned int wasPressedFor() const { return down_time_ms; }
    /// @brief 返回是否处于被按下的状态 受loop扫描的限制, 如果没loop扫描则可以先手动调用扫描后使用
    bool isPressed() const { return (state == _pressedState); }
    /// @brief 读取原始的按钮状态 (不去抖动), 此函数不受loop扫描的限制
    bool isPressedRaw(); // { return (get_state_cb(pin) == _pressedState); }
    /// @brief 曾经按下的状态 是否是点击后立即松开
    bool wasPressed(){ if(was_pressed){ was_pressed = false; return true; } return false; }
    /// @brief 连击了几下
    uint8_t getNumberOfClicks() const{ return click_count;}
    /// @brief [已经弃用] 获取上次按钮的按下数据. 返回按钮状态(按钮状态参考read函数的说明)
    uint8_t getType() const { return last_click_type; }
    /// @brief 读取按钮的按下数据. 返回按钮状态 0没按 1单击 2双击 3三击 4长按 5点击后长按 6双击后长按
    uint8_t read();
    /// @brief 连续循环扫描按钮. 必须多次反复调用, 最好是单独开一个task来实现
    void loop();
    /// @brief 设置是否识别双击 三连击等高级手势
    /// @param scan =1识别双击或三击, =0则不识别双击或三击等需要延时返回的情况
    void enScanDT(uint8_t scan) { scanDT = scan; }
    /* void setMinDebounce(short n) { min_debounce    =n;}   //去抖时间
    void setLongPressMs(short n) { long_press_ms   =n;}  //长按持续时间+双击识别间隔最大时间
    void setLongRepeat(short n)  { long_repeat_ms  =n;}  //长按连按间隔时间
    void getMinDebounce(short n) { min_debounce    =n;}   //去抖时间
    void getLongPressMs(short n) { long_press_ms   =n;}  //长按持续时间+双击识别间隔最大时间
    void getLongRepeat(short n)  { long_repeat_ms  =n;}  //长按连按间隔时间 */
};

#endif /* END OF FILE. ReadGuy project. */