/*A touch of serial I/O to interface with my power metering board.. */

#include <stdio.h>
#include <string.h>
#include <termios.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <math.h>
#include <time.h>

#define POWERBAUD	B115200
#define POWERDEV	"/dev/ttyUSB0"

//#define RMS2AMP(r)	(((double)r)/5.89)
#define RMS2AMP(r)	(((double)r)/4.73-0.67)

//static byte lookup[]={0xb7,0x11,0xad,0x3d,0x1b,0x3e,0xbe,0x15,0xbf,0x1f,0x9f,0x0ba,0xa6,0xb9,0xae,0x8e};

static char lline[80];

double log_rms(char *rfile, char *line, int n) {
	static short buf[1000];
	static int bpos=0;
	double rms=0.0;
	short val;

	if (bpos>=1000) {
		double ms=0;
		FILE *fp;
		for(bpos=3; bpos<996; bpos++) {
			// Keiser-bessel 7th order lpf, Fb=50Hz, Fs=1000Hz, M=7, Att=50dB. Generated from:
			// http://www.arc.id.au/FilterDesign.html
			double fil = 3 * (
				(double)buf[bpos-3]*0.004768+
				(double)buf[bpos-2]*0.034642+
				(double)buf[bpos-1]*0.078350+
				(double)buf[bpos]*0.100000+
				(double)buf[bpos+1]*0.078350+
				(double)buf[bpos+2]*0.034642+
				(double)buf[bpos+3]*0.004768
			);
			ms+=fil*fil;
		}
		rms=sqrt(ms/1000);
		bpos=0;
		fp=fopen(rfile, "a");
		if (fp) {
			fprintf(fp, "%d: %.2lf %s\n", time(NULL), rms, lline);
			fclose(fp);
		}
	}
	if (sscanf(line, "%hx", &val)==1) {
		buf[bpos++]=val;
	}
	return rms;
}

char disp[11];
int dlen=0;
void write_amps(double rms) {
	double amp = RMS2AMP(rms);
	if (amp<0.0) amp=0.0;
	int flr = (int)floor(amp);
	int dec = (int)((amp-floor(amp))*100);
	disp[0] = (char)((flr/100)%10)+'0';
	disp[1] = (char)((flr/10)%10)+'0';
	disp[2] = (char)(flr%10)+'0';
	disp[3] = 0x40;
	disp[4] = 'r';
	disp[5] = (char)((dec/10)%10)+'0';
	disp[6] = (char)(dec%10)+'0';
	disp[7] = 0x0;
	disp[8] = 'r';
	disp[9] = 'A';
	disp[10]= '\n';
	dlen=11;
}

int main(int argc, char **argv) {
	int arg, fd, n, d=0;
	struct termios oio, tio = {0};
	char line[40];
	char *dev = POWERDEV;
	char *rms = NULL;

	for(arg=1; arg<argc; arg++) {
		if (argv[arg][1]=='d')
			dev = argv[++arg];
		else if (argv[arg][1]=='r') {
			rms = argv[++arg];
		} else {
			puts("usage: serio [-d <serial device>] [-r <rms value log>]");
			return(0);
		}
	}

	// Open serial device, avoid it becoming controlling terminal
	fd = open(dev, O_RDWR | O_NOCTTY);
	if (fd<0) {
		perror("opening serial device");
		return(1);
	}

	// save old config
	tcgetattr(fd, &oio);

	// set new config
	tio.c_cflag = POWERBAUD | CS8 | CLOCAL | CREAD;
	tio.c_iflag = IGNPAR | ICRNL;
	tio.c_oflag = 0;
	tio.c_lflag = ICANON;
	tio.c_cc[VEOF] = 4;	// Ctrl-d
	tio.c_cc[VMIN] = 1;	// Wait for at least one char per read

	// flush anything collected, and set new config
	tcflush(fd, TCIFLUSH);
	tcsetattr(fd, TCSANOW, &tio);

	// read lines from power meter, print 'em
	while ((n=read(fd, line, sizeof(line)))>0) {
		// retain last line starting with '-'
		if (line[0]=='-') {
			strncpy(lline, line, sizeof(lline)-1);
			lline[n-1<sizeof(lline)-1?n-1:sizeof(lline)-1]=0;
		// log rms values if requested
		} else if (rms) {
			double d = log_rms(rms, line, n);
			if (d!=0.0) {
				write_amps(d);
			}
		} else {
			write(1, line, n);
			// check for display messages, forward them
			if (ioctl(0, FIONREAD, &n)<0) {
				perror("checking stdin pipe");
				break;
			} else if (n>0) {
				if ((n=read(0, line, sizeof(line)))>0) {
					write(fd, line, n);
				} else if (n==0) {
					// EOF from controlling process, quit
					break;
				}
			}
		}
		// clock out display chars (if any)
		if(dlen) {
			if (++d>=10) {
				--dlen;
				write(fd, &disp[dlen], 1);
				//printf("%02x,", disp[dlen]);
				fflush(stdout);
				d=0;
			}
		}
	}

	// reset config
	tcsetattr(fd, TCSANOW, &oio);
	return 0;
}

