#include "contiki.h"
#include "net/ip/uip.h"
#include <string.h>

#define UDP_CLIENT_PORT 8765
#define UDP_SERVER_PORT 5678

#define MAX_PAYLOAD_LEN 123 // 127 - 2 (id du message) - 1 (type du message) - 2 (taille des données)

// Structure de datagramme
struct datagram {
  uint16_t id;
  uint8_t type;
  uint16_t data_len;
  uint8_t data[MAX_PAYLOAD_LEN];
};

static struct uip_udp_conn *udp_conn;
static struct datagram datagram;

PROCESS(udp_sender_process, "UDP sender process");

AUTOSTART_PROCESSES(&udp_sender_process);

PROCESS_THREAD(udp_sender_process, ev, data)
{
  static struct etimer et;
  static uint16_t message_id = 0;

  PROCESS_BEGIN();

  // Initialise la structure de datagramme
  datagram.id = 0;
  datagram.type = 0x00; // 0x00 représente des données
  datagram.data_len = 0;

  // Ouvre la connexion UDP
  uip_ipaddr_t ipaddr;
  uip_ip6addr(&ipaddr, 0, 0, 0, 0, 0, 0, 0, 1); // adresse ::1
  udp_conn = udp_new(&ipaddr, UIP_HTONS(UDP_SERVER_PORT), NULL);

  udp_bind(udp_conn, UIP_HTONS(UDP_CLIENT_PORT));

  while(1) {
    // Modifie la structure de datagramme avec les données à envoyer

    // Envoie la structure de datagramme sur le réseau
    datagram.id = message_id++;
    uip_udp_packet_send(udp_conn, &datagram, sizeof(struct datagram));

    // Attends un certain temps avant d'envoyer le prochain message
    etimer_set(&et, CLOCK_SECOND);
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
  }

  PROCESS_END();
}
