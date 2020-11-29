// Register definitions sufficient for powermonitor..

#ifndef _MC9S08AW32_H
#define _MC9S08AW32_H
#include <stdint.h>

// we use SDCC ability to declare variables at absolute addressing and bitfields
// @see /usr/share/sdcc/include/hc08/mc68hc908qy.h for an SDCC-provided example

// default single bitfields, also shows us the bit order (implementation defined
// in ANSI-C specification) stolen from example header :)
struct __hc08_bits
{
  unsigned int bit0:1;
  unsigned int bit1:1;
  unsigned int bit2:1;
  unsigned int bit3:1;
  unsigned int bit4:1;
  unsigned int bit5:1;
  unsigned int bit6:1;
  unsigned int bit7:1;
};


// == System Reset Status
volatile __xdata uint8_t __at 0x1800 SRS;
#define __RESET_WATCHDOG()	(SRS=0)

// == Port A
volatile __data uint8_t __at 0x0000 PTAD;
volatile __data uint8_t __at 0x0001 PTADD;


// == Port B
volatile __data uint8_t __at 0x0002 PTBD;
volatile __data uint8_t __at 0x0003 PTBDD;


// == internal clock generator control 1
struct __icgc1_bits {
  unsigned int _unused0:1;
  unsigned int LOCD:1;
  unsigned int OSCSTEN:1;
  unsigned int CLKS:2;
  unsigned int REFS:1;
  unsigned int RANGE:1;
  unsigned int HGO:1;
};
volatile __data uint8_t __at 0x0048 ICGC1;
#define ICGC1_LOCD ((struct __icgc1_bits *)(&ICGC1))->LOCD
#define ICGC1_OSCSTEN ((struct __icgc1_bits *)(&ICGC1))->OSCSTEN
#define ICGC1_CLKS ((struct __icgc1_bits *)(&ICGC1))->CLKS
#define ICGC1_REFS ((struct __icgc1_bits *)(&ICGC1))->REFS
#define ICGC1_RANGE ((struct __icgc1_bits *)(&ICGC1))->RANGE
#define ICGC1_HGO ((struct __icgc1_bits *)(&ICGC1))->HGO

// == internal clock generator control 2
struct __icgc2_bits {
  unsigned int RFD:3;
  unsigned int LOCRE:1;
  unsigned int MFD:3;
  unsigned int LOLRE:1;
};
volatile __data uint8_t __at 0x0049 ICGC2;
#define ICGC2_RFD ((struct __icgc2_bits *)(&ICGC2))->RFD
#define ICGC2_LOCRE ((struct __icgc2_bits *)(&ICGC2))->LOCRE
#define ICGC2_MFD ((struct __icgc2_bits *)(&ICGC2))->MFD
#define ICGC2_LOLRE ((struct __icgc2_bits *)(&ICGC2))->LOLRE

