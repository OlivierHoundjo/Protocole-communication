CONTIKI_PROJECT = device_sender

# Définir la plateforme de simulation
TARGET = cooja

# Inclure la librairie Contiki
CONTIKI = contiki-ng

# Inclure les modules nécessaires
MODULES_REL += $(CONTIKI_NG_SERVICES_DIR)/shell
UIP_CONF_IPV6 = 1 

# Inclure les fichiers sources
PROJECT_SOURCEFILES += device_sender.c

# Ajouter les règles de compilation et de liage
include $(CONTIKI)/Makefile.include

CONTIKI = /home/user/contiki
