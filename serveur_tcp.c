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
    char message_serveur[32] = "Connection réussi au serveur!";
    //pour stocker les messages envoyés par les clients
    char buf[2048];
    char name[32];

    // création de socket
    int socket_serveur;
    socket_serveur = socket(AF_INET, SOCK_STREAM, 0);

    //spécification de l'adresse serveur
    struct sockaddr_in addresse_serveur;
    addresse_serveur.sin_family = AF_INET;
    addresse_serveur.sin_port = htons(PORT);
    addresse_serveur.sin_addr.s_addr = INADDR_ANY;

    // bind (attacher/lier) socket à IP et PORT spécifié
    bind(socket_serveur, (struct sockaddr*) &addresse_serveur, sizeof(addresse_serveur));

    printf("Bonjour je suis le serveur\n");

    // spécification du nombre de clients
    listen(socket_serveur, 7);
    char id_client[] = "0";
    printf("J'attends les invités !!! \n");

    // Que la disscution commence
    while(1){
        //création de socket client
        int socket_client;
        //On accepte le client (On lui donne la parole)
        socket_client = accept(socket_serveur, NULL, NULL);
        if(socket_client>=0){
            //on ecoute son id
            //if(read(socket_client, &id_client, 1)>=0){
            //On vérifie si le client existe déjà
            //if(strcmp(id_client, "0") == 0){
            //    write(socket_client, "1", 1);
            if(read(socket_client, &name, 32)>=0){
                //On écrit un message de bienvenu
                printf("Bienvenue %s !!!\n", name);
                //envoyer le message au client
                write(socket_client, message_serveur, sizeof(message_serveur));
                if(read(socket_client, &buf, 2048)>=0){
                    buf[strlen(buf)-1]= '\0';
                    printf("%s : %s\n", name, buf);
                }
            }
            //}else
            //{
            //    if(read(socket_client, &name, 32)>=0){
            //        //On écoute son message
            //        if(read(socket_client, &buf, 2048)>=0){
            //            printf("%s : %s", name, buf);
            //        }
            //    }
            //}
            
            //}
            //on déconnecte le client
            close(socket_client);
        }
        
    }

    //fermeture de socket
    close(socket_serveur);
    return 0;

}