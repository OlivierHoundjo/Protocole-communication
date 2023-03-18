#include "contiki.h"
#include "contiki-net.h"
#include <stdio.h>
#include <string.h>

#define SERVER_IP_ADDR "fe80::212:7401:1:101" // adresse IPv6 de la passerelle RPL
#define SERVER_PORT     5000                 // port utilisé pour la communication avec la passerelle

PROCESS(udp_client_process, "UDP client process");
AUTOSTART_PROCESSES(&udp_client_process);

static struct uip_udp_conn *udp_conn;

PROCESS_THREAD(udp_client_process, ev, data)
{
  static struct etimer timer;
  static char message[50];

  PROCESS_BEGIN();

  // Configuration de la connexion UDP
  udp_conn = udp_new(NULL, UIP_HTONS(0), NULL);
  udp_bind(udp_conn, UIP_HTONS(3000)); // Port local pour la communication UDP

  while(1) {
    // Envoi de la donnée "Hello World!" à la passerelle RPL
    sprintf(message, "Hello World!");
    uip_udp_packet_sendto(udp_conn, message, strlen(message), 
                          &SERVER_IP_ADDR, UIP_HTONS(SERVER_PORT));

    // Attente pendant 5 secondes avant l'envoi de la prochaine donnée
    etimer_set(&timer, CLOCK_SECOND * 5);
    PROCESS_WAIT_UNTIL(etimer_expired(&timer));
  }

  PROCESS_END();
}
