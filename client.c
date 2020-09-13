#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<netdb.h>
#include<string.h>

#define SIZE 100
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
    
    //création de socket
    int socket_serveur = socket(AF_INET, SOCK_STREAM, 0);
    if(socket_serveur == -1){
        perror("problème socket");
        exit(h_errno);
    }
    char buf[1024];
    
    // spécifier les adresses pour le socket
    struct sockaddr_in addresse_serveur;
    addresse_serveur.sin_family = AF_INET;
    addresse_serveur.sin_port = htons(PORT);
    addresse_serveur.sin_addr.s_addr = INADDR_ANY;

    //connection au serveur
    int connection_status = connect(socket_serveur, (struct sockaddr *) &addresse_serveur, sizeof(addresse_serveur));
    if(connection_status == -1){
        perror("problème connect");
        exit(h_errno);
    }
    printf("Connecté\n");
    //On va reçevoir un id
    char Cid[] = "0";
    write(socket_serveur, Cid, 1);
    printf("id envoyé\n");
    read(socket_serveur, &Cid, 1);
    if(strcmp(Cid, "0") == 0 ){
        perror("id non reçu");
        exit(EXIT_FAILURE);
    }
    printf("id recu\n");

    char name[32];
    //faut se présenter
    if(argc > 1){
        memcpy(name, argv[1], sizeof(argv[1]));
    }
    else{
        memcpy(name, "Anonyme", 8);
    }
    write(socket_serveur, name, sizeof(name));

    //recevoir les données du serveur
    char reponse_serveur[256];
    read(socket_serveur, &reponse_serveur, sizeof(reponse_serveur));

    // Affichage de la réponse
    printf("Le serveur a répondu : \"%s\"\n", reponse_serveur);

    // On écrit un message
    printf("Quel est votre message ? ");
    fgets(buf, 1024, stdin);
    write(socket_serveur, buf, 1024);
    printf("Nous avons envoyé le message : %s", buf);

    // fermeture de socket
    close(socket_serveur);
    char message[2048];
    while(1){
        memset(buf, 0, 1024);
        memset(message, 0, 2048);
        strcat(message, name);
        strcat(message, " : ");
        // On écrit un message
        printf("Quel est votre message ? ");
        fgets(buf, 1024, stdin);
        strcat(message, buf);

        int socket_serveur = socket(AF_INET, SOCK_STREAM, 0);
        int connection_status = connect(socket_serveur, (struct sockaddr *) &addresse_serveur, sizeof(addresse_serveur));
        if(connection_status == -1){
            perror("problème connect");
            exit(h_errno);
        }
        //faut se présenter
        write(socket_serveur, "1", 1);
        //write(socket_serveur, name, sizeof(name));
        write(socket_serveur, message, 2048);
        printf("Nous avons envoyé le message : %s", buf);

        // fermeture de socket
        close(socket_serveur);
    }
    return 0;
}