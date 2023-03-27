#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "contiki.h"
#include "net/ip/uip.h"
#include "net/ipv6/uip-ds6.h"
#include "net/udp/simple-udp.h"

#define SERVER_IP "::1" // Adresse IP du serveur
#define CLIENT_PORT 8765 // Port du client
#define SERVER_PORT 5678 // Port du serveur
#define MAX_PAYLOAD_LEN 125 // Taille maximale des données

static struct simple_udp_connection udp_connection;

PROCESS(example_process, "Example process");
AUTOSTART_PROCESSES(&example_process);

PROCESS_THREAD(example_process, ev, data)
{
    static struct etimer periodic_timer;
    static uint16_t message_id = 0;

    PROCESS_BEGIN();

    // Initialise la connexion UDP
    simple_udp_register(&udp_connection, CLIENT_PORT, NULL, SERVER_PORT, NULL);

    while(1) {
        char message[MAX_PAYLOAD_LEN];
        uint8_t type = 0x01; // Type de message pour les données envoyées
        uint16_t data_len = 0; // Taille des données
        uint16_t message_len = 0; // Taille totale du message
        uint16_t remaining_len = MAX_PAYLOAD_LEN - 5; // Taille restante pour les données
        uint16_t sent_len = 0; // Taille des données envoyées
        uint16_t received_len = 0; // Taille des données reçues
        uint8_t *ptr;

        // Envoie des données
        sprintf(message, "Exemple de données envoyées %d", message_id++);
        data_len = strlen(message) + 1; // +1 pour le caractère de fin de chaîne
        message_len = 5 + data_len; // 5 octets pour l'ID, le type et la taille
        if (message_len > MAX_PAYLOAD_LEN) {
            printf("Erreur: taille du message trop grande\n");
            continue;
        }
        ptr = (uint8_t *)message;
        ptr[-2] = message_id >> 8; // ID du message (octet de poids fort)
        ptr[-1] = message_id & 0xFF; // ID du message (octet de poids faible)
        ptr[0] = type; // Type de message
        ptr[1] = data_len >> 8; // Taille des données (octet de poids fort)
        ptr[2] = data_len & 0xFF; // Taille des données (octet de poids faible)
        sent_len = simple_udp_sendto(&udp_connection, message, message_len, &SERVER_IP);

        // Attente de la réponse
        etimer_set(&periodic_timer, CLOCK_SECOND);
        PROCESS_WAIT_EVENT_UNTIL(ev == PROCESS_EVENT_TIMER);
        if (uip_newdata()) {
            received_len = uip_datalen();
            if (received_len > MAX_PAYLOAD_LEN) {
                printf("Erreur: taille des données reçues trop grande\n");
                continue;
            }
            ptr = (uint8_t *)uip_appdata;
            if (ptr[0] == 0x02) { // Type de message pour les commandes reçues
                printf("Commande reçue: %s\n", ptr + 1); // Affichage de la commande
            }
        }
    }

    PROCESS_END();
}
