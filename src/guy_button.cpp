/******************** F r i e n d s h i p E n d e r ********************
 * 本程序隶属于 Readguy 开源项目, 请尊重开源开发者, 也就是我FriendshipEnder.
 * 如果有条件请到 extra/artset/reward 中扫描打赏,否则请在 Bilibili 上支持我.
 * 项目交流QQ群: 926824162 (萌新可以进来问问题的哟)
 * 郑重声明: 未经授权还请不要商用本开源项目编译出的程序.
 * @file guy_button.cpp
 * @author FriendshipEnder (f_ender@163.com), Bilibili: FriendshipEnder
 * @brief 按键驱动库 源代码文件  
      guy_button.cpp - for Readguy project
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
  guy_button.cpp - for Readguy project
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
#include "guy_button.h"
#include "guy_driver_config.h"

// initalize static counter

guy_button::guy_button() {  
  //pin = UNDEFINED_PIN;
  //id = _nextID++;
}

void guy_button::begin(uint8_t _pin, std_U8_function_U8 f, bool activeLow/*=true*/) {
  //pin = attachTo;
  //id = _nextID++;
  _pressedState = activeLow ? LOW : HIGH;
  //  state = activeLow ? HIGH : LOW;
  prev_state = state ;
  get_state_cb = f;
  pin = _pin;
  state = get_state_cb(pin, activeLow);
  min_debounce    =READGUY_DEFAULT_MIN_DEBOUNCE_MS;   //去抖时间
  long_press_ms   =READGUY_LONG_PRESS_MS;  //长按持续时间
  double_press_ms =READGUY_DOUBLE_PRESS_MS;  //双击识别间隔最大时间
  long_repeat_ms  =READGUY_LONG_REPEAT_MS;  //长按连按间隔时间
  scanDT          =1;    // =1识别双击或三击, =0则不识别双击或三击等需要延时返回的情况
  lk=0;
}
bool guy_button::isPressedRaw() {
  int mi=millis();
  while(lk) if(millis()-mi>GUYBTN_READ_TIMEOUT) return 0; //等待数据读完
  lk=3;
  bool willreturn = (get_state_cb(pin, !_pressedState) == _pressedState);
  lk=0;
  return willreturn;
}
uint8_t guy_button::read() { //注意ticker不能在此触发
  int mi=millis();
  while(lk) if(millis()-mi>GUYBTN_READ_TIMEOUT) return 0; //等待数据读完
  lk=2;
  unsigned long n = millis();
  if(state == _pressedState && n - down_ms>= long_press_ms && long_clicked < n){
    long_clicked = trig_mode?(n+long_repeat_ms):0xfffffffful;
    lk=0;
    return (click_count>=3)?GUYBUTTON_xxlong_click:\
            ((click_count==2)?GUYBUTTON_xlong_click:GUYBUTTON_long_click);
  }
  uint8_t res = last_click_type;
  last_click_type = GUYBUTTON_empty;
  was_pressed = false;
  lk=0;
  return (res>=GUYBUTTON_long_click)?GUYBUTTON_empty:res;
}

void guy_button::loop() {
  if(get_state_cb==NULL || lk==255) return;
  int mi=millis();
  while(lk) if(millis()-mi>GUYBTN_LOOP_TIMEOUT) return; //等待数据读完
  lk=1;
  unsigned long now = millis();
  prev_state = state;
  state = get_state_cb(pin, !_pressedState);
  
  // is button pressed?
  if (state == _pressedState) {
    // is it pressed now?
    if (prev_state != _pressedState) {
      down_ms = now;
      pressed_triggered = false;
      click_ms = down_ms;
    // trigger pressed event (after debounce has passed)
    } else if (!pressed_triggered && (now - down_ms >= min_debounce)) {
      pressed_triggered = true;
      click_count++;
    }
  // is the button released?
  } else if (state != _pressedState) {
    // is it released right now?
    if (prev_state == _pressedState) {
      down_time_ms = now - down_ms;
      // is it beyond debounce time?
      if (down_time_ms >= min_debounce) {
        //last_click_type = GUYBUTTON_single_click;
        // trigger release        
        // trigger tap
        // was it a longclick? (preceeds single / double / triple clicks)
        if (down_time_ms >= long_press_ms)
          longclick_detected = true;
      }
    // is the button released and the time has passed for multiple clicks?
    } else if (now - click_ms > (scanDT?double_press_ms:min_debounce)) {
      // was there a longclick?
      if (longclick_detected) {
        // was it part of a combination?
        if (click_count) {
          last_click_type = (click_count>=3)?GUYBUTTON_xxlong_click:\
            ((click_count==2)?GUYBUTTON_xlong_click:GUYBUTTON_long_click);
          was_pressed = true;
        }
        longclick_detected = false;

      // determine the number of single clicks
      } else if (click_count > 0) {
        if(scanDT){
          switch (click_count) {
            case 1: last_click_type = GUYBUTTON_single_click; break;
            case 2: last_click_type = GUYBUTTON_double_click; break;
            case 3: last_click_type = GUYBUTTON_triple_click;
          }
        }
        else last_click_type = GUYBUTTON_single_click; //此时若click_count>1 视为抖动
        was_pressed = true;
      }
      // clean up
      click_count = 0;
      click_ms = 0;
    }
    long_clicked = 0; //长按结束
  }
  lk=0; //解锁
} /* END OF FILE. ReadGuy project. */
