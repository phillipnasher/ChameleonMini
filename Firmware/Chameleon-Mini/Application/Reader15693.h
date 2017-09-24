#ifndef READER15693_H
#define READER15693_H

#include "Application.h"
#include "Codec/Codec.h"

#define SOC_1_OF_4_CODE         0x7B
#define SOC_1_OF_256_CODE       0x7E
#define EOC_CODE                0xDF

uint8_t ReaderSendBuffer[CODEC_BUFFER_SIZE];
uint16_t ReaderSendBitCount;

void Reader15693AppInit(void);
void Reader15693AppReset(void);
void Reader15693AppTask(void);
void Reader15693AppTick(void);
void Reader15693AppTimeout(void);
uint16_t Reader15693AppProcess(uint8_t* Buffer, uint16_t BitCount);



#endif //READER15693_H
