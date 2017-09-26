#include "Reader15693.h"
#include "ISO15693-A.h"
#include "Application.h"
#include "../Codec/Reader15693-2.h"
#include "Crypto1.h"
#include "../System.h"
#include "../Terminal/Terminal.h"

Reader15693Command Reader15693CurrentCommand = Reader15693_Do_Nothing;

#define SOF_1_OF_4_CODE 0x81
#define SOF_1_OF_256_CODE 0x84
#define EOF_CODE 0x02


//Second field flags as specified in page 9 of the ISO15693-3
#define DUAL_SUB_CARRIER_FLAG   0x01
#define HIGH_DATA_RATE_FLAG     0x02
#define INVENTORY_FLAG          0x04
#define PROTOCOL_EXTENSION_FLAG 0x08

//Flags used when inventory flag has NOT been set
#define SELECT_FLAG             0x10
#define ADDRESS_FLAG            0x20
#define OPTION_FLAG             0x40

//Flags used when inventory flag HAS been set
#define AFI_FLAG                0x10
#define NB_ONE_SLOT             0x20
#define OPTION_FLAG             0x40

static enum {
	STATE_IDLE,
	STATE_HALT,
	STATE_READY,
	STATE_END
} ReaderState = STATE_IDLE;

void Reader15693AppTimeout(void)
{
	Reader15693AppReset();
	Reader15693CodecReset();
	ReaderState = STATE_IDLE;
}

void Reader15693AppInit(void) {
    
}

void Reader15693AppReset(void) {
    Reader15693CurrentCommand = Reader15693_Do_Nothing;
}

void Reader15693AppTask(void) {

}

void Reader15693AppTick(void) {

}

uint16_t InsertSOF(uint8_t* Buffer, uint8_t SOFCode, uint16_t ByteCount)
{
    Buffer[0] = SOFCode;
    ++ByteCount;
    return ByteCount;
}

uint16_t InsertFlags(uint8_t* Buffer, uint8_t Flags, uint16_t ByteCount)
{
    Buffer[1] = Flags;
    ++ByteCount;
    return ByteCount;
}

uint16_t Reader15693AppProcess(uint8_t* Buffer, uint16_t ByteCount) {
    
    switch (Reader15693CurrentCommand)
	{
        case Reader15693_Identify:
        {
            ByteCount = InsertSOF(Buffer, SOF_1_OF_4_CODE, ByteCount);
            ByteCount = InsertFlags(Buffer, SOF_1_OF_4_CODE, ByteCount);
            
            Reader15693CodecStart();
        }
        
        default:
            return 0;
    }
    
    CodecReaderFieldStop();
    
    return ByteCount;
}

