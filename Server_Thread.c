#include "server.h"

void *Server_Thread (void *arg)
{
    struct Queue From_Parent;
    int idQueue = *((int*)arg);
    int Port;

    char Message[MSG_LEN];

    int Server_Socket;

    struct sockaddr_in Server_Addr, Client_Addr;
    socklen_t Client_Addr_Len = sizeof(Client_Addr);
    bzero(&Server_Addr, sizeof(struct sockaddr_in));

    if (msgrcv(idQueue, &From_Parent, sizeof(From_Parent) - sizeof(long), pthread_self(), NO_FLAGS ) > 0) {
        printf("Msg was recieved\n");
    }
    else {
        printf("Msg was not recieved: %s\n", strerror(errno));
    }

    Port = From_Parent.Port;

    Server_Addr.sin_family = AF_INET;
    Server_Addr.sin_addr.s_addr = INADDR_ANY;
    Server_Addr.sin_port = Port;

     //init socket
    if ((Server_Socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) >= 0) {
        printf("Thread Socket created: %d\n", Server_Socket);
    }
    else {
        printf("Thread Error creating socket: %s\n", strerror(errno));
        exit(6);
    }
    //binding
    if (bind(Server_Socket, (struct sockaddr *) &Server_Addr,
             sizeof(Server_Addr)) >= 0) {
        printf("Thread Succeesfully binded\n");
    }
    else {
        printf("Thread Error on binding: %s\n", strerror(errno));
        exit(7);
    }

    bzero(&Client_Addr, sizeof(struct sockaddr_in));
    //get message from any client
    if ((recvfrom(Server_Socket, Message, MSG_LEN, MSG_WAITALL,
                  (struct sockaddr *) &Client_Addr, &Client_Addr_Len))
        >= 0) {
        printf("Thread Recieved message from client: %s\n", Message);
    }
    else {
        printf("Thread Recieving error: %s\n", strerror(errno));
        exit(8);
    }

    //change message
    *Message = '7';
    //send message to client
    if (sendto(Server_Socket, Message, strlen(Message), MSG_CONFIRM,
               (struct sockaddr *) &Client_Addr, Client_Addr_Len) > 0) {
        printf("Thread Send to client: %s\n", Message);
    }
    else {
        printf("Thread Error on sendig message: %s\n", strerror(errno));
        exit(9);
    }
    close(Server_Socket);
}