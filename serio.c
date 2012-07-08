/*A touch of serial I/O to interface with my power metering board.. */

#include <stdio.h>
#include <termios.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <math.h>
#include <time.h>

#define POWERBAUD	B115200
#define POWERDEV	"/dev/ttyUSB0"

void log_rms(char *rms, char *line, int n) {
	static short buf[1000];
	static int bpos=0;
	short val;

	if (bpos>=1000) {
		long ms=0;
		FILE *fp;
		for(bpos=0; bpos<1000; bpos++)
			ms+=(long)buf[bpos]*(long)buf[bpos];
		ms=(long)sqrt(ms/1000);
		bpos=0;
		fp=fopen(rms, "a");
		if (fp) {
			fprintf(fp, "%d: %ld\n", time(NULL), ms);
			fclose(fp);
		}
	}
	if (sscanf(line, "%hx", &val)==1) {
		buf[bpos++]=val;
	}
}

int main(int argc, char **argv) {
	int arg, fd, n;
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
		// skip lines starting with '-'
		if (line[0]=='-')
			continue;
		// log rms values if requested
		if (rms)
			log_rms(rms, line, n);
		else
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

	// reset config
	tcsetattr(fd, TCSANOW, &oio);
	return 0;
}

