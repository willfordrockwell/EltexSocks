#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/poll.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>

#define PORT_LENGTH 6
#define IP_LENGTH 17
#define MAX_CLIENTS 5
#define MSG_LEN 80
#define NO_FLAGS 0
#define NUM_SOCKETS 2
#define TIME_OUT 1

void Get_Port(char *Auto_Port, char *Server_Port);
void Get_IP(char* Auto_IP, char *Server_IP);
