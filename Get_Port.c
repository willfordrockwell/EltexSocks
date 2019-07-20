#include "server.h"

void Get_Port(char *Auto_Port, char *Server_Port)
{
    printf("Enter server's port (auto: %s): ", Auto_Port);
    fgets(Server_Port, PORT_LENGTH, stdin);
    if (strncmp(Server_Port, "", 1)) {
        strcpy(Server_Port, Auto_Port);
    }
}