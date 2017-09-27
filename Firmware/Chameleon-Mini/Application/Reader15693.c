#include "Reader15693.h"
#include "ISO15693-A.h"
#include "Application.h"
#include "../Codec/Reader15693-2.h"
#include "Crypto1.h"
#include "../System.h"
#include "../Terminal/Terminal.h"

Reader15693Command Reader15693CurrentCommand = Reader15693_Do_Nothing;


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

//
#define INVENTORY_CMD           0x01

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
    return ByteCount + 1;
}

uint16_t InsertEOF(uint8_t* Buffer, uint8_t EOFCode, uint16_t ByteCount)
{
    Buffer[ByteCount] = EOFCode;
    return ByteCount + 1;
}

uint16_t InsertFlags(uint8_t* Buffer, uint8_t Flags, uint16_t ByteCount)
{
    Buffer[0] = Flags;
    return ByteCount + 1;
}

uint16_t InsertCmd(uint8_t* Buffer, uint8_t Cmd, uint16_t ByteCount)
{
    Buffer[1] = Cmd;
    return ByteCount + 1;
}

uint16_t InsertData(uint8_t* Buffer, uint8_t Data, uint16_t ByteCount)
{
    Buffer[2] = Data;
    return ByteCount + 1;
}


void sendIntToTerminal(uint8_t val)
{
    char buf[16];
    snprintf(buf,16, "%d,",val);
    TerminalSendString(buf);
}

uint16_t Reader15693AppProcess(uint8_t* Buffer, uint16_t ByteCount) 
{
    
    switch (Reader15693CurrentCommand)
	{
        case Reader15693_Identify:
        {
            uint8_t i = 0;
            uint8_t RequestBuffer[5];
            
            ByteCount = 0;
            
            ByteCount = InsertFlags(RequestBuffer, 
                                    INVENTORY_FLAG | HIGH_DATA_RATE_FLAG | 
                                    NB_ONE_SLOT, ByteCount);
            ByteCount = InsertCmd(RequestBuffer, INVENTORY_CMD, ByteCount);
            ByteCount = InsertData(RequestBuffer, 0, ByteCount);
            
            ISO15693AppendCRC(RequestBuffer, ByteCount);
            ByteCount += ISO15693_CRC16_SIZE;
            
            ByteCount = InsertSOF(Buffer, SOC_1_OF_256_CODE, ByteCount);
            memcpy(Buffer + 1, RequestBuffer, 5);
            ByteCount = InsertEOF(Buffer, EOF_CODE, ByteCount);
            
            
            for (i = 0; i < ByteCount; i++) {
                    sendIntToTerminal(Buffer[i]);
            }

            Reader15693CodecStart();
        }
        
        default:
            return 0;
    }
    
    CodecReaderFieldStop();
    
    return ByteCount;
}

