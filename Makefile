CC=cc
CFLAGS= $(shell pkg-config libusb-1.0 --cflags)
CXXFLAGS=
LD=cc
LDFLAGS= $(shell pkg-config libusb-1.0 --libs)

target = main

%.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@

all: DeviceMonitor.o
	$(LD) $(LDFLAGS) $< -o $(target)

.PHONY: clean

clean:
	rm -f *.o $(target)

