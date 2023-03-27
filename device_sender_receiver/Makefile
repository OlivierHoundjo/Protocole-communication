CONTIKI_PROJECT = device_sender_receiver

all: $(CONTIKI_PROJECT)

# Définition des sources
PROJECT_SOURCEFILES += device_sender_receiver.c

# Configuration de l'environnement pour Cooja

UIP_CONF_IPV6 = 1 

CONTIKI = /home/user/contiki
# Inclusion des bibliothèques nécessaires
MODULES_REL += $(TARGET)/lib/net/ip/uip6.a
MODULES_REL += $(TARGET)/lib/net/ipv6/uip-ds6.a
MODULES_REL += $(TARGET)/lib/net/ipv6/multicast/uip-mcast6.a
MODULES_REL += $(TARGET)/lib/net/ipv6/uip-icmp6.a
MODULES_REL += $(TARGET)/lib/net/udp/simple-udp.a

# Compilation du projet
include $(CONTIKI)/Makefile.include