// == serial communications interface 1
volatile __data uint8_t __at 0x0038 SCI1BDH;
volatile __data uint8_t __at 0x0039 SCI1BDL;
volatile __data uint8_t __at 0x003A SCI1C1;
#define SCI1C1_PT ((struct __hc08_bits *)(&SCI1C1))->bit0
#define SCI1C1_PE ((struct __hc08_bits *)(&SCI1C1))->bit1
#define SCI1C1_ILT ((struct __hc08_bits *)(&SCI1C1))->bit2
#define SCI1C1_WAKE ((struct __hc08_bits *)(&SCI1C1))->bit3
#define SCI1C1_M ((struct __hc08_bits *)(&SCI1C1))->bit4
#define SCI1C1_RSRC ((struct __hc08_bits *)(&SCI1C1))->bit5
#define SCI1C1_SCISWAI ((struct __hc08_bits *)(&SCI1C1))->bit6
#define SCI1C1_LOOPS ((struct __hc08_bits *)(&SCI1C1))->bit7
volatile __data uint8_t __at 0x003B SCI1C2;
#define SCI1C2_SBK ((struct __hc08_bits *)(&SCI1C2))->bit0
#define SCI1C2_RWU ((struct __hc08_bits *)(&SCI1C2))->bit1
#define SCI1C2_RE ((struct __hc08_bits *)(&SCI1C2))->bit2
#define SCI1C2_TE ((struct __hc08_bits *)(&SCI1C2))->bit3
#define SCI1C2_ILIE ((struct __hc08_bits *)(&SCI1C2))->bit4
#define SCI1C2_RIE ((struct __hc08_bits *)(&SCI1C2))->bit5
#define SCI1C2_TCIE ((struct __hc08_bits *)(&SCI1C2))->bit6
#define SCI1C2_TIE ((struct __hc08_bits *)(&SCI1C2))->bit7
volatile __data uint8_t __at 0x003C SCI1S1;
#define SCI1S1_PF ((struct __hc08_bits *)(&SCI1S1))->bit0
#define SCI1S1_FE ((struct __hc08_bits *)(&SCI1S1))->bit1
#define SCI1S1_NF ((struct __hc08_bits *)(&SCI1S1))->bit2
#define SCI1S1_OR ((struct __hc08_bits *)(&SCI1S1))->bit3
#define SCI1S1_IDLE ((struct __hc08_bits *)(&SCI1S1))->bit4
#define SCI1S1_RDRF ((struct __hc08_bits *)(&SCI1S1))->bit5
#define SCI1S1_TC ((struct __hc08_bits *)(&SCI1S1))->bit6
#define SCI1S1_TDRE ((struct __hc08_bits *)(&SCI1S1))->bit7
#define SCI1S1_PF_MASK		0x01
#define SCI1S1_FE_MASK		0x02
#define SCI1S1_NF_MASK		0x04
#define SCI1S1_OR_MASK		0x08
#define SCI1S1_IDLE_MASK	0x10
#define SCI1S1_RDRF_MASK	0x20
#define SCI1S1_TC_MASK		0x40
#define SCI1S1_TDRE_MASK	0x80
volatile __data uint8_t __at 0x003D SCI1S2;
#define SCI1S2_RAF ((struct __hc08_bits *)(&SCI1S2))->bit0
#define SCI1S2_BRK13 ((struct __hc08_bits *)(&SCI1S2))->bit2
volatile __data uint8_t __at 0x003E SCI1C3;
#define SCI1C3_PEIE ((struct __hc08_bits *)(&SCI1C3))->bit0
#define SCI1C3_FEIE ((struct __hc08_bits *)(&SCI1C3))->bit1
#define SCI1C3_NEIE ((struct __hc08_bits *)(&SCI1C3))->bit2
#define SCI1C3_OEIE ((struct __hc08_bits *)(&SCI1C3))->bit3
#define SCI1C3_TXINV ((struct __hc08_bits *)(&SCI1C3))->bit4
#define SCI1C3_TXDIR ((struct __hc08_bits *)(&SCI1C3))->bit5
#define SCI1C3_T8 ((struct __hc08_bits *)(&SCI1C3))->bit6
#define SCI1C3_R8 ((struct __hc08_bits *)(&SCI1C3))->bit7
volatile __data uint8_t __at 0x003F SCI1D;

