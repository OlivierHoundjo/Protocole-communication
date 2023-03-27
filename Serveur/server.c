#include "contiki.h"
#include "net/ipv6/uip-ds6.h"
#include "net/ipv6/simple-udp.h"
#include <stdio.h>
#include <string.h>

// Définition de la taille maximale du paquet
#define MAX_PAYLOAD_LEN 127

// Définition des identificateurs de message pour les différents types de messages
#define DATA_MSG_ID 0x01
#define CMD_MSG_ID 0x02

// Définition des types de message
enum message_type {
    DATA,
    COMMAND
};

// Définition de la structure de données du message
struct message {
    uint16_t id;
    enum message_type type;
    uint16_t data_length;
    uint8_t payload[MAX_PAYLOAD_LEN];
};

// Définition des ports du client et du serveur
#define CLIENT_PORT 8765
#define SERVER_PORT 5678

// Déclaration des variables globales
static struct simple_udp_connection udp_connection;
static struct message rcv_msg;

// Fonction de traitement des messages reçus
static void handle_msg(const void *data, uint16_t datalen, const uip_ipaddr_t *source_addr, uint16_t source_port) {
    // Vérification de la taille des données
    if (datalen < sizeof(struct message)) {
        printf("Message trop court, taille minimale = %d octets\n", sizeof(struct message));
        return;
    }

    // Copie des données reçues dans la structure de message
    memcpy(&rcv_msg, data, sizeof(struct message));

    // Vérification de l'identificateur de message
    if (rcv_msg.id != CMD_MSG_ID) {
        printf("Message invalide, identificateur de message incorrect\n");
        return;
    }

    // Traitement du message de commande reçu
    printf("Commande reçue : %s\n", rcv_msg.payload);
}

// Fonction principale
PROCESS(server_process, "Serveur de contrôle");
AUTOSTART_PROCESSES(&server_process);
PROCESS_THREAD(server_process, ev, data) {
    PROCESS_BEGIN();

    // Configuration de la connexion UDP
    simple_udp_register(&udp_connection, SERVER_PORT, NULL, CLIENT_PORT, handle_msg);

    // Boucle d'exécution principale
    while (1) {
        // Attente d'événements
        PROCESS_WAIT_EVENT();

        // Traitement des événements
        if (ev == PROCESS_EVENT_TIMER) {
            // Envoi d'un message de données à tous les appareils
            struct message send_msg;
            send_msg.id = DATA_MSG_ID;
            send_msg.type = DATA;
            send_msg.data_length = 6;
            memcpy(send_msg.payload, "hello", 6);
            simple_udp_sendto(&udp_connection, &send_msg, sizeof(struct message), NULL);
        } else if (ev == PROCESS_EVENT_EXIT) {
            // Sortie de la boucle d'exécution
            break;
        } else {
            printf("Evénement inattendu, type : %d\n", ev);
        }
    }

    // Fin du processus
    PROCESS_END();
}
