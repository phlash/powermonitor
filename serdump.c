/*A touch of serial I/O to dump bytes from a serial tap on the IHD WiFi connector.. */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <signal.h>
#include <time.h>

#define IHDBAUD	B115200
#define IHDDEV	"/dev/ttyUSB0"

static struct termios oio, tio = {0};
int open_serial(char *dev) {
	// Open serial device, avoid it becoming controlling terminal
	int fd = open(dev, O_RDWR | O_NOCTTY);
	if (fd<0) {
		perror("opening serial device");
		return -1;
	}

	// set new config
	tcgetattr(fd, &oio);
	tio.c_cflag = IHDBAUD | CS8 | CLOCAL | CREAD;
	tio.c_iflag = IGNPAR | ICRNL;
	tio.c_oflag = 0;
	tio.c_lflag = 0;
	tio.c_cc[VEOF] = 4;	// Ctrl-d (unused as ICANON not set)
	tio.c_cc[VMIN] = 1;	// Wait for at least one char per read

	// flush anything collected, and set new config
	tcflush(fd, TCIFLUSH);
	tcsetattr(fd, TCSANOW, &tio);
	return fd;
}

static int done=0;
void trap(int sig) {
	done=1;
}
int main(int argc, char **argv) {
	int arg, fd, n, d=0, bkg=0, l=0, s=0;
	char buf[128];
	char *dev = IHDDEV;
	char *out = NULL;

	for(arg=1; arg<argc; arg++) {
		if (argv[arg][1]=='d')
			dev = argv[++arg];
		else if (argv[arg][1]=='o')
			out = argv[++arg];
		else {
			puts("usage: serdump [-d <serial device>] [-o <outfile>]");
			return(0);
		}
	}
	signal(SIGINT, trap);

	// Redirect stdout if requested
	if (out) {
		if (freopen(out, "a", stdout)==NULL) {
			perror("opening log");
			return 1;
		}
	}

	// read bytes from serial port, dump 'em!
	fd = open_serial(dev);
	if (fd<0) {
		perror("opening device");
		return 2;
	}
    bkg = 0;
	while ((n=read(fd, buf, sizeof(buf)))>0) {
		fwrite(buf, 1, n, stdout);
		fflush(stdout);
		bkg+=n;
		fprintf(stderr, "\r%d  ", bkg);
		fflush(stderr);
		if (done)
			break;
	}
	// reset config
	tcsetattr(fd, TCSANOW, &oio);
	close(fd);
	return 0;
}