// == timer / pwm module 1
volatile __data uint8_t __at 0x0020 TPM1SC;
#define TPM1SC_PS0 ((struct __hc08_bits *)(&TPM1SC))->bit0
#define TPM1SC_PS1 ((struct __hc08_bits *)(&TPM1SC))->bit1
#define TPM1SC_PS2 ((struct __hc08_bits *)(&TPM1SC))->bit2
#define TPM1SC_CLKSA ((struct __hc08_bits *)(&TPM1SC))->bit3
#define TPM1SC_CLKSB ((struct __hc08_bits *)(&TPM1SC))->bit4
#define TPM1SC_CPWMS ((struct __hc08_bits *)(&TPM1SC))->bit5
#define TPM1SC_TOIE ((struct __hc08_bits *)(&TPM1SC))->bit6
#define TPM1SC_TOF ((struct __hc08_bits *)(&TPM1SC))->bit7
volatile __data uint8_t __at 0x0021 TPM1CNTH;
volatile __data uint8_t __at 0x0022 TPM1CNTL;
volatile __data uint8_t __at 0x0023 TPM1MODH;
volatile __data uint8_t __at 0x0024 TPM1MODL;
volatile __data uint8_t __at 0x0025 TPM1C0SC;
#define TPM1_ELS0A ((struct __hc08_bits *)(&TPM1C0SC))->bit2
#define TPM1_ELS0B ((struct __hc08_bits *)(&TPM1C0SC))->bit3
#define TPM1_MS0A ((struct __hc08_bits *)(&TPM1C0SC))->bit4
#define TPM1_MS0B ((struct __hc08_bits *)(&TPM1C0SC))->bit5
#define TPM1_CH0IE ((struct __hc08_bits *)(&TPM1C0SC))->bit6
#define TPM1_CH0F ((struct __hc08_bits *)(&TPM1C0SC))->bit7
volatile __data uint8_t __at 0x0026 TPM1C0VH;
volatile __data uint8_t __at 0x0027 TPM1C0VL;
volatile __data uint8_t __at 0x0028 TPM1C1SC;
#define TPM1_ELS1A ((struct __hc08_bits *)(&TPM1C1SC))->bit2
#define TPM1_ELS1B ((struct __hc08_bits *)(&TPM1C1SC))->bit3
#define TPM1_MS1A ((struct __hc08_bits *)(&TPM1C1SC))->bit4
#define TPM1_MS1B ((struct __hc08_bits *)(&TPM1C1SC))->bit5
#define TPM1_CH1IE ((struct __hc08_bits *)(&TPM1C1SC))->bit6
#define TPM1_CH1F ((struct __hc08_bits *)(&TPM1C1SC))->bit7
volatile __data uint8_t __at 0x0029 TPM1C1VH;
volatile __data uint8_t __at 0x002A TPM1C1VL;
volatile __data uint8_t __at 0x002B TPM1C2SC;
#define TPM1_ELS2A ((struct __hc08_bits *)(&TPM1C2SC))->bit2
#define TPM1_ELS2B ((struct __hc08_bits *)(&TPM1C2SC))->bit3
#define TPM1_MS2A ((struct __hc08_bits *)(&TPM1C2SC))->bit4
#define TPM1_MS2B ((struct __hc08_bits *)(&TPM1C2SC))->bit5
#define TPM1_CH2IE ((struct __hc08_bits *)(&TPM1C2SC))->bit6
#define TPM1_CH2F ((struct __hc08_bits *)(&TPM1C2SC))->bit7
volatile __data uint8_t __at 0x002C TPM1C2VH;
volatile __data uint8_t __at 0x002D TPM1C2VL;
volatile __data uint8_t __at 0x002E TPM1C3SC;
#define TPM1_ELS3A ((struct __hc08_bits *)(&TPM1C3SC))->bit2
#define TPM1_ELS3B ((struct __hc08_bits *)(&TPM1C3SC))->bit3
#define TPM1_MS3A ((struct __hc08_bits *)(&TPM1C3SC))->bit4
#define TPM1_MS3B ((struct __hc08_bits *)(&TPM1C3SC))->bit5
#define TPM1_CH3IE ((struct __hc08_bits *)(&TPM1C3SC))->bit6
#define TPM1_CH3F ((struct __hc08_bits *)(&TPM1C3SC))->bit7
volatile __data uint8_t __at 0x002F TPM1C3VH;
volatile __data uint8_t __at 0x0030 TPM1C3VL;
volatile __data uint8_t __at 0x0031 TPM1C4SC;
#define TPM1_ELS4A ((struct __hc08_bits *)(&TPM1C4SC))->bit2
#define TPM1_ELS4B ((struct __hc08_bits *)(&TPM1C4SC))->bit3
#define TPM1_MS4A ((struct __hc08_bits *)(&TPM1C4SC))->bit4
#define TPM1_MS4B ((struct __hc08_bits *)(&TPM1C4SC))->bit5
#define TPM1_CH4IE ((struct __hc08_bits *)(&TPM1C4SC))->bit6
#define TPM1_CH4F ((struct __hc08_bits *)(&TPM1C4SC))->bit7
volatile __data uint8_t __at 0x0032 TPM1C4VH;
volatile __data uint8_t __at 0x0033 TPM1C4VL;
volatile __data uint8_t __at 0x0034 TPM1C5SC;
#define TPM1_ELS5A ((struct __hc08_bits *)(&TPM1C5SC))->bit2
#define TPM1_ELS5B ((struct __hc08_bits *)(&TPM1C5SC))->bit3
#define TPM1_MS5A ((struct __hc08_bits *)(&TPM1C5SC))->bit4
#define TPM1_MS5B ((struct __hc08_bits *)(&TPM1C5SC))->bit5
#define TPM1_CH5IE ((struct __hc08_bits *)(&TPM1C5SC))->bit6
#define TPM1_CH5F ((struct __hc08_bits *)(&TPM1C5SC))->bit7
volatile __data uint8_t __at 0x0035 TPM1C5VH;
volatile __data uint8_t __at 0x0036 TPM1C5VL;

