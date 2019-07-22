#include "server.h"

int main(int argc, char const *argv[]) //IP server, Port server
{
    char Message[MSG_LEN];

    char Server_IP[IP_LENGTH];
    char Port_Str[PORT_LENGTH];
    int Port;

    int Socket;

    struct sockaddr_in Server_Addr;
    socklen_t Server_Addr_Len = sizeof(Server_Addr);

    if (argc < 3) {
        Get_IP("127.0.0.1", Server_IP);
    }
    else {
        strcpy(Server_IP, argv[1]);
    }

    if (argc < 3) {
        Get_Port("12345", Port_Str);
    }
    else {
        strcpy(Port_Str, argv[2]);
    }

    Port = htons(atoi(Port_Str));

    Server_Addr.sin_family = AF_INET;
    inet_pton(AF_INET, Server_IP, &(Server_Addr.sin_addr));
    Server_Addr.sin_port = Port;

    if ((Socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) >= 0) {
        printf("Socket created: %d\n", Socket);
    }
    else {
        printf("Error creating socket: %s\n", strerror(errno));
        exit(1);
    }

    if (connect(Socket, (struct sockaddr *) &Server_Addr, Server_Addr_Len)
        >= 0) {
        printf("Connected with server\n");
    }
    else {
        printf("Connection to server error: %s\n", strerror(errno));
        exit(2);
    }

    printf("Type message to server\n");

    fgets(Message, MSG_LEN, stdin);
    *(Message + strlen(Message)) = '\0';

    if (send(Socket, Message, strlen(Message), NO_FLAGS) > 0) {
        printf("Send to server: %s\n", Message);
    }
    else {
        printf("Error on sendig message: %s\n", strerror(errno));
        exit(3);
    }
    memset(Message, 0, MSG_LEN);
    if (recv(Socket, Message, MSG_LEN, NO_FLAGS) > 0) {
        printf("Recieved from server: %s\n", Message);
    }
    else {
        printf("Error on recieving message: %s\n", strerror(errno));
        exit(4);
    }
    shutdown(Socket, SHUT_RDWR);
    return 0;
}
