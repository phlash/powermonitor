# Build linux power meter control app
JAR=jar
JAVAC=javac
LIBS=-lm

all: power_meter.jar rms_view.jar

clean:
	rm -rf serio power_meter.jar rms_view.jar class *.o

power_meter.jar: class class/com/ashbysoft/power_meter/PowerMeter.class serio
	$(JAR) cfe $@ com.ashbysoft.power_meter.PowerMeter serio -C class .

rms_view.jar: class class/com/ashbysoft/power_meter/RmsView.class
	$(JAR) cfe $@ com.ashbysoft.power_meter.RmsView -C class .

class:
	mkdir -p class

serio: serio.o
	$(CC) -o $@ $< $(LIBS)

%.o: %.c
	$(CC) -c -o $@ $(CFLAGS) $<

class/%.class: %.java
	$(JAVAC) -d class $<