// == analog to digital converter 1
struct __adcsc1_bits {
  unsigned int ADCH:5;
  unsigned int ADCO:1;
  unsigned int AIEN:1;
  unsigned int COCO:1;
};
volatile __data uint8_t __at 0x0010 ADC1SC1;
#define ADC1SC1_ADCH ((struct __adcsc1_bits *)(&ADC1SC1))->ADCH
#define ADC1SC1_ADCO ((struct __adcsc1_bits *)(&ADC1SC1))->ADCO
#define ADC1SC1_AIEN ((struct __adcsc1_bits *)(&ADC1SC1))->AIEN
#define ADC1SC1_COCO ((struct __adcsc1_bits *)(&ADC1SC1))->COCO
volatile __data uint8_t __at 0x0011 ADC1SC2;
#define ADC1SC2_ACFGT ((struct __hc08_bits *)(&ADC1SC2))->bit4
#define ADC1SC2_ACFE ((struct __hc08_bits *)(&ADC1SC2))->bit5
#define ADC1SC2_ADTRG ((struct __hc08_bits *)(&ADC1SC2))->bit6
#define ADC1SC2_ADACT ((struct __hc08_bits *)(&ADC1SC2))->bit7
volatile __data uint8_t __at 0x0012 ADC1RH;
volatile __data uint8_t __at 0x0013 ADC1RL;
volatile __data uint8_t __at 0x0014 ADC1CVH;
volatile __data uint8_t __at 0x0015 ADC1CVL;
struct __adccfg_bits {
  unsigned int ADICLK:2;
  unsigned int MODE:2;
  unsigned int ADLSMP:1;
  unsigned int ADIV:2;
  unsigned int ADLPC:1;
};
volatile __data uint8_t __at 0x0016 ADC1CFG;
#define ADC1CFG_ADICLK ((struct __adccfg_bits *)(&ADC1CFG))->ADICLK
#define ADC1CFG_MODE ((struct __adccfg_bits *)(&ADC1CFG))->MODE
#define ADC1CFG_ADLSMP ((struct __adccfg_bits *)(&ADC1CFG))->ADLSMP
#define ADC1CFG_ADIV ((struct __adccfg_bits *)(&ADC1CFG))->ADIV
#define ADC1CFG_ADLPC ((struct __adccfg_bits *)(&ADC1CFG))->ADLPC
volatile __data uint8_t __at 0x0017 APCTL1;
#define ADPC0 ((struct __hc08_bits *)(&APCTL1))->bit0
#define ADPC1 ((struct __hc08_bits *)(&APCTL1))->bit1
#define ADPC2 ((struct __hc08_bits *)(&APCTL1))->bit2
#define ADPC3 ((struct __hc08_bits *)(&APCTL1))->bit3
#define ADPC4 ((struct __hc08_bits *)(&APCTL1))->bit4
#define ADPC5 ((struct __hc08_bits *)(&APCTL1))->bit5
#define ADPC6 ((struct __hc08_bits *)(&APCTL1))->bit6
#define ADPC7 ((struct __hc08_bits *)(&APCTL1))->bit7
volatile __data uint8_t __at 0x0018 APCTL2;
#define ADPC8 ((struct __hc08_bits *)(&APCTL2))->bit0
#define ADPC9 ((struct __hc08_bits *)(&APCTL2))->bit1
#define ADPC10 ((struct __hc08_bits *)(&APCTL2))->bit2
#define ADPC11 ((struct __hc08_bits *)(&APCTL2))->bit3
#define ADPC12 ((struct __hc08_bits *)(&APCTL2))->bit4
#define ADPC13 ((struct __hc08_bits *)(&APCTL2))->bit5
#define ADPC14 ((struct __hc08_bits *)(&APCTL2))->bit6
#define ADPC15 ((struct __hc08_bits *)(&APCTL2))->bit7
volatile __data uint8_t __at 0x0019 APCTL3;
#define ADPC16 ((struct __hc08_bits *)(&APCTL3))->bit0
#define ADPC17 ((struct __hc08_bits *)(&APCTL3))->bit1
#define ADPC18 ((struct __hc08_bits *)(&APCTL3))->bit2
#define ADPC19 ((struct __hc08_bits *)(&APCTL3))->bit3
#define ADPC20 ((struct __hc08_bits *)(&APCTL3))->bit4
#define ADPC21 ((struct __hc08_bits *)(&APCTL3))->bit5
#define ADPC22 ((struct __hc08_bits *)(&APCTL3))->bit6
#define ADPC23 ((struct __hc08_bits *)(&APCTL3))->bit7


// == interrupt vector numbers according to SDCC
#define VectorNumber_Vsci1rx	17
#define VectorNumber_Vtpm1ovf	11
#define VectorNumber_Vadc1		23

// == helper macros for interrupt control
#define EnableInterrupts		__asm__("CLI\n")
#define DisableInterrupts		__asm__("SEI\n")

#endif //_MC9S08AW32_H
