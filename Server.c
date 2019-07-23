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

    struct epoll_event Ev, Events[MAX_EVENTS];
    int Events_Num, Events_fd;

    struct sockaddr_in TCP_Server_Addr, UDP_Server_Addr, TCP_Client_Addr,
                       UDP_Client_Addr;
    socklen_t TCP_Client_Addr_Len = sizeof(TCP_Client_Addr);
    socklen_t UDP_Client_Addr_Len = sizeof(UDP_Client_Addr);
    bzero(&TCP_Server_Addr, sizeof(struct sockaddr_in));

    if (argc < 3) { // no Ports as paramater
        printf("TCP:\n");
        Get_Port("12345", TCP_Port_Str);
        printf("UDP:\n");
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

    Events_fd = epoll_create1(NO_FLAGS);

    Ev.events = EPOLLIN;
    Ev.data.fd = TCP_Server_Socket;

    epoll_ctl(Events_fd, EPOLL_CTL_ADD, TCP_Server_Socket, &Ev);

    Ev.data.fd = UDP_Server_Socket;

    epoll_ctl(Events_fd, EPOLL_CTL_ADD, UDP_Server_Socket, &Ev);
    
    //start listen
    bzero(&TCP_Client_Addr, sizeof(struct sockaddr_in));
    listen(TCP_Server_Socket, MAX_CLIENTS);

    while (1) {
        Events_Num = epoll_wait(Events_fd, Events, MAX_EVENTS, TIME_OUT);
        if (Events_Num == -1) {
            printf("epoll_wait error: %s\n", strerror(errno));
            exit(5);
        }
        for (int i = 0; i < Events_Num; i++) {
            //check tcp
            if (Events[i].data.fd == TCP_Server_Socket) {
                //get message from any client via tcp
                TCP_Client_Socket = accept(TCP_Server_Socket,
                                          (struct sockaddr *) &TCP_Client_Addr,
                                          &TCP_Client_Addr_Len);
                if (TCP_Client_Socket >= 0) {
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
            if (Events[i].data.fd == UDP_Server_Socket) {
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
                    printf("Send to client: %s\n", Message);
                }
                else {
                    printf("Error on sendig message: %s\n",
                           strerror(errno));
                    exit(9);
                }
            }
        }
    }
    return 0;
}
