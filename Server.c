#include "server.h"

int main(int argc, char const *argv[]) // port
{
    char Message[MSG_LEN];
    memset(Message, 0, MSG_LEN);

    char Port_Str[PORT_LENGTH];
    int Port;

    int Server_Socket;

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
    if ((Server_Socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) >= 0) {
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
        //get message from any client
        if ((recvfrom(Server_Socket, Message, MSG_LEN, MSG_WAITALL,
                      (struct sockaddr *) &Client_Addr, &Client_Addr_Len))
            >= 0) {
            printf("Recieved message from client: %s\n", Message);
        }
        else {
            printf("Recieving error: %s\n", strerror(errno));
            exit(3);
        }

       //change message
        *Message = '7';

        //send message to client
        if (sendto(Server_Socket, Message, strlen(Message), MSG_CONFIRM,
                   (struct sockaddr *) &Client_Addr, Client_Addr_Len) > 0) {
            printf("Send to client: %s\n", Message);
        }
        else {
            printf("Error on sendig message: %s\n", strerror(errno));
            exit(4);
        }
    }
    return 0;
}
