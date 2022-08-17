CC=cc
DESTDIR=/usr/local
CFLAGS=-Wall -O2
LFLAGS=-lm -s

OBJS=do.o

TARGET=do

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) -o $@ $^ $(LFLAGS)

%.o: %.c
	$(CC) -c $(CFLAGS) -o $@ $<

install:
	install -vDm6755 -- $(TARGET) $(DESTDIR)/sbin/

uninstall:
	cd -- $(DESTDIR)/sbin/ && rm -fv -- $(TARGET)

clean:
	rm -fv -- $(OBJS) $(TARGET)
