# Build linux power meter control app
BIN=bin
JAR=jar
JAVAC=javac
LIBS=-lm

all: $(BIN) $(BIN)/serio $(BIN)/power_meter.jar $(BIN)/rms_view.jar

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

$(BIN)/%.o: %.c
	$(CC) -c -o $@ $(CFLAGS) $<

$(BIN)/%.class: %.java
	$(JAVAC) -d $(BIN) $<
