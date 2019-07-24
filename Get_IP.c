#include "server.h"

void Get_IP(char *Auto_IP, char *Server_IP)
{
    printf("Enter broadcast IP (auto: %s): ", Auto_IP);
    fgets(Server_IP, IP_LENGTH, stdin);
    if (strncmp(Server_IP, "", 1)) {
        strcpy(Server_IP, Auto_IP);
    }
}