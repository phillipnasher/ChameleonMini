#include "Reader15693.h"
#include "ISO15693-A.h"
#include "Application.h"
#include "../Codec/Reader15693-2.h"
#include "Crypto1.h"
#include "../System.h"
#include "../Terminal/Terminal.h"

Reader15693Command Reader15693CurrentCommand = Reader15693_Do_Nothing;

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

uint16_t Reader15693AppProcess(uint8_t* Buffer, uint16_t ByteCount) {
    
    switch (Reader15693CurrentCommand)
	{
        case Reader15693_Identify:
        {
            Reader15693CodecStart();
        }
        
        default:
            return 0;
    }
    
    CodecReaderFieldStop();
    
    return ByteCount;
}

