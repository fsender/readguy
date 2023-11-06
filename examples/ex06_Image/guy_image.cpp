/******************** F r i e n d s h i p E n d e r ********************
 * 本程序隶属于 Readguy 开源项目, 请尊重开源开发者, 也就是我FriendshipEnder.
 * 如果有条件请到 extra/artset/reward 中扫描打赏,否则请在 Bilibili 上支持我.
 * 项目交流QQ群: 926824162 (萌新可以进来问问题的哟)
 * 郑重声明: 未经授权还请不要商用本开源项目编译出的程序.
 * @file guy_image.cpp
 * @author FriendshipEnder (f_ender@163.com), Bilibili: FriendshipEnder
 * @brief guy_image 基础功能 源代码文件. 
 * @version 1.0
 * @date 2023-11-01

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

#include "guy_image.h"
#include "readguy.h"
#define GUY_STAGES ((h+_h-1)/_h)

/// @brief 此函数为自定义图片显示程序
uint8_t readguyImage::drawImgHandler(int r, LGFX_Sprite *spr){
  int widthDiv8=spr->width()>>3; //宽度的1/8. 注意此处宽度一定是8的倍数.
  int stage = widthDiv8*_h;
  //uint32_t colors[8]={0x101010,0x303030,0x505050,0x707070,0x909090,0xb0b0b0,0xd0d0d0,0xf0f0f0};//for debug

  if(r%stage==0){ //分为8个阶段绘制图像, 每个阶段开始时, 打开图片文件尝试绘制.
    guy->implEndTransfer();
    Serial.printf("filename: %s(%d) Stage %d: Pixels: w=%d, h=%d\n",filename,format,r/stage,spr->width(),_h);

    spr->fillScreen(background?0xffff:0); //背景色填充白色

    //spr->drawBmp(*baseFs,filename,x,y,std::min(w,spr->width()),std::min(h,_h),
      //offsetx,offsety,scalex,scaley);
    int _x=0,_y=0,xd=0,yd=0;
    uint8_t rot = guy->getRotation();
    //uint8_t td=lgfx::v1::TL_DATUM; //top left for default
    switch(rot){
      case 0: 
      _x=x; xd=0;
      _y=y-r/stage*_h;
      if(_y<0){ yd=-_y; _y=0; }
      break;
      case 1: 
      //旋转之后, sprite为竖条. spr的width为_h. height为widthDiv8<<3(原sprite宽度)
      //输入的x位置: 转化为_y位置: (widthDiv8<<3)-x-1
      //输入的y位置: 转化为_x位置: _x=y. 限制: 分8块 分块: _x=y-r/stage*_h;
      //_x=y-r/stage*_h; yd=0;
      //_y=x;//(widthDiv8<<3)-x-1;
      _x=x-r/stage*_h;
      _y=y; yd=0;
      if(_x<0){ xd=-_x; _x=0; }
      break;
      case 2: 
      _x=x; xd=0;
      _y=y-(GUY_STAGES-r/stage-1)*_h;
      if(_y<0){ yd=-_y; _y=0; }
      break;
      case 3: 
      _x=x-(GUY_STAGES-r/stage-1)*_h;
      _y=y; yd=0;
      if(_x<0){ xd=-_x; _x=0; }
      break;
    }
    spr->setRotation(rot);
    switch(format&3){
      case 1:
        spr->drawBmpFile(*baseFs,filename,_x,_y,0,0,offsetx+xd,offsety+yd,scalex,scaley,datum);
        break;
#ifndef ESP8266
      case 2:
        spr->drawPngFile(*baseFs,filename,_x,_y,0,0,offsetx+xd,offsety+yd,scalex,scaley,datum);
        break;
#endif
      case 3:
        spr->drawJpgFile(*baseFs,filename,_x,_y,0,0,offsetx+xd,offsety+yd,scalex,scaley,datum);
        break;
    }
    spr->setRotation(0);
    //spr->setTextColor(0u,colors[r/stage]);//for debug
    //spr->drawString("Hello",0,0);//for debug
    /*此函数将会把图片文件绘制到这个灰度图里面.
    for(int j=0;j<spr->height();j++){
      for(int i=0;i<spr->width();i++){
        Serial.print((spr->readPixelRGB(i,j).R8())&0xff,HEX);
        Serial.write(' ');
      }
      Serial.print("\r\n\r\n");
    }*/
    delay(1);
    guy->implBeginTransfer();
  }
  // *
  if(r%widthDiv8==0){ //开始显示新的一行像素, 读取到buff内
    guy->implEndTransfer();
    int w=spr->width();
    //Serial.printf("line: %d %d, readStart: %d, readEnd: %d\n",r/w,r/widthDiv8,((r%stage)<<3), ((r%stage)<<3)+w);
    if(r==0){
      memset(floyd,0,w<<1);
      //Serial.printf("floyd buff = %d:\n",w);
      //for(int j=0;j<(w<<1);j++){
      //  Serial.printf("%d ",floyd[j]);
      //}
      //Serial.println();
    }
    memcpy_P(readBuff,((uint8_t*)spr->getBuffer())+((r%stage)<<3),w);
    uint_fast8_t buff8bit=0;
    for(int32_t j=0;j<w;j++){
      int32_t flodelta = floyd[((r/widthDiv8)&1)*w+j]+(int32_t)((readBuff[j]<<8)|readBuff[j]);
      if(format>>2){ //是灰度模式
        uint_fast8_t cg=0;
        if(enableFloyd){
          while(flodelta>=0x800) { 
            cg++;
            if(flodelta>=0) flodelta -= 0x1000;
          }
          if(flodelta<0) flodelta++;
        }
        else{ cg=readBuff[j]>>4; }
        if(format>>6){
          if(cg<15) //白色不考虑
            buff8bit |= (cg>=((~(format>>2))&0xf))<<((~j)&7);
        }
        else{
          buff8bit |= (cg<(format>>2))<<((~j)&7);
        }
      }
      else if(flodelta>=0x8000) { 
        //spr.drawPixel(j,i,1);
        buff8bit |= 1<<((~j)&7);
        flodelta -= 0xffff;
      }
      if((j&7)==7 || j==(w-1)){
        writeBuff[j>>3]=buff8bit^((format>>2)?0xff:0);
        buff8bit=0;
      }
      //计算出读取到的颜色, 然后与128比较, 如果小于128, 显示黑色,否则显示白色
      //else              { spr.drawPixel(j,i,0); }
      //if(j!=(int32_t)spr.width()-1) {
      //        floyd[  i&1 ][j+1] += (flodelta*7)>>4;
      //        floyd[!(i&1)][j+1] += (flodelta  )>>4;
      //}
      //if(j) { floyd[!(i&1)][j-1] += (flodelta*3)>>4; }
      //      { floyd[!(i&1)][j  ] += (flodelta*5)>>4; }
      if(j!=w-1) { floyd[  ((r/widthDiv8)&1) *w+j+1] += (flodelta*7)>>4; }
      if(j)      { floyd[(!((r/widthDiv8)&1))*w+j-1] += (flodelta*3)>>4; }
                 { floyd[(!((r/widthDiv8)&1))*w+j  ] += (flodelta*5)>>4; }
      if(j!=w-1) { floyd[(!((r/widthDiv8)&1))*w+j+1] += (flodelta  )>>4; }
    }
    for(int floi=0;floi<w;floi++) floyd[((r/widthDiv8)&1)*w+floi]=0;
    guy->implBeginTransfer();
  }
  return writeBuff[r%widthDiv8];
  /*
  //根据r的值返回对应位置上的的颜色. 一次读取8个像素
  uint_fast8_t colorBase = 0; //即将发送出去的像素数据
  //根据r的值返回对应位置上的的颜色. 一次读取8个像素
  for(uint_fast8_t j=0;j<8;j++){
    uint_fast8_t grey = *(((uint8_t *)spr->getBuffer())+((r%stage)<<3)+j); //获取对应位置的像素点的灰度值
    //计算灰度数值, 并使用floyd算法转化为位图.
    //if(readguyEpdBase::greysc(grey)>=128) colorBase|=(1<<(7-j));
    if(grey>=128) colorBase|=(1<<(7-j));
  }
  return colorBase;
  */
}

