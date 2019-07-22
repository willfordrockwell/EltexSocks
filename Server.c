#include "server.h"

int main(int argc, char const *argv[]) // port
{
    char Message[MSG_LEN];
    memset(Message, 0, MSG_LEN);

    char Port_Str[PORT_LENGTH];
    int Port;

    pthread_t Tid;

    int idQueue;

    int Server_Socket;
    int Client_Socket;

    struct sockaddr_in Server_Addr, Client_Addr;
    socklen_t Client_Addr_Len = sizeof(Client_Addr);
    bzero(&Server_Addr, sizeof(struct sockaddr_in));

    if (argc < 2) { // no Port as paramater
        Get_Port("12345", Port_Str);
    }
    else {
        strcpy(Port_Str, argv[1]);
    }
    Port = htons(atoi(Port_Str));

    Server_Addr.sin_family = AF_INET;
    Server_Addr.sin_addr.s_addr = INADDR_ANY;
    Server_Addr.sin_port = Port;

    //init queue
    if ((idQueue = msgget(ftok("Server.elf", 1), 0600 | IPC_CREAT)) != -1) {
        printf("Queue was created\n");
    }
    else {
        printf("Queue was not created\n");
    }

    struct Queue To_Thread;

    //init socket
    if ((Server_Socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) >= 0) {
        printf("Socket created: %d\n", Server_Socket);
    }
    else {
        printf("Error creating socket: %s\n", strerror(errno));
        exit(1);
    }
    //binding
    if (bind(Server_Socket, (struct sockaddr *) &Server_Addr,
             sizeof(Server_Addr)) >= 0) {
        printf("Succeesfully binded\n");
    }
    else {
        printf("Error on binding: %s\n", strerror(errno));
        exit(2);
    }

    while (1) {
        //start listen
        bzero(&Client_Addr, sizeof(struct sockaddr_in));
        listen(Server_Socket, MAX_CLIENTS);
        //get message from any client
        if ((Client_Socket = accept(Server_Socket,
                                    (struct sockaddr *) &Client_Addr,
                                    &Client_Addr_Len)) >= 0) {
            printf("Accpeted connection with client: %d\n", Client_Socket);
        }
        else {
            printf("Accepting error: %s\n", strerror(errno));
            exit(3);
        }

        if (pthread_create(&Tid, NULL, Server_Thread, &idQueue) == 0) {
            printf("Thread %d was created to work with %d\n", Tid,
                   Client_Socket);
        }
        else {
            printf("Error creare Thread: %s\n", strerror(errno));
            exit(4);
        }
        To_Thread.mtype = Tid;
        To_Thread.Client_Socket = Client_Socket;
        msgsnd(idQueue, &To_Thread, sizeof(To_Thread) - sizeof(long), NO_FLAGS);
    }
    return 0;
}
