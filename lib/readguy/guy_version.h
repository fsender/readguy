/******************** F r i e n d s h i p E n d e r ********************
 * 本程序隶属于 Readguy 开源项目, 请尊重开源开发者, 也就是我FriendshipEnder.
 * 如果有条件请到 extra/artset/reward 中扫描打赏,否则请在 Bilibili 上支持我.
 * 项目交流QQ群: 926824162 (萌新可以进来问问题的哟)
 * 郑重声明: 未经授权还请不要商用本开源项目编译出的程序.
 * @file guy_version.h
 * @author FriendshipEnder (f_ender@163.com), Bilibili: FriendshipEnder
 * @brief readguy 版本控制文件.
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
 */

#ifndef _READGUY_VERSION_H_FILE
#define _READGUY_VERSION_H_FILE

#define READGUY_V_MAJOR 1
#define READGUY_V_MINOR 0
#define READGUY_V_PATCH 0
#define READGUY_VERSION_VAL (READGUY_V_MAJOR*1000+READGUY_V_MINOR*100+READGUY_V_PATCH*10)
#define READGUY_VERSION "1.0.0"

#ifdef ESP8266
#define _READGUY_PLATFORM "ESP8266"
#else
#ifdef CONFIG_IDF_TARGET_ESP32
#define _READGUY_PLATFORM "ESP32"
#elif defined(CONFIG_IDF_TARGET_ESP32S2)
#define _READGUY_PLATFORM "ESP32S2"
#elif defined(CONFIG_IDF_TARGET_ESP32S3)
#define _READGUY_PLATFORM "ESP32S3"
#elif defined(CONFIG_IDF_TARGET_ESP32C3)
#define _READGUY_PLATFORM "ESP32C3"
#else
#define _READGUY_PLATFORM "Unknown"
#endif
#endif

#define _GITHUB_LINK      "https://github.com/fsender/readguy"
#define _BILIBILI_LINK    "https://space.bilibili.com/180327370"


#endif /* END OF FILE. ReadGuy project.
Copyright (C) 2023 FriendshipEnder. */