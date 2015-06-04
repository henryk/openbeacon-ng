TARGET=pca10031-openmili
ARCH=nrf5
CPU=$(ARCH)1422
VARIANT=xxAC
DEBUG=-g
OPTIM=-Os -mword-relocations

APP_COMMON_CFLAGS=-Iinc -D__USE_CMSIS
APP_CFLAGS=-std=gnu99 -fgnu89-inline
APP_LDFLAGS=-lm

APP_SRC= \
  src/timer.c \
  src/radio.c \
  src/main.c \
  openmili/MiLightRadio.cpp \
  openmili/PL1167_nRF51.cpp

APP_SRC+=$(IMAGES_C)

all: $(TARGET).bin

publish: clean $(TARGET).bin
	scp $(TARGET).bin openbeacon.org:/home/wwwrun/open.bitmanufaktur.com/web/www/people/milosch/nrf5/

app_clean:
	find src -name '*.o' -exec rm \{\} \;

clean_all: clean
	rm -f README $(CUSTOM_KEY)

include ../core/Makefile.rules
