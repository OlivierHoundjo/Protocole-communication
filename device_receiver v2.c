#include "contiki-net.h"

#define MAX_DATA_SIZE 125

struct message {
  uint16_t id;
  uint8_t type;
  uint16_t size;
  uint8_t data[MAX_DATA_SIZE];
};

PROCESS(my_process, "My Process");
AUTOSTART_PROCESSES(&my_process);

static struct uip_udp_conn *conn;

PROCESS_THREAD(my_process, ev, data)
{
  static struct message msg;
  static uint16_t id = 0;
  static uint16_t size = 0;
  static uint8_t data[MAX_DATA_SIZE];
  
  PROCESS_BEGIN();

  // Initialise la connexion UDP
  conn = udp_new(NULL, UIP_HTONS(0), NULL);
  udp_bind(conn, UIP_HTONS(3000));

  while (1) {
    // Envoie les données
    msg.id = UIP_HTONS(id++);
    msg.type = 0x01;
    msg.size = UIP_HTONS(size);
    memcpy(msg.data, data, size);

    if (size > MAX_DATA_SIZE) {
      size = MAX_DATA_SIZE;
    }

    uip_udp_packet_send(conn, &msg, sizeof(msg));
    
    PROCESS_PAUSE();
  }

  PROCESS_END();
}
