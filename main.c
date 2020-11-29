#include "mc9s08aw32.h"

volatile uint16_t ticks=0;
volatile uint8_t disp[8]={0x9f,0x3e,0x9a,0xba, 0x1b,0x3e,0x8e,0x15}; // A5hbY5F7 (AshbySoft)? :)
volatile uint8_t dpos=0;
volatile int16_t aval=0;
volatile uint8_t adone=0;

void timer(void) __interrupt VectorNumber_Vtpm1ovf {
	// acknowledge interrupt, must read then write status
	uint8_t t = TPM1SC;
	TPM1SC_TOF = 0;
	// all displays off while we change stuff
	PTAD = 0xff;
	// new character, with 'dot' if there is a bit set in adc value
	// this provides a rough visual 'power bar'
	int16_t a = (aval<0? -aval : aval) >>2;
	PTBD = disp[dpos] | ((uint8_t)a&(1<<dpos) ? 0x40: 0x0);
	// turn on display (reversed bitfield due to physical layout)
	PTAD = ~(1<<(7-dpos));
	// move display pos (modulo length)
	dpos = (dpos+1)&0x7;
	// kick off ADC
	ADC1SC1_ADCH = 15;
	// tick counter
	++ticks;
}

void adc(void) __interrupt VectorNumber_Vadc1 {
	// read value (acks interrupt)
	static int16_t val;
	uint8_t h = ADC1RH;
	uint8_t l = ADC1RL;
	// we perform unsigned bit shifting / or'ing together the bytes
	// to form a 10-bit ADC value (always positive), which we
	// can also read as a signed value (same bits since positive)
	// to perform arithmetic (subtraction) without sign-extending
	// happening magically and breaking stuff.
	union {
		uint16_t us;
		int16_t ss;
	} rv;
	rv.us = ((unsigned short)h << 8) | (unsigned short)l;
	if (ADC1SC1_ADCH==15) {
		// retain positive sample, kick off negative ADC
		val = rv.ss;
		ADC1SC1_ADCH=14;
	} else {
		// calculate final differential sample value
		aval = val - rv.ss;
	}
}

void serial(char *msg) {
	// ASCIIZ string..
	while (*msg) {
		// busy wait for transmit empty
		while (0==SCI1S1_TDRE)
			__RESET_WATCHDOG();
		// write next char
		SCI1D = *msg;
		msg++;
	}
}

char *hex(signed short val) {
	static char map[16] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
	static char out[5];
	char pos=0;
	out[3] = map[val&0xf];
	val>>=4;
	out[2] = map[val&0xf];
	val>>=4;
	out[1] = map[val&0xf];
	val>>=4;
	out[0] = map[val&0xf];
	out[4] = 0;
	return out;
}

void main(void) {
	ICGC2_MFD = 0x03; // x10 FLLIN where FLLIN = ref * 64/7
	ICGC2_RFD = 0x01; // /2 FLL out
	ICGC1_CLKS = 0x01;// FLL engaged, internal ref
	SCI1BDL = 3;    // 5.554M/(16*3)~=115200 (115708 actually, but lowest error from spreadsheet analysis)
	SCI1C2_TE = 1;
	SCI1C2_RE = 1;
	TPM1MODH = (5553)>>8;// modulo counter = 0->5553 inclusive (5554 values)
	TPM1MODL = (5553)&0xff;
	TPM1SC_CLKSA = 1;   // busclk
	TPM1SC_TOIE = 1;    // enable interrupt
	ADC1CFG_MODE = 0x02;// 10-bit conversion
	ADC1CFG_ADIV = 0x01;// ADC clk = busclk/2 (~2.5MHz)
	ADC1SC1_AIEN = 1;   // enable interrupt
	ADPC14 = 1;			// disable GPIO on sampling pins
	ADPC15 = 1;
	EnableInterrupts;
	// initial display, blank
	PTAD = 0xff;
	PTADD= 0xff;
	PTBD = 0x00;
	PTBDD= 0xff;
	while(1) {
		signed short v = 0x8000;	// MIN_SHORT
		__asm__("WAIT\n");
		__RESET_WATCHDOG();
		DisableInterrupts;
		if (adone) {
			// we have a new sample - grab it
			v = aval;
			adone=0;
		}
		EnableInterrupts;
		if (v!=0x8000) {
			serial(hex(v));
			serial("\n");
		}
	}
}
