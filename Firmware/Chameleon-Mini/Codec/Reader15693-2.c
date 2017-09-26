/*
 * Reader15693-2A.c
 *
 *  Created on: 25.09.2017
 *      Author: Phillip Nash
 */

#include "Reader15693-2.h"
#include "Codec.h"
#include "../System.h"
#include "../Application/Application.h"
#include "LEDHook.h"
#include "Terminal/Terminal.h"
#include <util/delay.h> 

#define ISO15693_PERIOD 128 //9.4us

static volatile struct {
    volatile bool StartRead;
    volatile bool RxDone;
    volatile bool RxPending;
} Flags = { 0 };

static volatile enum {
	STATE_IDLE
} State;

// GPIOR0 and 1 are used as storage for the timer value of the current modulation
#define LastBit			Codec8Reg2				// GPIOR2
// GPIOR3 is used for some internal flags
#define BitCount		CodecCount16Register1	// GPIOR5:4
#define SampleRegister	GPIOR6
#define BitCountUp		GPIOR7
#define CodecBufferIdx	GPIOR8
#define CodecBufferPtr	CodecPtrRegister2

void Reader15693CodecInit(void) 
{
    /* Initialize common peripherals and start listening
     * for incoming data. */
	CodecInitCommon();
	CodecSetDemodPower(true);

    CODEC_TIMER_SAMPLING.PER = ISO15693_PERIOD;
    CODEC_TIMER_SAMPLING.CCB = 1;
    CODEC_TIMER_SAMPLING.CTRLA = TC_CLKSEL_OFF_gc;
    CODEC_TIMER_SAMPLING.INTCTRLA = 0;
    CODEC_TIMER_SAMPLING.INTCTRLB = TC_CCBINTLVL_HI_gc;

    CODEC_TIMER_LOADMOD.CTRLA = 0;
    State = STATE_IDLE;

    Flags.StartRead = false;
    Flags.RxPending = false;
    Flags.RxDone = false;
}

ISR(CODEC_TIMER_SAMPLING_OVF_VECT) 
{
    
}

void Reader15693CodecDeInit(void) 
{
    CodecSetDemodPower(false);
	CodecReaderFieldStop();
	CODEC_TIMER_SAMPLING.CTRLA = 0;
	CODEC_TIMER_SAMPLING.INTCTRLB = 0;
	CODEC_TIMER_LOADMOD.CTRLA = 0;
	CODEC_TIMER_LOADMOD.INTCTRLB = 0;
	Flags.RxDone = false;
	Flags.RxPending = false;
	Flags.StartRead = false;
}


void Reader15693CodecTask(void) 
{
   // LED_PORT.OUTSET = LED_RED;
    /* Call application with received data */
    BitCount = ApplicationProcess(CodecBuffer, BitCount);

    if (BitCount > 0)
    {
        
    }
}

void Reader15693CodecStart(void) //Called when want to start a card read
{ 
    BitCount = 0;
	Flags.StartRead = true;

	CodecReaderFieldStart();
}

void Reader15693CodecReset(void) 
{
   // Reader14443A_EOC(); // this breaks every interrupt etc.
	State = STATE_IDLE;
	Flags.RxDone = false;
	Flags.StartRead = false;
	CodecReaderFieldStop();
}