/// @brief 获取文件的扩展名, 最大长度为 exname_len
uint8_t readguyImage::getExName(const char* fname, char* exname, size_t exname_len){
  const char * dataex[12] = {
    "txt","bmp","jpg","png","mp3","wav","aac","flac","json","bin","html","js"
  };
  char dbuff[6];
  uint8_t foundLastName = 0;
  int fnamelen = strlen(fname);
  //Serial.print("test file name hook: ");
  //Serial.println(fname);
  if(fname[fnamelen-1] == '/') {
    //fname[fnamelen-1] = '\0'; //删除文件夹标记辅助位
    exname[0] = '/';
    exname[1] = '\0';
    return 2; //文件夹类型
  }
  for(int i=0;i<5;i++){
    if(fnamelen-1-i>=0) dbuff[i] = fname[fnamelen-1-i];
    else dbuff[i] = 0;
    if(dbuff[i] == '.'){
      dbuff[i] = 0;
      foundLastName = strlen(dbuff); //标记找到了扩展名
      break;
    }
  }
  if(foundLastName>exname_len) foundLastName = exname_len;
  for(int i=0;i<foundLastName;i++){
    exname[i] = dbuff[foundLastName-1-i];
  }
  exname[foundLastName] = 0;
  //Serial.print("test exname hook: ");
  //Serial.println(exname);
  if(exname[0] == 0) return 0; //无类型
  for(int i=0;i<12;i++){
    if(strcmp(exname,dataex[i]) == 0) return i+3;
  }
  return 1; //未知类型
}

