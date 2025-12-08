#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "main.h"

int main() {
    //creazione della socket
    int cSocket = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);

    //buffer per la porta, ip del server, messaggi ricevuti e messaggi inviati
    char ip[IPLEN] = {0};
    char messaggioDaInviare[MSGLEN] = {0};
    char messaggioRicevuto[MSGLEN] = {0};
    unsigned short porta = 0;

    printf("Inserire il nome del server a cui inviare il payload! (localhost)\n");
    fgets(ip, 16, stdin);
    printf("Inserire la porta del server a cui inviare il payload! (10420)\n");
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
    //qui connect setta solo l'indirizzo di destinazione per scambi di messaggi futuri
    if (connect(cSocket, (struct sockaddr *) &servAddr, srvAddrLen) < 0) {
        perror("Impossibile raggiungere il server!");
        close(cSocket);
        return -1;
    }

    //al contrario di TCP in UDP il server non sa che il client esiste fino a quando non riceve un pacchetto
    //quindi per far funzionare la logica del messaggio di connessione ricevuta invio prima un messaggio vuoto "ping"
    //per svegliare il server
    inviaMessaggio(cSocket, "", NULL, 0);//i campi in queste funzioni sono sempre null e 0 perche` sono storati in connect

    //ricezione primo messaggio
    riceviMessaggio(cSocket, messaggioRicevuto, NULL, 0);
    printf("Ricevuto: %s\n", messaggioRicevuto);
    memset(messaggioRicevuto, 0, MSGLEN);//clear del buffer

    //invio messaggi
    printf("\t\t\t\tCalcolatrice remota!\nInviare al server un valore seguito da un operatore (+,-,*,/) seguito da un altro valore!\n");
    printf("Operatore e valori devono essere separati da almeno uno spazio! (E.s. 15 / 2. Il numero di spazi in piu` e` irrilevante!!)\n");
    fgets(messaggioDaInviare, MSGLEN, stdin);
    inviaMessaggio(cSocket, messaggioDaInviare, NULL, 0);

    //ricevi risultato dal server e printalo
    riceviMessaggio(cSocket, messaggioRicevuto, NULL, 0);
    printf("Ricevuto: %s\n",messaggioRicevuto);

    //chiusura del programma
    printf("Chiusura programma!\n");
    close(cSocket);
    return 0;
}