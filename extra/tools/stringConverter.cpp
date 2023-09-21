/******************** F r i e n d s h i p E n d e r ********************
 * 本程序隶属于 Readguy 开源项目, 请尊重开源开发者, 也就是我FriendshipEnder.
 * 如果有条件请到 extra/artset/reward 中扫描打赏,否则请在 Bilibili 上支持我.
 * 项目交流QQ群: 926824162 (萌新可以进来问问题的哟)
 * 郑重声明: 未经授权还请不要商用本开源项目编译出的程序.
 * 
 * @file stringConverter.cpp
 * @author FriendshipEnder (f_ender@163.com), Bilibili: FriendshipEnder
 * @version 1.0
 * @date 2023-09-18
 * @brief 可以将文本文件转换为字符串格式, 方便在程序中调用.
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
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

void split_filename(const string &t, string &t1, string &t2, string &t3){
  string::size_type iPos=(t.find_last_of('\\')+1)==0?t.find_last_of('/')+1:t.find_last_of('\\')+1;
  if(iPos != string::npos){
    t1 = t .substr(iPos, t .length() - iPos);//获取带后缀的文件名
    t2 = t1.substr(0, t1.rfind("."));//获取不带后缀的文件名
    //t3 = t1.substr(t1.rfind("."),t1.length());//获取后缀名
    t3 = string("./") + t1 + (".txt");
  }
}
int main(int argc,char *argv[]) {
  int i=0;
  fstream rec_file;
  fstream out_file;
  string ImgName;
  string ImgNameNoTag;
  string ImgNameOutput;
  if(argc>=2){
    rec_file.open(argv[1],ios_base::in | ios_base::binary);
    split_filename(string(argv[1]),ImgName,ImgNameNoTag,ImgNameOutput);
  }
  else{
    string in_file_name;
    cout << "Text file to C-styled string." << endl << "Input File path: (UTF-8 encoded)" << endl;
    cin >> in_file_name;
    rec_file.open(in_file_name,ios_base::in | ios_base::binary);
    split_filename(in_file_name,ImgName,ImgNameNoTag,ImgNameOutput);
  }
  if(!rec_file.is_open()) return 1;
  rec_file.seekg(0,ios::end);
  size_t rec_file_sz = rec_file.tellg();
  rec_file.seekg(0,ios::beg);
  out_file.open(ImgNameOutput,ios_base::out);
  out_file << "const uint8_t "<< ImgNameNoTag <<"[" << rec_file_sz+1 << "] =" << endl << '\"';
  //out_file.setf(ios_base::hex,ios_base::basefield);
  cout << "File size: " << rec_file_sz << endl;
  int oneLine = 0;
  int unicode_status = 0;
  for(i=0;rec_file.good();i++){
    int got = rec_file.get();
    if(rec_file.eof()) {
      cout << "EOF found!!! " << i << endl;
      break;
    }
    if(oneLine>=80 && (!unicode_status)) {
      out_file << '\"' << endl << '\"';
      oneLine=0;
    }
    else oneLine++;

    if(got == '\\' || got == '\'' || got == '\"' || got == '?'){
      out_file << '\\';
    }
    else if(got >= 0x80 && got <0xc0 && unicode_status){
      unicode_status --;
    }
    else if(got >= 0xc0 && got <0xe0){
      unicode_status =1;
    }
    else if(got >= 0xe0 && got <0xf0){
      unicode_status =2;
    }
    else if(got >= 0xf0 && got <0xf8){
      unicode_status =3;
    }
    else if(got >= 0xf8 && got <0xff){
      unicode_status =4;
    }
    else if(got <0x80){
      unicode_status =0;
    }
    if(got != '\n' && got != '\r' && got != '\t'){
      out_file << (char)got;
    }
    else{ //是\n \r \t
      if(got == '\n') out_file << "\\n";
      if(got == '\r') out_file << "\\r";
      if(got == '\t') out_file << "\\t";
    }
  }
  out_file << "\";" << endl;
  out_file.close();
  rec_file.close();
  return 0;
}