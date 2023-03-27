CONTIKI_PROJECT = device_receiver
all: $(CONTIKI_PROJECT)

TARGET_LIBFILES += -lm

# Inclusion des bibliothèques
CFLAGS += -I$(CONTIKI)/os/lib \
          -I$(CONTIKI)/platform/$(TARGET)/dev \
          -I$(CONTIKI)/platform/$(TARGET) \
          -I$(CONTIKI)/examples/common

# Options de compilation
CFLAGS += -Wall -Wno-format -Os

# Liens avec les bibliothèques
LIBS += -L$(CONTIKI)/os/lib -lcontiki \
        -L$(CONTIKI)/platform/$(TARGET) -l$(TARGET) \
        -lm
#UIP_CONF_IPV6 = 1
# Liste des fichiers sources
PROJECT_SOURCEFILES += device_receiver.c

CONTIKI = /home/user/contiki
include $(CONTIKI)/Makefile.include
