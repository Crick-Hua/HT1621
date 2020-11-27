/*******************************************************************************
Copyright 2016-2018 anxzhu (github.com/anxzhu)
Copyright 2018-2020 Valerio Nappi (github.com/5N44P) (changes)
Based on segment-lcd-with-ht1621 from anxzhu (2016-2018)
(https://github.com/anxzhu/segment-lcd-with-ht1621)

Partially rewritten and extended by Valerio Nappi (github.com/5N44P) in 2018

This file is part of the HT1621 arduino library, and thus under the MIT license.
More info on the project and the license conditions on :
https://github.com/5N44P/ht1621-7-seg

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
of the Software, and to permit persons to whom the Software is furnished to do
so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

*******************************************************************************/

#include <Arduino.h>
#include "HT1621.h"

HT1621::HT1621(){
		_buffer[0] = 0x00;  //更新三个字符数组，显示电压。
		_buffer[1] = 0x00;
		_buffer[2] = 0x00;
	    _buffer[3] = 0x00;  //更新三个符号数组，显示电池以及图标。
		_buffer[4] = 0x00;
		_buffer[5] = 0x00;
		/*0,1,2,3,4,5,6,7,8,9,A,b,C,c,d,E,F,H,h,L,n,N,o,P,r,t,U,-, ,*/
}

void HT1621::begin(int cs_p,int wr_p,int data_p,int backlight_p)
{
	pinMode(cs_p, OUTPUT);
	pinMode(wr_p, OUTPUT);
	pinMode(data_p, OUTPUT);
	pinMode(backlight_p, OUTPUT);
	_cs_p=cs_p;
	_wr_p=wr_p;
	_data_p=data_p;
	_backlight_p=backlight_p;
	_backlight_en=true;
	config();

}

void HT1621::begin(int cs_p,int wr_p,int data_p)
{
	pinMode(cs_p, OUTPUT);
	pinMode(wr_p, OUTPUT);
	pinMode(data_p, OUTPUT);
	_cs_p=cs_p;
	_wr_p=wr_p;
	_data_p=data_p;
	_backlight_en = false;
	config();
}

/*********************************************** 
 *送cnt位数据d;从高位开始送 ，从COM0到COM3. 
 *  ********************************************/

void HT1621::wrDATA(unsigned char data, unsigned char cnt) {
	unsigned char i;
	for (i = 0; i < cnt; i++) {
		digitalWrite(_wr_p, LOW);
		delayMicroseconds(4);
		if (data & 0x80) {
			digitalWrite(_data_p, HIGH);
		}
		else
		{
			digitalWrite(_data_p, LOW);
		}
		digitalWrite(_wr_p, HIGH);
		delayMicroseconds(4);
		data <<= 1;
	}
}
/*********************************************************
//送d_count位数据d;从低位开始送，从COM3到COM0
/*********************************************************/
void HT1621::wrDATAL(unsigned char data, unsigned char cnt) {
	unsigned char i;
	for (i = 0; i < cnt; i++) {
		digitalWrite(_wr_p, LOW);
		delayMicroseconds(4);
		if (data & 0x01) {
			digitalWrite(_data_p, HIGH);
		}
		else
		{
			digitalWrite(_data_p, LOW);
		}
		digitalWrite(_wr_p, HIGH);
		delayMicroseconds(4);
		data >>= 1;
	}
}

//送4位数据（对应一个SEG），com0对应的低位，com3对应高位，低到高
void HT1621::wrclrdata_4Bit(unsigned char addr, unsigned char sdata)
{
	addr <<= 2;
	digitalWrite(_cs_p, LOW);
	wrDATA(0xa0, 3);
	wrDATA(addr, 6);
	wrDATAL(sdata, 4);   
	digitalWrite(_cs_p, HIGH);
}

//送8位数据（对应两个个SEG），com0对应的高位，com3对应低位，低到高
void HT1621::wrclrdata(unsigned char addr, unsigned char sdata)
{
	addr <<= 2;
	digitalWrite(_cs_p, LOW);
	wrDATA(0xa0, 3);
	wrDATA(addr, 6);
	wrDATA(sdata, 8);
	digitalWrite(_cs_p, HIGH);
}

void HT1621::display()
{
	wrCMD(LCDON);
}

void HT1621::noDisplay()
{
	wrCMD(LCDOFF);
}

void HT1621::wrone(unsigned char addr, unsigned char sdata)
{
	addr <<= 2;
	digitalWrite(_cs_p, LOW);
	wrDATA(0xa0, 3);
	wrDATA(addr, 6);
	wrDATA(sdata, 8);
	digitalWrite(_cs_p, HIGH);
}

void HT1621::backlight()
{
	if (_backlight_en)
		digitalWrite(_backlight_p, HIGH);
	delay(1);
}
void HT1621::noBacklight()
{
	if(_backlight_en)
		digitalWrite(_backlight_p, LOW);
	delay(1);
}

void HT1621::wrCMD(unsigned char CMD) {  //100写命令
	digitalWrite(_cs_p, LOW);
	wrDATA(0x80, 4);
	wrDATA(CMD, 8);
	digitalWrite(_cs_p, HIGH);
}

void HT1621::config()
{
	wrCMD(BIAS);
	wrCMD(RC256);
	wrCMD(SYSDIS);
	wrCMD(WDTDIS1);
	wrCMD(SYSEN);
	wrCMD(LCDON);

}
void HT1621::wrCLR(unsigned char len) {
	unsigned char addr = 0;
	unsigned char i;
	for (i = 0; i < len; i++) {
		wrclrdata(addr, 0x00);
		addr = addr + 2;
	}
}



