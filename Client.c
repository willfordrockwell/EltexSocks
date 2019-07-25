#include "server.h"

int main(int argc, char const *argv[]) //IP server, Port server, Port Client
{
    char Message[MSG_LEN + 28];

    char Server_IP[IP_LENGTH];
    char Port_Str[PORT_LENGTH];
    int Server_Port;

    int Port;

    int Socket;

    struct sockaddr_in Server_Addr;
    socklen_t Server_Addr_Len = sizeof(Server_Addr);

    struct udphdr *Header;

    Header = (struct udphdr *)Message;

    if (argc < 4) {
        Get_IP("127.0.0.1", Server_IP);
    }
    else {
        strcpy(Server_IP, argv[1]);
    }

    if (argc < 4) {
        Get_Port("12345", Port_Str);
    }
    else {
        strcpy(Port_Str, argv[2]);
    }

    Server_Port = htons(atoi(Port_Str));

    if (argc < 4) {
        Get_Port("12346", Port_Str);
    }
    else {
        strcpy(Port_Str, argv[3]);
    }

    Port = htons(atoi(Port_Str));

    //fill udp header
    Header->uh_dport = Server_Port;
    Header->uh_sport = Port;
    Header->uh_sum = 0;

    Server_Addr.sin_family = AF_INET;
    inet_pton(AF_INET, Server_IP, &(Server_Addr.sin_addr));
    Server_Addr.sin_port = Server_Port;

    //Connect to Server
    if ((Socket = socket(AF_INET, SOCK_RAW, IPPROTO_UDP)) >= 0) {
        printf("Socket created: %d\n", Socket);
    }
    else {
        printf("Error creating socket: %s\n", strerror(errno));
        exit(1);
    }

    //Type a message to Server
    printf("Type message to server\n");
    memset(Message + 8, 0, MSG_LEN);

    fgets(Message + 8, MSG_LEN, stdin);
    *(Message + 8 + strlen(Message + 8)) = '\0';

    Header->uh_ulen = htons(strlen(Message + 8) + 8);

    if (sendto(Socket, Message, strlen(Message + 8) + 8, MSG_CONFIRM,
               (struct sockaddr *) &Server_Addr, Server_Addr_Len) > 0) {
        printf("Send to server: %s\n", Message + 8);
    }
    else {
        printf("Error on sendig message: %s\n", strerror(errno));
        exit(2);
    }

    //start recieving
    while (1)
    {
        memset(Message, 0, MSG_LEN + 28);
        if (recvfrom(Socket, Message, MSG_LEN, MSG_WAITALL,
                 (struct sockaddr *) &Server_Addr, &Server_Addr_Len) == -1) {
            printf("Error on recieving message: %s\n", strerror(errno));
            exit(3);
        }

        //reading udp header
        Header = (struct udphdr *)(Message + 20);
        
        if (Header->uh_dport == Port && Header->uh_sport == Server_Port) {
            printf("Message founded!\n");
            break;
        }
    }
    close(Socket);
    return 0;
}
