#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "main.h"

int main() {
    //creazione della socket TCP
    int cSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

    //buffer per la porta, ip del server, messaggi ricevuti e messaggi inviati!
    char ip[IPLEN] = {0};
    char messaggioDaInviare[MSGLEN] = {0};
    char messaggioRicevuto[MSGLEN] = {0};
    unsigned short porta = 0;

    printf("Inserire l'ip del server a cui connettersi! (localhost)\n");
    fgets(ip, 16, stdin);
    printf("Inserire la porta del server a cui connettersi! (10114)\n");
    scanf("%hd", &porta);
    getchar();//per rimuovere trailing newline altrimenti fgets skippa
    risolviNome(cSocket,ip);

    //struttura socket per l'address e porta del server
    struct sockaddr_in servAddr;
    servAddr.sin_family = PF_INET;
    servAddr.sin_addr.s_addr = inet_addr(ip);
    servAddr.sin_port = htons(porta);
    socklen_t srvAddrLen = sizeof(servAddr);

    //connessione al server
    if (connect(cSocket, (struct sockaddr *) &servAddr, srvAddrLen) < 0) {
        perror("Impossibile connettersi al server!");
        close(cSocket);
        return -1;
    }

    //ricezione primo messaggio
    riceviMessaggio(cSocket, messaggioRicevuto);
    printf("Ricevuto: %s\n", messaggioRicevuto);
    memset(messaggioRicevuto, 0, MSGLEN);//clear del buffer

    //invio messaggi
    printf("\t\t\t\tCalcolatrice remota!\nInviare al server un valore seguito da un operatore (+,-,*,/) seguito da un altro valore!\n");
    printf("Operatore e valori devono essere separati da almeno uno spazio! (E.s. 15 / 2. Il numero di spazi in piu` e` irrilevante!!)\n");
    fgets(messaggioDaInviare, MSGLEN, stdin);
    inviaMessaggio(cSocket, messaggioDaInviare);

    //ricevi risultato dal server e printalo
    riceviMessaggio(cSocket, messaggioRicevuto);
    printf("Ricevuto: %s\n",messaggioRicevuto);

    //chiusura del programma
    printf("Chiusura programma!\n");
    close(cSocket);
    return 0;
}