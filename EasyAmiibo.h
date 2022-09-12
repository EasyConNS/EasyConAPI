#ifndef _EASY_AMIIBO_H_
#define _EASY_AMIIBO_H_

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define GAMEPAD_COLORDATA_SIZE 12
#define AMIIBO_DATASIZE 540

extern bool EasyCon_Unpair(void);
// 3:pro
// 2:jc-R
// 1:jc-L
extern bool EasyCon_ChangeMode(const byte mode);
// data.len must be 12
extern bool EasyCon_ChangeColor(uint8_t* data);
// amiibo bin size is 540
extern void EasyCon_SaveAmiibo(uint8_t* data);

#endif
