#include "Reader15693.h"
#include "ISO15693-A.h"
#include "Application.h"
#include "../Codec/Reader15693-2.h"
#include "Crypto1.h"
#include "../System.h"
#include "../Terminal/Terminal.h"

#define DUAL_SUB_CARRIER_REQ_FLAG           0x01
#define HIGH_DATA_RATE_REQ_FLAG             0x02  
#define INVENTORY_ENABLE_TAB_5_REQ_FLAG     0x04  
#define PROTOCOL_EXT_REQ_FLAG               0x08 

#define INVENTORY_TAB_4_SELECT_REQ_FLAG     0x10
#define INVENTORY_TAB_4_ADDR_REQ_FLAG       0x20
#define INVENTORY_TAB_4_OPTION_REQ_FLAG     0x40

#define INVENTORY_TAB_5_AFI_REQ_FLAG        0x10
#define INVENTORY_TAB_5_ONE_SLOT_REQ_FLAG   0x20
#define INVENTORY_TAB_5_OPTION_REQ_FLAG     0x40

#define INVENTORY_CMD_FLAG                  0x01
#define INVENTORY_STAY_QUIET_FLAG           0x02
#define INVENTORY_RFU_FLAG                  0x03


void Reader15693AppTimeout(void)
{
}

void Reader15693AppInit(void)
{
}

void Reader15693AppReset(void)
{
}

void Reader15693AppTask(void)
{

}

void Reader15693AppTick(void)
{

}

uint16_t AddReqFlags(uint8_t* Buffer, uint8_t Flags, uint16_t ByteCount) 
{
    Buffer[0] = Flags;
    return ByteCount + 1;
}

uint16_t AddCmdCode(uint8_t* Buffer, uint8_t CmdCode, uint16_t ByteCount) 
{
    Buffer[1] = CmdCode;
    return ByteCount + 1;
}

uint16_t AddParams(uint8_t* Buffer, uint8_t Params, uint16_t ByteCount) 
{
    Buffer[2] = Params;
    return ByteCount + 1;
}

uint16_t AddData(uint8_t* Buffer, uint8_t Data, uint16_t ByteCount) 
{
    Buffer[3] = Data;
    return ByteCount + 1;
}

uint16_t Reader15693AppProcess(uint8_t* Buffer, uint16_t ByteCount)
{
    ByteCount = AddReqFlags(Buffer, 
                             HIGH_DATA_RATE_REQ_FLAG | 
                             INVENTORY_ENABLE_TAB_5_REQ_FLAG | 
                             INVENTORY_TAB_5_ONE_SLOT_REQ_FLAG, ByteCount);
    ByteCount = AddCmdCode(Buffer, INVENTORY_CMD_FLAG, ByteCount);
    ByteCount = AddData(Buffer, 0, ByteCount);
    ByteCount = ISO15693AppendCRC(Buffer, ByteCount);
    return ByteCount;
}