/// @brief 显示图像
void readguyImage::drawImageFile(bool use16grey){
  if(filename == nullptr || filename[0] == 0 || !(baseFs->exists(filename))) return; //文件不存在
  char ex[8]; //保存文件的扩展名
  getExName(filename,ex,7); //获取文件的扩展名.最后一个参数用于防止数组越界
  format = 0; //16灰度模式
  //Serial.printf("filename: %s, exname: %s\n",filename,ex);

  //图片将会分割成8个部分, 分块绘制, 节省内存.
  w=(guy->drvWidth()+7)&0x7ffffff8; //guy->guyMemoryWidth() 返回不随旋转参数而改变的显示内存宽度
  if(!w) return; //保证宽度>0
  h=guy->drvHeight();
  if(exPoolSize>guy->bufferLength()){ //当外部缓存的像素超过屏幕缓存时,使用外部缓存作为主缓冲区
    _h=exPoolSize/w;
    _pool=exPool;
  }
  if(_pool==nullptr) {
    _h=(h+7)>>3; //设置缓存区的高度. 更多内存将可以更快显示
    _pool=(uint8_t *)guy->getBuffer();
  }
  //(guy->guyMemoryHeight()+7)>>3 返回高度,并补齐后右移三位 (等效于除以2³, 分成8份)
  //最后一个参数代表bpp,也就是有多少位, 在这里需要使用8位灰度.
  
  LGFX_Sprite bmpspr;
  //首先, 需要获取到内部显存的地址, 用于建立图片分块绘制缓存.
  //获取屏幕缓存, 随后分配图片解码所需的内存.
  bmpspr.setBuffer(_pool,w,_h,lgfx::v1::color_depth_t::grayscale_8bit);
  //bmpspr.createSprite(guy_width,(guy_height+7)&0x7ffffff8);

  //必须在此处转化为8bit灰度 (256等阶)
  //bmpspr.setColorDepth(lgfx::v1::grayscale_8bit); //因为LGFX的限制, 图片经过解码之后不能直接转换为单色/16色

  //随后打开图片进行解码. 可选显示的位置和宽度高度参数, 屏幕上的其他部分则会变成白色.
  if(strcmp(ex,"bmp") == 0 || strcmp(ex,"BMP") == 0) //BMP格式, 绘制BMP图片
    format|=1; //BMP格式
#ifndef ESP8266
  else if(strcmp(ex,"png") == 0 || strcmp(ex,"PNG") == 0) //PNG格式, 绘制PNG图片
    format|=2; //PNG格式
#endif
  else if(strcmp(ex,"jpg") == 0 || strcmp(ex,"JPG") == 0 || strcmp(ex,"jpeg") == 0 || strcmp(ex,"JPEG") == 0)
    format|=3; //JPG格式
  else return; //未知格式
  //bmpspr.drawBmp(*baseFs,filename,x,y,w,h,offsetx,offsety,scalex,scaley);
  floyd = new int16_t[w<<1];
  readBuff = new uint8_t[w];
  writeBuff = new uint8_t[w>>3];
  //guy->display([](int)->uint8_t{ return 0xff; },true);

  if(use16grey){
    for(int i=1;i<16;i++){
      format |= (i<<2);
      if(guy->supportGreyscaling()==16) format|=64;
      guy->draw16greyStep(std::bind(&readguyImage::drawImgHandler,this,std::placeholders::_1,&bmpspr),i);
      format &= 0x03;
    }
  }
  else{
    // ************* 提示: 编写此示例时的最新版本LovyanGFX库不提供此函数. 请看ex06_Image.ino文件开头的解决方法!
    guy->display(std::bind(&readguyImage::drawImgHandler,this,std::placeholders::_1,&bmpspr));
    // 此函数过不了编译 需要改库.
  }

  delete []floyd;
  delete []readBuff;
  delete []writeBuff;
  guy->fillScreen(0xff); //清空乱码
}

