# Build HCS08 targetted application with SDCC

BIN=bin

CC=sdcc
CFLAGS=-ms08 --stack-loc 0x86f
LDFLAGS=--out-fmt-s19 

all: $(BIN) $(BIN)/powermonitor.s19

clean:
	rm -rf $(BIN)

$(BIN):
	mkdir -p $(BIN)

$(BIN)/powermonitor.s19: $(BIN)/main.rel
	$(CC) -o $@ $(CFLAGS) $(LDFLAGS) $<
 
$(BIN)/main.rel: main.c
	$(CC) -c -o $@ $(CFLAGS) $<
