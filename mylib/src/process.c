
//--------include------------
#include "process.h"
#include "stm32f4xx.h"
const u8 sf21[]={'+','2','1'};
const u8 sf22[]={'+','2','2'};
const u8 sf30[]={'+','3','0'};
const u8 sf31[]={'+','3','1'};
const u8 sf40[]={'+','4','0'};
const u8 sf41[]={'+','4','1'};
const u8 sf42[]={'+','4','2'};
const u8 sf51[]={'+','5','1'};

const u8 f20[]={'2','0'};
const u8 f21[]={'2','1'};
const u8 f22[]={'2','2'};
const u8 f30[]={'3','0'};
const u8 f31[]={'3','1'};
const u8 f32[]={'3','2'};
const u8 f40[]={'4','0'};
const u8 f41[]={'4','1'};
const u8 f42[]={'4','2'};
const u8 f43[]={'4','3'};
const u8 f50[]={'5','0'};
const u8 f51[]={'5','1'};
const u8 f52[]={'5','2'};
const u8 f54[]={'5','4'};
const u8 f60[]={'6','0'};
const u8 f62[]={'6','2'};
const u8 f71[]={'7','1'};
const u8 f81[]={'8','1'};
const u8 f82[]={'8','2'};
//----------------------------------------
//        char数输出到字符串 函数      (无小数点)      
//入口: 位数:wei; 字符串:*str  变量:ivar  
//----------------------------------------
void CharToStr(u8 wei,u8 *str,u8 ivar) 
{
  u8 i,num=0;
  u8 intbuf;   // 整型数缓冲 
  intbuf = (u8)ivar;// 变整形     
  str[wei] = 0;       // 字符串结束 
  num = wei;
  for(i=0;i<wei;i++){
    
	num --;
	str[num] = intbuf % 10 + 0x30;// 送一个字符 
  intbuf = intbuf / 10;         // 上一位     
    }// for 
}


//----------------------------------------
//        整数输出到字符串 函数            
//入口: 位数:wei; 字符串:*str  变量:ivar  
//----------------------------------------
void SPuti(u8 wei,u8 *str,u16 ivar) 
{
  u8 i,num=0;
  u16 intbuf;   // 整型数缓冲 

  intbuf = (u16)ivar;// 变整形     
  str[wei] = 0;      // 字符串结束 
  num = wei;
  for(i=0;i<wei;i++){
    num --;
    str[num] = intbuf % 10 + 0x30;// 送一个字符 
    intbuf = intbuf / 10;         // 上一位     
    }// for 
}
//----------------------------------------
//        长整型整数输出到字符串 函数            
//入口: 位数:wei; 字符串:*str  变量:ivar  
//----------------------------------------
void SPutl(u8 wei,u8 *str,u32 lvar) 
{
  u8 i,num=0;
  u32 lbuf;   // 整型数缓冲 

  lbuf = (u32)lvar;// 变整形     
  str[wei] = 0;       // 字符串结束 
  num = wei;
  for(i=0;i<wei;i++){
    num --;
    str[num] = lbuf % 10 + 0x30;// 送一个字符 
    lbuf = lbuf / 10;         // 上一位     
    }// for 
}



//--------------------------------------------
// 浮点数输出到字符串                         
// 入口: 说明 *format, 字符串:*str 变量:fvar  
//--------------------------------------------
void SPutf(const u8 *format,u8 *str,float fvar)
{ 
  u8 a,b,i,num=0;
  unsigned long longbuf; // 整型数缓冲   

  if (*(format) =='+'){
    if (fvar<0){ 
      fvar = -fvar;    // 负变量转换绝对值  
      str[num]='-';   // 加负号            
      }
    else{ 
      str[num]='+'; // 加正号            
      }
    num ++;
    format ++;
    }
  a = *(format) - 0x30;    // 取总长度a     
  format ++;
  b = *(format) - 0x30;    // 取小数点位数b 
  for (i=0;i<b;i++){
    fvar = fvar * 10;    // 消除小数      
    }
  fvar = fvar + (float)0.50;     // 四舌五入      
  num = num + a;
  if (b>0){ 
    num ++;                  // 有小数点  
    }
  longbuf = (unsigned long)fvar;// 变整形 
  str[num] = 0;             // 字符串结束 
  for(i=1;i<=a;i++){
    num --;
    str[num] = longbuf % 10 + 0x30; // 送一个字符  
    longbuf = longbuf / 10;         // 上一位      
    if (i==b){ 
      num --;
      str[num] = '.';               // 加小数点    
      }
    }
}

