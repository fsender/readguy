/******************** F r i e n d s h i p E n d e r ********************
 * 本程序隶属于 Readguy 开源项目, 请尊重开源开发者, 也就是我FriendshipEnder.
 * 如果有条件请到 extra/artset/reward 中扫描打赏,否则请在 Bilibili 上支持我.
 * 项目交流QQ群: 926824162 (萌新可以进来问问题的哟)
 * 郑重声明: 未经授权还请不要商用本开源项目编译出的程序.
 * @file guy_version.h
 * @author FriendshipEnder (f_ender@163.com), Bilibili: FriendshipEnder
 * @brief readguy 版本控制文件.
 * @version 1.4.2
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

//在进行版本更迭之前, 需要检查以下文件的版本号是否为最新
//1. ChangeLog.md
//2. library.json
//3. library.properities
//4. README.md
//5. git commit 信息
//6. 下面的三行 以及下面的这个字符串
//务必保证这些版本号是一致的.
//另外, 在提交新版本之前, 不要忘记在github上创建release, 否则Arduino IDE会读不到
#define READGUY_V_MAJOR 1
#define READGUY_V_MINOR 5
#define READGUY_V_PATCH 0
#define READGUY_VERSION_VAL (READGUY_V_MAJOR*1000+READGUY_V_MINOR*100+READGUY_V_PATCH*10)
#define READGUY_VERSION "1.5.0"

#define _GITHUB_LINK      "https://github.com/fsender/readguy"
#define _BILIBILI_LINK    "https://www.bilibili.com/video/BV1f94y187wz"


#endif /* END OF FILE. ReadGuy project.
Copyright (C) 2023 FriendshipEnder. */