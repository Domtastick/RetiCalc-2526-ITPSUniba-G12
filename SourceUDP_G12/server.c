#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include "main.h"

int main() {

    int porta = SERVERPORT;
    //socket del client
    struct sockaddr_in servAdd = {0};
    struct sockaddr_in cliAdd = {0};
    socklen_t cliSockLen = sizeof(cliAdd);

    //creazione della welcome socket
    int sSocket = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
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

    printf("Server inizializzato con successo.\n");

    while (1) {
        //questa volta non ho messo multithreading perche` non lo trovavo necessario, indi per cui il codice
        //e` tutto nel loop
        char bufferMessaggioRicevuto[MSGLEN] = {0};
        char bufferMessaggioDaInviare[MSGLEN] = {0};

        //ricezione del ping per catturare indirizzo e porta del client
        riceviMessaggio(sSocket, bufferMessaggioRicevuto, &cliAdd, &cliSockLen);

        printf("Ping ricevuto dal client %s\n", inet_ntoa(cliAdd.sin_addr));

        //logica di comunicazione
        //invio messaggio di avvenuta connessione usando l'indirizzo appena catturato
        inviaMessaggio(sSocket, "Parametri registrati.", &cliAdd, cliSockLen);
        printf("Inviato: Parametri registrati.\n");

        //gestione payload
        //prima resetto il buffer
        memset(bufferMessaggioRicevuto, 0, MSGLEN);

        //poi ricevo il payload
        riceviMessaggio(sSocket, bufferMessaggioRicevuto, &cliAdd, &cliSockLen);

        //gestione del caso e calcolo del risultato
        //contenitori dei valori
        char operatore = '0';
        double valore1, valore2, risultato;

        //estrazione dei valori e print a schermo della query
        sscanf(bufferMessaggioRicevuto, "%lf %c %lf",&valore1, &operatore, &valore2);
        printf("Query ricevuta dal client: %s\n",bufferMessaggioRicevuto);

        //devo aggiungere una variabile per tenere traccia della validita` di un'operazione dato che non c'e`
        //una connessione da chiudere in caso fallisca
        short operazioneValida = 1;
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
                inviaMessaggio(sSocket,"L'operatore matematico che hai inserito non e` valido! riavvia il programma e riprova.", &cliAdd, cliSockLen);
                operazioneValida = 0;
                break;
        }

        if(operazioneValida) {
            snprintf(bufferMessaggioDaInviare, MSGLEN, "Il risultato di %.3lf %c %.3lf e` %.3lf!", valore1, operatore, valore2,risultato);
            inviaMessaggio(sSocket, bufferMessaggioDaInviare, &cliAdd, cliSockLen);
            printf("Messaggio inviato: %s\nQuery del client %s risolta.\n",bufferMessaggioDaInviare,inet_ntoa(cliAdd.sin_addr));
        }

        printf("Torno in ascolto.\n");
    }
}