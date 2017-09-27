/*
 * Reader15693-2A.h
 *
 *  Created on: 11.04.2017
 *      Author: Phillip Nash
 */

#ifndef READER15693_2_H_
#define READER15693_2_H_

#include "Codec.h"
#include "Terminal/CommandLine.h"

#define SOF_1_OF_4_CODE         0x81
#define SOF_1_OF_256_CODE       0x84
#define EOF_CODE                0x02

/* Codec Interface */
void Reader15693CodecInit(void);
void Reader15693CodecDeInit(void);
void Reader15693CodecTask(void);

/* Application Interface */
void Reader15693CodecStart(void);
void Reader15693CodecReset(void);

#endif /* READER15693_2_H_ */
