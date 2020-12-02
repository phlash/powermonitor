# Powermonitor on an HCS08AW32

Re-born from my 2012 work using closed source CodeWarrior tooling,
this is the firmware (and supporting Linux apps) for an HCS08AW32
MCU using open source tools:

 * SDCC, which supports S08 targets ok (maybe a little fugly).
 * USBDM, for flashing and debugging with my USBDM-CF-3.1 board.

## Building

You are going to need (on Debian or simlar):

 * `build-essentials` make, gcc, binutils
 * `sdcc`, the small-device C compiler
 * `usbdm`, driver software and applications for USBDM board,
   NB: not a standard package but maintained here:
   http://usbdm.sourceforge.net/USBDM_V4.12/html/index.html
 * `openjdk-11-jdk` 'cause I wrote the GUIs in Java, so sue me :)

## Debugging

Not really got into this yet, but SDCC has emulators for S08
targets that might be useful, along with `sdcdb`. I need to
to figure out a remote mechanism via USBDM gdb server layer
to the real thing.

## But, what does it do?

It samples a mains current sensor / clamp around a domestic mains
input meter tail (not both, they would cancel out!), reporting
the sample value over serial data at 1kHz sample rate, so you
can plot waveforms (see choppers working) average the current over
a few seconds for logging... all on the other end of the serial
output (eg: RaspberryPi or NAS).

The supporting `serio` application reads the serial data, allows
other applications to read it over a network, averages it in 1
second periods and logs the average (RMS) values to file.

The PowerMeter.java GUI app connects to serio network port and
simply plots the raw waveform oscilloscope style

The RmsView.java GUI app reads the RMS log file(s) and provides
a pretty visualisation / graph that is explorable to see where
all the power goes in a domestic property - mine!

## Why?

Because. Also I now have a smart meter for gas, and wanted more
up to date figures for electric consumption..

Mostly it was an exercise in getting open source tooling to
play along with HCS08 MCU software development.

## Are there other ways?

It should be possible to jack the cable clamp / sensor directly
into a sound card (provided it samples down to 50Hz!) and avoid
all this messing about - too easy ;)

It would also be possible to connect to any other microcontroller
that has analogue-to-digital conversion capability (eg: that Raspi
you have lying around, or an Arduino)...

