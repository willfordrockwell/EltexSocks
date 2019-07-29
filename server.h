#include <netinet/udp.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>

#define PORT_LENGTH 6
#define IP_LENGTH 17
#define MAX_CLIENTS 5
#define MSG_LEN 72
#define NO_FLAGS 0
#define UDP_HDR_SIZE sizeof(struct udphdr)
#define IP_HDR_SIZE sizeof(struct iphdr)

void Get_Port(char *Auto_Port, char *Server_Port);
void Get_IP(char* Auto_IP, char *Server_IP);
