#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "main.h"

//funzione helper per printare eventuali errori
void errore(const char* msg) {
    perror(msg);
}

//funzione per risolvere il nome inserito clientside
void risolviNome(int socket,char* buffer) {
    if (strcmp(buffer, "localhost\n") == 0) {
        memset(buffer, 0, IPLEN);
        snprintf(buffer, IPLEN, "%s", "127.0.0.1");
    }else {
        printf("Nome non trovato! riavviare il programma e riprovare.\n");
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
