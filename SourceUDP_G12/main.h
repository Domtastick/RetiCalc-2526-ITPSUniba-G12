#ifndef ESRETI1_MAIN_H
#define ESRETI1_MAIN_H

#include <netinet/in.h> //Necessario per struct sockaddr_in

//========================DEFINIZIONI==========================
#define SPAZIO ' '
#define SERVERPORT 10420
#define QLEN 1
#define MSGLEN 1024
#define IPLEN 16

//========================FUNZIONI=============================
void errore(const char* msg);
void riceviMessaggio(int socket, char* buffer, struct sockaddr_in* addr, socklen_t* addrLen);
void inviaMessaggio(int socket, char* msg, struct sockaddr_in* addr, socklen_t addrLen);
void risolviNome(int socket,char* buffer);

//======================VARIABILI GLOBALI======================
extern short clientiInRisoluzione; //creata solo perche` voglio implementare il multithreading

#endif //ESRETI1_MAIN_H