uint8_t readguyImage::drawImageToBuffer(){
  if(filename == nullptr || filename[0] == 0 || !(baseFs->exists(filename))) return 1; //文件不存在
  if( w==0 || h==0 || w>=0x8000 || h>=0x8000 || exPool==nullptr || exPoolSize<1024) return 3; //内存不足
  char ex[8]; //保存文件的扩展名
  format = 0; //16灰度模式
  getExName(filename,ex,7); //获取文件的扩展名.最后一个参数用于防止数组越界
  Serial.printf("filename: %s, exname: %s\n",filename,ex);

  if(strcmp(ex,"bmp") == 0 || strcmp(ex,"BMP") == 0) //BMP格式, 绘制BMP图片
    format|=1; //BMP格式
#ifndef ESP8266
  else if(strcmp(ex,"png") == 0 || strcmp(ex,"PNG") == 0) //PNG格式, 绘制PNG图片
    format|=2; //PNG格式
#endif
  else if(strcmp(ex,"jpg") == 0 || strcmp(ex,"JPG") == 0 || strcmp(ex,"jpeg") == 0 || strcmp(ex,"JPEG") == 0)
    format|=3; //JPG格式
  else return 2; //未知格式

  _h=exPoolSize/w;
  if(_h>h) _h=h; //分配的内存更多了, 直接使用
  _pool=exPool;
  if(_h==0 || GUY_STAGES>8) return 3; //内存不足以在显示8次之内就... 总之就是内存不够

  LGFX_Sprite spr;
  spr.setBuffer(_pool,w,_h,lgfx::v1::color_depth_t::grayscale_8bit);
  for(int i=0;i<GUY_STAGES;i++){
    spr.fillScreen(background?0xffff:0);
    switch(format&3){
      case 1:
        spr.drawBmpFile(*baseFs,filename,0,0,0,0,offsetx,offsety+_h*i,scalex,scaley);
        break;
#ifndef ESP8266
      case 2:
        spr.drawPngFile(*baseFs,filename,0,0,0,0,offsetx,offsety+_h*i,scalex,scaley);
        break;
#endif
      case 3:
        spr.drawJpgFile(*baseFs,filename,0,0,0,0,offsetx,offsety+_h*i,scalex,scaley);
        break;
    }
    guy->drawImageStage(spr,x,y+_h*i,i,GUY_STAGES);
  }
  return 0;
} /* END OF FILE. ReadGuy project.
Copyright (C) 2023 FriendshipEnder. */
