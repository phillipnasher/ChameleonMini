/*
 * Reader15693-2A.c
 *
 *  Created on: 11.04.2017
 *      Author: Phillip Nash
 */

#include "Reader15693-2.h"
#include "Codec.h"
#include "../System.h"
#include "../Application/Application.h"
#include "LEDHook.h"
#include "Terminal/Terminal.h"
#include <util/delay.h> 

#define ISO15693_SAMPLE_PERIOD  128 //9.4us
#define ISO15693_SAMPLE_CLK     TC_CLKSEL_DIV2_gc //13.56MHz
#define StateRegister           Codec8Reg1
#define ByteCount               Codec8Reg2
#define CodecBufSize            Codec8Reg3
#define PPMPosition		        CodecCount16Register1
#define SampleRegister	        GPIOR6
#define SOC_1_OF_4_CODE         0x7B
#define SOC_1_OF_256_CODE       0x81
#define EOC_CODE                0xDF

#define PPM_PER_BYTE            (512 - 1)

static volatile enum {
    STATE_PPM_IDLE,
    STATE_PPM_SOF,
    STATE_PPM_1_OF_4,
    STATE_PPM_1_OF_256,
    STATE_PPM_EOF
} State;


void Reader15693CodecInit(void) {
    ByteCount = 0;
    CodecBufSize = 0;
    PPMPosition = 0;
    StateRegister = STATE_PPM_IDLE;
    
    
    /*
    
    StateRegister = STATE_PPM_IDLE;
    CODEC_TIMER_SAMPLING.PER = ISO15693_SAMPLE_PERIOD - 1;                     
    CODEC_TIMER_SAMPLING.CTRLA = ISO15693_SAMPLE_CLK;                     
    CODEC_TIMER_SAMPLING.INTFLAGS = TC0_OVFIF_bm;                     
    CODEC_TIMER_SAMPLING.INTCTRLA = TC_OVFINTLVL_HI_gc;
    */
}

ISR(CODEC_TIMER_SAMPLING_OVF_VECT) {
   
    static void * JumpTable[] = {
        [STATE_PPM_IDLE] = &&STATE_PPM_IDLE_LABEL,
        [STATE_PPM_SOF] = &&STATE_PPM_SOF_LABEL,
        [STATE_PPM_1_OF_4] = &&STATE_PPM_1_OF_4_LABEL,
        [STATE_PPM_1_OF_256] = &&STATE_PPM_1_OF_256_LABEL,
        [STATE_PPM_EOF] = &&STATE_PPM_EOF_LABEL
    };
    
    if ( (StateRegister >= STATE_PPM_IDLE) && (StateRegister <= STATE_PPM_EOF) ) {
        goto *JumpTable[StateRegister];
    } else {
        return;
    }
    
    STATE_PPM_IDLE_LABEL:
        SampleRegister = SOC_1_OF_256_CODE;
        ByteCount = 0;
    STATE_PPM_SOF_LABEL:
    
        if (SampleRegister & 0x80) {
            LED_PORT.OUTSET = LED_RED;
            _delay_us(9.44);
            LED_PORT.OUTCLR = LED_RED;
        } 
        
        SampleRegister <<= 1;
        ++ByteCount;
        
        if (ByteCount == 8) {
            PPMPosition = 0;
            ByteCount = 0;
            StateRegister = STATE_PPM_1_OF_256;
        } else {
            StateRegister = STATE_PPM_SOF;
        }
    
        return;
        
    STATE_PPM_1_OF_4_LABEL:
    
        return;
    
    STATE_PPM_1_OF_256_LABEL:
    
        if (PPMPosition == PPM_PER_BYTE) {
            PPMPosition = 0;
            if (ByteCount == CodecBufSize - 1) {
                StateRegister = STATE_PPM_EOF;
                SampleRegister = EOC_CODE;
                ByteCount = 0;
            } else {
                ++ByteCount;
            }
        } else {
            if (CodecBuffer[ByteCount] * 2 == PPMPosition) {
                LED_PORT.OUTSET = LED_RED;
                _delay_us(9.44);
                LED_PORT.OUTCLR = LED_RED; 
            }
        }
    
        ++PPMPosition;
    
        return;
    
    STATE_PPM_EOF_LABEL: 
        SampleRegister <<= 1;
        
        if (SampleRegister & 0x80) {
            //EOF PULSE. Output and shut off timer
            LED_PORT.OUTSET = LED_RED;
            _delay_us(9.44);
            LED_PORT.OUTCLR = LED_RED; 
            
            CODEC_TIMER_SAMPLING.CTRLA = TC_CLKSEL_OFF_gc;
            CODEC_TIMER_SAMPLING.INTCTRLB = 0;
        }
    
    return;
}

void Reader15693CodecDeInit(void) {
    CodecReaderFieldStop();
    CODEC_TIMER_SAMPLING.CTRLA = TC_CLKSEL_OFF_gc;
    CODEC_TIMER_SAMPLING.INTCTRLB = 0;
}


void Reader15693CodecTask(void) {
    
    if (StateRegister == STATE_PPM_IDLE) {
        CodecBufSize = ApplicationProcess(CodecBuffer, CodecBufSize);
        //Start the PPM timer
        CODEC_TIMER_SAMPLING.PER = ISO15693_SAMPLE_PERIOD - 1;                     
        CODEC_TIMER_SAMPLING.CTRLA = ISO15693_SAMPLE_CLK;                     
        CODEC_TIMER_SAMPLING.INTFLAGS = TC0_OVFIF_bm;                     
        CODEC_TIMER_SAMPLING.INTCTRLA = TC_OVFINTLVL_HI_gc;
    }
    
}

void Reader15693CodecStart(void) {
  //  CodecReaderFieldStart();
}

void Reader15693CodecReset(void) {
    
}
