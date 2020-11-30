# Powermonitor on an HCS08AW32

Re-born from my 2012 work using closed source CodeWarrior tooling,
this is the firmware for an HCS08AW32 MCU using open source tools:

 * SDCC, which supports S08 targets ok (maybe a little fugly).
 * USBDM, for flashing and debugging with my USBDM-CF-3.1 board.

## Building

You are going to need (on Debian or simlar):

 * `build-essentials` make, possibly binutils..
 * `sdcc`, the small-device C compiler
 * `usbdm`, driver software and applications for USBDM board,
   not a standard package but maintained here:
   http://usbdm.sourceforge.net/USBDM_V4.12/html/index.html

## Debugging

Not really got into this yet, but SDCC has emulators for S08
targets that might be useful, along with `sdcdb`. I need to
to figure out a remote mechanism via USBDM gdb server layer
to the real thing.

## But, what does it do?

It samples a mains current sensor / clamp around one of my house
mains power cables (not both, they would cancel out!), reporting
the sample value over serial data at 1kHz sample rate, so you
can plot waveforms (see choppers working) average the power over
a few seconds for logging... all on the other end of the serial
output (eg: RaspberryPi or NAS).

## Why?

Because. Also I now have a smart meter for gas, and wanted more
up to date figures for electric consumption..

Mostly it was an exercise in getting open source tooling to
play along with MCU software development.

## Are there other ways?

It should be possible to jack the cable clamp / sensor directly
into a sound card (provided it samples down to 50Hz!) and avoid
all this messing about - too easy ;)

It would also be possible to connect to any other microcontroller
that has analogue-to-digital conversion capability (eg: that Raspi
you have lying around, or an Arduino)...

