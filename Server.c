#include "server.h"

int main(int argc, char const *argv[]) // TCP port UDP port
{
    char Message[MSG_LEN];
    memset(Message, 0, MSG_LEN);

    char TCP_Port_Str[PORT_LENGTH];
    int TCP_Port;

    char UDP_Port_Str[PORT_LENGTH];
    int UDP_Port;

    int TCP_Server_Socket;
    int TCP_Client_Socket;

    int UDP_Server_Socket;

    int Max_Socket;

    struct timeval Time_Out;

    fd_set Sockets;

    struct sockaddr_in TCP_Server_Addr, UDP_Server_Addr, TCP_Client_Addr,
                       UDP_Client_Addr;
    socklen_t TCP_Client_Addr_Len = sizeof(TCP_Client_Addr);
    socklen_t UDP_Client_Addr_Len = sizeof(UDP_Client_Addr);
    bzero(&TCP_Server_Addr, sizeof(struct sockaddr_in));

    if (argc < 3) { // no Ports as paramater
        Get_Port("12345", TCP_Port_Str);
        Get_Port("12346", UDP_Port_Str);
    }
    else {
        strcpy(TCP_Port_Str, argv[1]);
        strcpy(UDP_Port_Str, argv[2]);
    }
    TCP_Port = htons(atoi(TCP_Port_Str));
    UDP_Port = htons(atoi(UDP_Port_Str));

    TCP_Server_Addr.sin_family = AF_INET;
    TCP_Server_Addr.sin_addr.s_addr = INADDR_ANY;
    TCP_Server_Addr.sin_port = TCP_Port;

    UDP_Server_Addr.sin_family = AF_INET;
    UDP_Server_Addr.sin_addr.s_addr = INADDR_ANY;
    UDP_Server_Addr.sin_port = UDP_Port;

    //init tcp socket
    if ((TCP_Server_Socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) >= 0) {
        printf("Socket created: %d\n", TCP_Server_Socket);
    }
    else {
        printf("Error creating socket: %s\n", strerror(errno));
        exit(1);
    }
    //binding tcp
    if (bind(TCP_Server_Socket, (struct sockaddr *) &TCP_Server_Addr,
             sizeof(TCP_Server_Addr)) >= 0) {
        printf("Succeesfully binded\n");
    }
    else {
        printf("Error on binding: %s\n", strerror(errno));
        exit(2);
    }

    //init udp socket
    if ((UDP_Server_Socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) >= 0) {
        printf("Socket created: %d\n", UDP_Server_Socket);
    }
    else {
        printf("Error creating socket: %s\n", strerror(errno));
        exit(3);
    }
    //binding tcp
    if (bind(UDP_Server_Socket, (struct sockaddr *) &UDP_Server_Addr,
             sizeof(UDP_Server_Addr)) >= 0) {
        printf("Succeesfully binded\n");
    }
    else {
        printf("Error on binding: %s\n", strerror(errno));
        exit(4);
    }

    Max_Socket =
        TCP_Server_Socket > UDP_Server_Socket ?
        TCP_Client_Socket + 1 : UDP_Server_Socket + 1;
    //start listen
    bzero(&TCP_Client_Addr, sizeof(struct sockaddr_in));
    listen(TCP_Server_Socket, MAX_CLIENTS);

    while (1) {
        FD_ZERO(&Sockets);
        FD_SET(TCP_Server_Socket, &Sockets);
        FD_SET(UDP_Server_Socket, &Sockets);

        Time_Out.tv_sec = 1;
        Time_Out.tv_usec = 0;

        select(Max_Socket, &Sockets, 0, 0, &Time_Out);
        //check tcp
        if(FD_ISSET(TCP_Server_Socket, &Sockets)) {
            //get message from any client via tcp
            if ((TCP_Client_Socket = accept(TCP_Server_Socket,
                                            (struct sockaddr *) &TCP_Client_Addr,
                                            &TCP_Client_Addr_Len)) >= 0) {
                printf("Accpeted connection with client: %d\n", TCP_Client_Socket);
            }
            else {
                printf("Accepting error: %s\n", strerror(errno));
                exit(5);
            }
            if (recv(TCP_Client_Socket, Message, MSG_LEN, NO_FLAGS) > 0) {
                printf("Recieved from client: %s\n", Message);
            }
            else {
                printf("Error on recieving message: %s\n", strerror(errno));
                exit(6);
            }
            //change message
            *Message = '7';
            //send message to client
            if (send(TCP_Client_Socket, Message, strlen(Message), NO_FLAGS)
                > 0) {
                printf("Send to client: %s\n", Message);
            }
            else {
                printf("Error on sendig message: %s\n", strerror(errno));
                exit(7);
            }
            //remove socket
            shutdown(TCP_Client_Socket, SHUT_RDWR);
        }
        //check udp
        if (FD_ISSET(UDP_Server_Socket, &Sockets)) {
            //start listen
            bzero(&UDP_Client_Addr, sizeof(struct sockaddr_in));
            //get message from any client
            if ((recvfrom(UDP_Server_Socket, Message, MSG_LEN, MSG_WAITALL,
                          (struct sockaddr *) &UDP_Client_Addr,
                          &UDP_Client_Addr_Len)) >= 0) {
                printf("Recieved message from client: %s\n", Message);
            }
            else {
                printf("Recieving error: %s\n", strerror(errno));
                exit(8);
            }

            //change message
            *Message = '7';
            //send message to client
            if (sendto(UDP_Server_Socket, Message, strlen(Message),
                       MSG_CONFIRM, (struct sockaddr *) &UDP_Client_Addr,
                       UDP_Client_Addr_Len) > 0) {
                printf("Thread Send to client: %s\n", Message);
            }
            else {
                printf("Thread Error on sendig message: %s\n",
                       strerror(errno));
                exit(9);
            }
        }
    }
    return 0;
}
