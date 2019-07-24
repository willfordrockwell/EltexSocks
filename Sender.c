#include "server.h"

int main(int argc, char const *argv[]) //Port
{
    char Message[MSG_LEN];

    char Server_IP[IP_LENGTH];
    char Port_Str[PORT_LENGTH];
    int Port;

    int Socket;

    int Broadcast = 1;

    struct sockaddr_in Server_Addr;
    socklen_t Server_Addr_Len = sizeof(Server_Addr);

    if (argc < 2) {
        Get_Port("12345", Port_Str);
    }
    else {
        strcpy(Port_Str, argv[1]);
    }

    Port = htons(atoi(Port_Str));

    Server_Addr.sin_family = AF_INET;
    Server_Addr.sin_addr.s_addr = htonl(INADDR_BROADCAST);
    Server_Addr.sin_port = Port;

    //Connect to Server
    if ((Socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) >= 0) {
        printf("Socket created: %d\n", Socket);
    }
    else {
        printf("Error creating socket: %s\n", strerror(errno));
        exit(1);
    }

    //allows broadcast
    if (setsockopt(Socket, SOL_SOCKET, SO_BROADCAST, &Broadcast, sizeof(Broadcast)) == 0) {
        printf("Broadcast flag setted\n");
    }
    else {
        printf("Error setting flag: %s\n", strerror(errno));
        exit(2);
    }

    //Send a message to all
    printf("Type message to all\n");

    fgets(Message, MSG_LEN, stdin);
    *(Message + strlen(Message)) = '\0';

    if (sendto(Socket, Message, strlen(Message), MSG_CONFIRM,
               (struct sockaddr *) &Server_Addr, Server_Addr_Len) > 0) {
        printf("Send to all: %s\n", Message);
    }
    else {
        printf("Error on sendig message: %s\n", strerror(errno));
        exit(3);
    }
    close(Socket);
    return 0;
}
