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

PROCESS(udp_receiver_process, "UDP receiver process");

AUTOSTART_PROCESSES(&udp_receiver_process);

PROCESS_THREAD(udp_receiver_process, ev, data)
{
  PROCESS_BEGIN();

  // Ouvre la connexion UDP
  udp_conn = udp_new(NULL, UIP_HTONS(UDP_SERVER_PORT), NULL);
  udp_bind(udp_conn, UIP_HTONS(UDP_CLIENT_PORT));

  while(1) {
    static struct datagram datagram;
    static uip_ipaddr_t sender_addr;
    static uint16_t sender_port;

    // Attend la réception d'un message
    PROCESS_WAIT_EVENT_UNTIL(ev == tcpip_event);

    if(uip_newdata()) {
      // Copie le datagramme reçu dans la structure de datagramme
      memcpy(&datagram, uip_appdata, sizeof(struct datagram));

      // Vérifie si le datagramme est destiné à cet appareil et s'il s'agit d'une commande
      if(datagram.type == 0x01) { // 0x01 représente une commande
        // Traite la commande ici
      }
    }
  }

  PROCESS_END();
}
