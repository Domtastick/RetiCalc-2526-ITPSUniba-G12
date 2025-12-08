#ifndef ESRETI1_MAIN_H
#define ESRETI1_MAIN_H

//========================DEFINIZIONI==========================
#define SPAZIO ' '
#define SERVERPORT 10114
#define QLEN 1
#define MSGLEN 1024
#define IPLEN 16

//========================FUNZIONI=============================
void errore(const char* msg);
void* serverClientHandler(void* pClientSocket);
void riceviMessaggio(int socket, char* buffer);
void inviaMessaggio(int socket, char* msg);
void risolviNome(int socket,char* buffer);

//======================VARIABILI GLOBALI======================
extern short clientiInRisoluzione; //creata solo perche` voglio implementare il multithreading

#endif //ESRETI1_MAIN_H