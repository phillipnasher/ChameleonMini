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
    volatile bool Start;
    volatile bool RxDone;
    volatile bool RxPending;
} Flags = { 0 };

static volatile enum {
	STATE_IDLE
} State;

void Reader15693CodecInit(void) {
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

    Flags.Start = false;
    Flags.RxPending = false;
    Flags.RxDone = false;
}

ISR(CODEC_TIMER_SAMPLING_OVF_VECT) {
    
}

void Reader15693CodecDeInit(void) {
    CodecReaderFieldStop();
    CODEC_TIMER_SAMPLING.CTRLA = TC_CLKSEL_OFF_gc;
    CODEC_TIMER_SAMPLING.INTCTRLB = 0;
}


void Reader15693CodecTask(void) {
    LED_PORT.OUTSET = LED_RED;
}

void Reader15693CodecStart(void) {
    //CodecReaderFieldStart();
   // LED_PORT.OUTSET = LED_RED;
}

void Reader15693CodecReset(void) {
    
}
