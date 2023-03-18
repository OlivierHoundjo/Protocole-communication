#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define PORT 12345 // Le port utilisé pour la communication

int main() {
    int server_socket;
    struct sockaddr_in server_address, client_address;
    char buffer[1024];

    // Création du socket du serveur
    server_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (server_socket < 0) {
        perror("Erreur lors de la création du socket");
        exit(1);
    }

    // Configuration de l'adresse du serveur
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);
    server_address.sin_port = htons(PORT);

    // Association du socket à l'adresse du serveur
    if (bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address)) < 0) {
        perror("Erreur lors de l'association du socket à l'adresse");
        exit(1);
    }

    printf("Le serveur est prêt à recevoir des commandes et des données.\n");

    while (1) {
        socklen_t client_address_length = sizeof(client_address);

        // Réception des données envoyées par les appareils IoT
        int data_length = recvfrom(server_socket, buffer, sizeof(buffer), 0, (struct sockaddr *)&client_address, &client_address_length);
        if (data_length < 0) {
            perror("Erreur lors de la réception des données");
            exit(1);
        }

        // Traitement des données reçues
        buffer[data_length] = '\0';
        printf("Données reçues : %s\n", buffer);

        // Envoi d'une commande à un appareil IoT
        char command[] = "allumer_led_1";
        if (sendto(server_socket, command, sizeof(command), 0, (struct sockaddr *)&client_address, client_address_length) < 0) {
            perror("Erreur lors de l'envoi de la commande");
            exit(1);
        }

        // Attente d'une commande de l'utilisateur
        printf("Entrez une commande : ");
        scanf("%s", buffer);

        // Envoi de la commande à un appareil IoT
        if (sendto(server_socket, buffer, strlen(buffer), 0, (struct sockaddr *)&client_address, client_address_length) < 0) {
            perror("Erreur lors de l'envoi de la commande");
            exit(1);
        }
    }

    return 0;
}
