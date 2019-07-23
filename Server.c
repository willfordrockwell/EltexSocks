#include "server.h"

int main(int argc, char const *argv[]) // port
{
    char Message[MSG_LEN];
    memset(Message, 0, MSG_LEN);

    char Port_Str[PORT_LENGTH];
    int Port;

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
            printf("Recieving error: %s\n", strerror(errno));
            exit(3);
        }

        if (recv(Client_Socket, Message, MSG_LEN, NO_FLAGS) > 0) {
            printf("Recieved from client: %s\n", Message);
        }
        else {
            printf("Error on recieving message: %s\n", strerror(errno));
            exit(4);
        }
        //change message
        *Message = '7';
        //send message to client
        if (send(Client_Socket, Message, strlen(Message), NO_FLAGS) > 0) {
            printf("Send to client: %s\n", Message);
        }
        else {
            printf("Error on sendig message: %s\n", strerror(errno));
            exit(5);
        }
        //remove socket
        if (shutdown(Client_Socket, SHUT_RDWR) == 0) {
            printf("Successfully shutdown socket\n");
        }
        else {
            printf("Error on shutdown socket: %s\n", strerror(errno));
            exit(6);
        }
    }
    return 0;
}