void HT1621::clear(){

	wrCLR(16);

}

void HT1621::update(){ // 更新电压值,显示三个数码管
        //提取缓冲区并将其直接放入驱动程序
		// the buffer is backwards with respect to the lcd. could be improved
		// 缓冲区相对于lcd向后。可以改善

		wrone(3, _buffer[0]); //送八位
		wrone(5, _buffer[1]);
		wrone(7, _buffer[2]);

}

void HT1621::updateC(){ // 更新符号值，电池以及图标
        //提取缓冲区并将其直接放入驱动程序
		// the buffer is backwards with respect to the lcd. could be improved
		// 缓冲区相对于lcd向后。可以改善

		wrclrdata_4Bit(0, _buffer[3]);  //送4位
		wrclrdata_4Bit(1, _buffer[4]);
		wrclrdata_4Bit(2, _buffer[5]);

}

 void HT1621::displayVcc(float number){  //显示99.9-0.0
  int n = number * 10;
  if(n < 100 ){
    _buffer[0] = 0x0;
    _buffer[1] = _digitalnum[n/10] + 0x1;  //0x1表示显示小数点
    _buffer[2] = _digitalnum[n%10] + 0x1; // 0x1表示显示‘V’
  }else if(n >=100){
    _buffer[0] = _digitalnum[n/100];
    _buffer[1] = _digitalnum[(n%100)/10] + 0x1;  
    _buffer[2] = _digitalnum[n%100%10]   + 0x1;

  }
  update();
 }

void HT1621::display_Battery(int num){  //显示电池电量

     switch(num){
      case 0://没电，一格电都没有
             wrclrdata_4Bit(0,0x8);
             _buffer[3] = _buffer[3] | 0x8 & 0xB;
             _buffer[4] &= 0x3;
             _buffer[5] &= 0x3;
             break;
      case 1://一格电

             _buffer[3] = _buffer[3] | 0x8 & 0xB;
             _buffer[4] = _buffer[4] | 0x8 & 0xB;
             _buffer[5] &= 0x3;
             break;
      case 2: //2格电

             _buffer[3] = _buffer[3] | 0x8 & 0xB;
             _buffer[4] |= 0xC;
             _buffer[5] &= 0x3;
             break;
      case 3://3格电

             _buffer[3] = _buffer[3] | 0x8 & 0xB;
             _buffer[4] |= 0xC;
             _buffer[5] = _buffer[5] | 0x4 & 0x7;
             break;
      case 4://4电格电
 
             _buffer[3] = _buffer[3] | 0x8 & 0xB;
             _buffer[4] |= 0xC;
             _buffer[5] |= 0xC;
             break;
      case 5://5格电
 
             _buffer[3] |= 0xC;
             _buffer[4] |= 0xC;
             _buffer[5] |= 0xC;
             break;
      default: break;
     }
     updateC();
 }

 void HT1621::display_Char(char num){  //显示字符
     switch(num){
      case 'H'://显示H
               //Write_1621_4Bit(0,0x1);
               _buffer[3] |= 0x1;
               break;
      case 'A'://显示A
               //Write_1621_4Bit(0,0x2);
               _buffer[3] |= 0x2;
               break;
      case 'P'://显示小数点
               //Write_1621_4Bit(6,0x8);
               _buffer[1] += 0x1;
               break;
      case 'V'://显示V
               //Write_1621_4Bit(8,0x8);
               _buffer[2] += 0x1;
               break;
      case 'O'://显示太阳符号
               //Write_1621_4Bit(2,0x1);
               _buffer[5] |= 0x1;
               break;
      case 'L'://显示S2（指向电池符号）
               //Write_1621_4Bit(2,0x2);
               _buffer[5] |= 0x2;
               break;
      case 'R'://显示S9（指向负载符号）
               //Write_1621_4Bit(1,0x2);
               _buffer[4] |= 0x2;
               break;
      case 'D'://显示负载
               //Write_1621_4Bit(1,0x1);
               _buffer[4] |= 0x1;
               break;
      case '%': //显示%
               //Write_1621_4Bit(4,0x8);
               _buffer[0] += 0x1;
               break;
      default: break;
     }
     update();
     updateC();
}


 void HT1621::undisplay_Char(char num){  //熄灭字符
     switch(num){
      case 'H'://显示H
               //Write_1621_4Bit(0,0x1);
               _buffer[3] &= ~0x1;
               break;
      case 'A'://显示A
               //Write_1621_4Bit(0,0x2);
               _buffer[3] &= ~0x2;
               break;
      case 'P'://显示小数点
               //Write_1621_4Bit(6,0x8);
               _buffer[1] &= ~0x1;
               break;
      case 'V'://显示V
               //Write_1621_4Bit(8,0x8);
               _buffer[2] &= ~0x1;
               break;
      case 'O'://显示太阳符号
               //Write_1621_4Bit(2,0x1);
               _buffer[5] &= ~0x1;
               break;
      case 'L'://显示S2（指向电池符号）
               //Write_1621_4Bit(2,0x2);
               _buffer[5] &= ~0x2;
               break;
      case 'R'://显示S9（指向负载符号）
               //Write_1621_4Bit(1,0x2);
               _buffer[4] &= ~0x2;
               break;
      case 'D'://显示负载
               //Write_1621_4Bit(1,0x1);
               _buffer[4] &= ~0x1;
               break;
      case '%': //显示%
               //Write_1621_4Bit(4,0x8);
               _buffer[0] &= ~0x1;
               break;
      default: break;
     }
     update();
     updateC();
}