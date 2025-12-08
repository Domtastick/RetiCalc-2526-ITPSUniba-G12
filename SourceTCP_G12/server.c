#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "main.h"

int main() {

    int porta = SERVERPORT;
    //socket del client
    struct sockaddr_in servAdd = {0};
    struct sockaddr_in cliAdd = {0};
    socklen_t cliSockLen = sizeof(cliAdd);

    //creazione della welcome socket
    int sSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sSocket < 0 ) errore("Creazione della socket del server fallita!");

    //impostazione della socket per venire riutilizzata
    int opt = 1;
    if (setsockopt(sSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(int)) < 0 ) {
        errore("Fallita l'impostazione di SO_REUSEADDR!");
    }

    //creazione della struttura
    servAdd.sin_family = PF_INET;
    servAdd.sin_addr.s_addr = inet_addr("127.0.0.1");
    servAdd.sin_port = htons(porta);
    //bind
    if (bind(sSocket, (struct sockaddr *) &servAdd, sizeof(servAdd)) < 0 ) {
        errore("Bind della welcome socket fallita!");
        close(sSocket);
        return -1;
    }

    //listen
    if (listen(sSocket, QLEN) < 0) {
        errore("Listen della socket server fallita!");
        close(sSocket);
        return -1;
    }

    printf("Server inizializzato con successo.\n");

    while (1) {
        //creazione della socket speciale per il client
        //accetto la connessione sulla socket per il client
        if (clientiInRisoluzione == 0) {//tutto viene eseguito solo se il server non e` occupato
            int cliSock = accept(sSocket, (struct sockaddr *) &cliAdd, &cliSockLen);
            if (cliSock < 0) errore("Impossibile accettare connessione!");

            //malloc per impedire race condition
            int *pClient = malloc(sizeof(int));
            *pClient = cliSock;

            //creazione thread
            pthread_t cliThread;
            if (pthread_create(&cliThread, NULL, serverClientHandler, (void*)pClient) != 0) {
                errore("Impossibile creare thread!");
                free(pClient);
                close(sSocket);
            }

            pthread_detach(cliThread);
            printf("Rimango in ascolto.\n");
        }
    }
}