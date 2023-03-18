#include "contiki.h"
#include "contiki-net.h"
#include <stdio.h>
#include <string.h>

#define MAX_PAYLOAD_LEN 20
#define SERVER_PORT     5000 // port utilisé pour la communication avec la passerelle
#define CLIENT_PORT     5001 // port utilisé pour la communication avec le serveur

PROCESS(udp_client_process, "UDP client process");
PROCESS(udp_server_process, "UDP server process");
AUTOSTART_PROCESSES(&udp_client_process, &udp_server_process);

static struct uip_udp_conn *udp_conn;

PROCESS_THREAD(udp_client_process, ev, data)
{
  PROCESS_BEGIN();

  static struct etimer et;
  char buf[MAX_PAYLOAD_LEN];

  // Configuration de la connexion UDP
  udp_conn = udp_new(NULL, UIP_HTONS(0), NULL);
  udp_bind(udp_conn, UIP_HTONS(CLIENT_PORT));

  while(1) {
    // Envoi d'un message UDP au serveur toutes les 5 secondes
    sprintf(buf, "Hello from client");
    uip_udp_packet_send(udp_conn, buf, strlen(buf), &UIP_IP_BUF->srcipaddr, UIP_HTONS(SERVER_PORT));
    printf("Sent: '%s'\n", buf);
    etimer_set(&et, CLOCK_SECOND * 5);
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
  }

  PROCESS_END();
}

PROCESS_THREAD(udp_server_process, ev, data)
{
  PROCESS_BEGIN();

  // Configuration de la connexion UDP
  udp_conn = udp_new(NULL, UIP_HTONS(0), NULL);
  udp_bind(udp_conn, UIP_HTONS(SERVER_PORT));

  while(1) {
    static char buf[MAX_PAYLOAD_LEN+1];
    static int len;
    static struct uip_udp_conn *client_conn;
    PROCESS_WAIT_EVENT_UNTIL(ev == tcpip_event);
    if(uip_newdata()) {
      len = uip_datalen();
      if(len > MAX_PAYLOAD_LEN) {
        len = MAX_PAYLOAD_LEN;
      }
      memcpy(buf, uip_appdata, len);
      buf[len] = '\0';
      printf("Received: '%s'\n", buf);
      
      // Envoi d'une réponse au client
      client_conn = udp_new(&UIP_IP_BUF->srcipaddr, UIP_HTONS(CLIENT_PORT), NULL);
      sprintf(buf, "Hello from server");
      uip_udp_packet_send(client_conn, buf, strlen(buf));
      printf("Sent: '%s'\n", buf);
      udp_remove(client_conn);
    }
  }

  PROCESS_END();
}
