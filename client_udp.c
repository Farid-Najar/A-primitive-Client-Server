#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<netdb.h>
#include<string.h>

#define PORT 9600

int main(int argc, char *argv[]){
            /*
    * Variables du client
    *
    * Déclarer ici les variables suivantes :
    *
    - sockfd le descripteur de socket
    *
    - structure d’adresse du serveur
    *
    - pointeur vers la structure descriptive de machine (hostent)
    *
    - zone de mémoire destinée à accueillir la chaîne
    *
    entrée au clavier
    *
    - taille de la chaîne à envoyer
    */
    //initialisation du message initial
    char message[1024];
    printf("Bonjour, je suis là!!!\n");

    //création de socket
    int socketfd = socket(PF_INET, SOCK_DGRAM, 0);
    if(socketfd == -1){
        perror("problème socket");
        exit(EXIT_FAILURE);
    }

    //initialisation hostent
    struct hostent *hostinfo = NULL;
    const char *hostname = "localhost";
    hostinfo = gethostbyname(hostname);
    if(hostinfo == NULL){
        perror("Problème hostent\n");
        exit(0);
    }

    // spécifier les adresses pour le socket
    struct sockaddr_in addresse_serveur;
    memset(&addresse_serveur, 0, sizeof(addresse_serveur));
    addresse_serveur.sin_family = PF_INET;
    addresse_serveur.sin_port = htons(PORT);
    memcpy(&addresse_serveur.sin_addr.s_addr, hostinfo->h_addr, sizeof(hostinfo->h_addr));

    // Prendre le message au clavier
    printf("Quel est votre message ? ");
    fgets(message, 1024, stdin);
    printf("%s\n", message);

    bind(socketfd, (struct sockaddr*) &addresse_serveur, sizeof(addresse_serveur));

    //envoi d'un message au serveur    
    int sendOk = sendto(socketfd, (char *)message, strlen(message), 0, (struct sockaddr *)&addresse_serveur, sizeof(addresse_serveur));
    if(sendOk < 0){
        perror("problème sendto()");
        exit(h_errno);
    }

    printf("Message envoyé\n");
    // fermeture de socket
    close(socketfd);
    return 0;
}