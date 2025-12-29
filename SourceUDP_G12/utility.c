#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "main.h"

//funzione helper per printare eventuali errori
void errore(const char* msg) {
    perror(msg);
}

//funzione per risolvere il nome inserito clientside
void risolviNome(int socket,char* buffer) {//copiaincollata da source TCP
    struct hostent* entry;
    struct in_addr **lista;

    buffer[strcspn(buffer, "\n")] = 0;//serve rimuovere il newline senno` gethostbyname impazzisce

    entry = gethostbyname(buffer);

    //exception handling
    if (entry == NULL) {
        herror("gethostbyname");//outputta l'errore preciso. utile
        printf("Nome non trovato! riavviare il programma e riprovare.\n");
        close(socket);
        return;
    }

    //estrazione dalla lista
    lista = (struct in_addr**) entry->h_addr_list;

    //piu` error checking e output finale
    if (lista[0] != NULL) {
        char *ip = inet_ntoa(*lista[0]);
        strncpy(buffer, ip, 15);
        buffer[15] = '\0'; //aggiunta del null terminator
    } else {
        printf("Impossibile trovare indirizzo IP!\n");
        close(socket);
    }
}

//funzione generica per inviare un messaggio
void inviaMessaggio(int socket, char* msg, struct sockaddr_in* addr, socklen_t addrLen) {
    //buffer del messaggio
    char sndMsg[MSGLEN] = {0};

    //formatting del messaggio
    snprintf(sndMsg, MSGLEN, "%s", msg);

    //invio messaggio
    ssize_t sndBytes = sendto(socket, sndMsg, strlen(sndMsg), 0, (struct sockaddr*)addr, addrLen);

    if (sndBytes < 0) {
        errore("Impossibile inviare messaggio!");
        close(socket);
    }
}

//funzione generica per ricevere un messaggio
void riceviMessaggio(int socket, char* buffer, struct sockaddr_in* addr, socklen_t* addrLen) {
    //ricezione dei dati
    ssize_t rcvBytes = recvfrom(socket, buffer, MSGLEN - 1, 0, (struct sockaddr*)addr, addrLen);

    if (rcvBytes < 0) {
        errore("Payload non ricevuto!");
        close(socket);
    }

    //rimozione di newline e carriage return e aggiunta del null terminator
    buffer[strcspn(buffer, "\r\n")] = 0;
    buffer[rcvBytes] = '\0';
}
