#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<netdb.h>
#include <string.h>
#include<stdlib.h>

/* Port local du serveur */
#define PORT 9600

int main(int argc, char *argv[]){
    /*
* Variables du serveur
*
* Déclarer ici les variables suivantes :
*
- sockfd le descripteur de socket
*
- structure d’adresse locale du serveur
*
- structure d’adresse du client
*
- taille de l’adresse du client
*/
    // message envoyé en cas de connection
    char *message_serveur = "Connection réussi au serveur!\n";
    //pour stocker les messages envoyés par les clients
    char buf[1024];
    bzero(buf, 1024);

    // création de socket
    int socketfd;
    socketfd = socket(PF_INET, SOCK_DGRAM, 0);
    if(socketfd == -1){
        perror("problème socket\n");
        exit(1);
    }

    //spécification de l'adresse serveur
    struct sockaddr_in addresse_serveur;
    memset(&addresse_serveur, 0, sizeof(addresse_serveur)); 
    addresse_serveur.sin_family = PF_INET;
    addresse_serveur.sin_port = htons(PORT);
    addresse_serveur.sin_addr.s_addr = INADDR_ANY;
    printf("Déclarations réussies\n");
  

    // bind (attacher/lier) socket 
    bind(socketfd, (const struct sockaddr*) &addresse_serveur, sizeof(addresse_serveur));
    printf("bind réussi\n");


    struct sockaddr_in client;
    memset(&client, 0, sizeof(client)); 
    int client_size  = sizeof(struct sockaddr_in);
    printf("Serveur démarre #############################\n");

    while(1){
        int n = recvfrom(socketfd, buf, 128, 0, (struct sockaddr *)&client, &client_size);
        printf("Message reçu!!\n");
        buf[strlen(buf)-1] = '\0';
        printf("Nous avons reçu : %s\n", buf);    
        bzero(buf, 1024);
        //envoyer le message au client
        sendto(socketfd, message_serveur, sizeof(message_serveur), 0, (struct sockaddr *)&client, sizeof(client));
    }

    //fermeture de socket
    close(socketfd);
    return 0;

}