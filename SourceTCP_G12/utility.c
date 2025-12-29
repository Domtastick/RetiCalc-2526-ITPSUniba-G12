#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include "main.h"

//funzione helper per printare eventuali errori
void errore(const char* msg) {
    perror(msg);
}
//funzione per risolvere il nome inserito clientside
void risolviNome(int socket,char* buffer) {
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
void inviaMessaggio(int socket, char* msg) {
    //buffer del messaggio
    char sndMsg[MSGLEN] = {0};

    //formatting del messaggio
    snprintf(sndMsg, MSGLEN, "%s", msg);

    //invio messaggio
    ssize_t sndBytes = send(socket, sndMsg, strlen(sndMsg), 0);
    if (sndBytes < 0) {
        errore("Impossibile inviare messaggio!");
        close(socket);
    } else if (sndBytes == 0) {
        printf("Connessione crollata!\n");
        clientiInRisoluzione = 0;
        close(socket);
    }
}

//funzione generica per ricevere un messaggio
void riceviMessaggio(int socket, char* buffer) {
    //ricezione dei dati
    ssize_t rcvBytes = recv(socket, buffer, MSGLEN - 1, 0);
    if (rcvBytes < 0) {
        errore("Impossibile ricevere messaggi!");
        clientiInRisoluzione = 0;
        close(socket);
    }else if (rcvBytes == 0) {
        printf("Connessione crollata!\n");
        clientiInRisoluzione = 0;
        close(socket);
    }
    //rimozione di newline e carriage return e aggiunta del null terminator
    buffer[strcspn(buffer, "\r\n")] = 0;
    buffer[rcvBytes] = '\0';
}


//funzione che gestisce il client (quella che verra` eseguita nel nuovo thread creato)
void* serverClientHandler(void* pClientSocket) {
    //un client e` in risoluzione quindi aumento il valore della variabile globale
    clientiInRisoluzione = 1;

    //prendo il valore della client socket presente nel main loop
    int cliSock = *((int*)pClientSocket);

    //libero la memoria allocata per quel puntatore dato che non serve piu` nel loop
    free(pClientSocket);

    printf("Thread creato per il client %d.\n",cliSock-3);

    //logica di comunicazione
    //invio messaggio di avvenuta connessione
    inviaMessaggio(cliSock,"Connessione avvenuta.");
    printf("Inviato: Connessione avvenuta.\n");

    //ricezione del payload e logica per gestirlo
    //buffer per i payload
    char bufferMessaggioRicevuto[MSGLEN] = {0};
    char bufferMessaggioDaInviare[MSGLEN] = {0};

    riceviMessaggio(cliSock,bufferMessaggioRicevuto);
    printf("Query ricevuta dal client: %s\n",bufferMessaggioRicevuto);

    //gestione del caso e calcolo del risultato
    //contenitori dei valori
    char operatore = '0';
    double valore1, valore2, risultato;

    //estrazione dei valori
    sscanf(bufferMessaggioRicevuto, "%lf %c %lf", &valore1, &operatore, &valore2);

    //calcolo
    switch (operatore) {
        case '+':
            risultato = valore1 + valore2;
            break;
        case '-':
            risultato = valore1 - valore2;
            break;
        case '*':
            risultato = valore1 * valore2;
            break;
        case '/':
            risultato = valore1 / valore2;
            break;
        default:
            errore("Operatore matematico ricevuto dal client non valido!");
            inviaMessaggio(cliSock,"L'operatore matematico che hai inserito non e` valido! Riavvia il programma e riprova.");
            clientiInRisoluzione = 0;
            close(cliSock);
            break;
    }

    snprintf(bufferMessaggioDaInviare, MSGLEN, "Il risultato di %.3lf %c %.3lf e` %.3lf!", valore1, operatore, valore2,risultato);
    inviaMessaggio(cliSock, bufferMessaggioDaInviare);
    printf("Messaggio inviato: %s\nQuery del client %d risolta.\n",bufferMessaggioDaInviare,cliSock);

    return NULL;
}
