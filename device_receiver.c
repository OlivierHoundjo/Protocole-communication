#include "contiki.h"
#include "contiki-net.h"
#include <stdio.h>
#include <string.h>

#define MAX_PAYLOAD_LEN 20
#define SERVER_PORT     5000 // port utilisé pour la communication avec la passerelle

PROCESS(udp_server_process, "UDP server process");
AUTOSTART_PROCESSES(&udp_server_process);

static struct uip_udp_conn *udp_conn;

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
    }
  }

  PROCESS_END();
}
