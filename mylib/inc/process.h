
#ifndef __PROCESS_H_
#define __PROCESS_H_ 1

#include "stm32f4xx.h"
extern const u8 sf21[];
extern const u8 sf22[];
extern const u8 sf30[];
extern const u8 sf31[];
extern const u8 sf40[];
extern const u8 sf41[];
extern const u8 sf42[];
extern const u8 sf51[];

extern const u8 f20[];
extern const u8 f21[];

extern const u8 f30[];
extern const u8 f31[];
extern const u8 f32[];
extern const u8 f40[];
extern const u8 f41[];
extern const u8 f42[];
extern const u8 f43[];
extern const u8 f50[];
extern const u8 f51[];
extern const u8 f52[];
extern const u8 f54[];
extern const u8 f60[];
extern const u8 f62[];
extern const u8 f71[];
extern const u8 f81[];
extern const u8 f82[];

void CharToStr(u8 wei,u8 *str,u8 ivar);
void SPuti(u8 wei,u8 *str,u16 ivar);
void SPutl(u8 wei,u8 *str,u32 lvar);
void SPutf(const u8 *format,u8 *str,float fvar);

#endif /*__PROCESS_H_*/
