# Build HCS08 targetted power monitor with SDCC & linux control apps
BIN=bin
JAR=jar
JAVAC=javac
LIBS=-lm

SDCC=sdcc
SDCCFLAGS=-ms08 --stack-loc 0x86f
SDCCLDFLAGS=--out-fmt-s19 

all: $(BIN) $(BIN)/powermonitor.s19 $(BIN)/serio $(BIN)/serdump $(BIN)/power_meter.jar $(BIN)/rms_view.jar

clean:
	rm -rf $(BIN)

$(BIN)/power_meter.jar: $(BIN)/com/ashbysoft/power_meter/PowerMeter.class
	$(JAR) cfe $@ com.ashbysoft.power_meter.PowerMeter -C $(BIN) com/ashbysoft/power_meter/PowerMeter.class

$(BIN)/rms_view.jar: $(BIN)/com/ashbysoft/power_meter/RmsView.class
	$(JAR) cfe $@ com.ashbysoft.power_meter.RmsView -C $(BIN) com/ashbysoft/power_meter/RmsView.class

$(BIN):
	mkdir -p $(BIN)

$(BIN)/serio: $(BIN)/serio.o
	$(CC) -o $@ $< $(LIBS)

$(BIN)/serdump: $(BIN)/serdump.o
	$(CC) -o $@ $<

$(BIN)/%.o: %.c
	$(CC) -c -o $@ $(CFLAGS) $<

$(BIN)/%.class: %.java
	$(JAVAC) -d $(BIN) $<

$(BIN)/powermonitor.s19: $(BIN)/main.rel
	$(SDCC) -o $@ $(SDCCFLAGS) $(SDCCLDFLAGS) $<
 
$(BIN)/%.rel: %.c
	$(SDCC) -c -o $@ $(SDCCFLAGS) $<
