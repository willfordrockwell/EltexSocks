#include "server.h"

void *Server_Thread (void *arg)
{
    char Message[MSG_LEN];
    int idQueue = *((int*)arg);

    int Client_Socket;

    struct Queue From_Parent;

    msgrcv(idQueue, &From_Parent, sizeof(From_Parent) - sizeof(long), 
           pthread_self(), NO_FLAGS);

    Client_Socket = From_Parent.Client_Socket;

    if (recv(Client_Socket, Message, MSG_LEN, NO_FLAGS) > 0) {
            printf("Recieved from client: %s\n", Message);
    }
    else {
        printf("Error on recieving message: %s\n", strerror(errno));
        exit(5);
    }
    //change message
    *Message = '7';
    //send message to client
    if (send(Client_Socket, Message, strlen(Message), NO_FLAGS) > 0) {
        printf("Send to client: %s\n", Message);
    }
    else {
        printf("Error on sendig message: %s\n", strerror(errno));
        exit(6);
    }
    //remove socket
    if (shutdown(Client_Socket, SHUT_RDWR) == 0) {
        printf("Successfully shutdown socket\n");
    }
    else {
        printf("Error on shutdown socket: %s\n", strerror(errno));
        exit(7);
    }
}