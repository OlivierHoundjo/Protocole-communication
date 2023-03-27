CONTIKI_PROJECT = server
all: $(CONTIKI_PROJECT)

# Configuration de la plateforme et des modules requis
CONTIKI = /home/user/contiki

include $(CONTIKI)/Makefile.include

# Liste des fichiers sources
PROJECT_SOURCEFILES += server.c

# Configuration du compilateur
CFLAGS += -std=c99
