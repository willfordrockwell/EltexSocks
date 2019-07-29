#include "server.h"

int main(int argc, char const *argv[]) //IP server, Port server, Port Client
{
    char Message[MSG_LEN + UDP_HDR_SIZE + IP_HDR_SIZE];

    char Server_IP[IP_LENGTH];
    char Port_Str[PORT_LENGTH];
    int Server_Port;

    int Port;

    int Socket;

    int SetVal = 1;

    struct sockaddr_in Server_Addr;
    socklen_t Server_Addr_Len = sizeof(Server_Addr);

    struct udphdr *Udp_Header;

    struct iphdr *Ip_Header;

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

    //set internet-layer
    setsockopt(Socket, IPPROTO_IP, IP_HDRINCL, &SetVal, sizeof(SetVal));

    //Type a message to Server
    printf("Type message to server\n");
    memset(Message + UDP_HDR_SIZE + IP_HDR_SIZE, 0, MSG_LEN);

    fgets(Message + UDP_HDR_SIZE + IP_HDR_SIZE, MSG_LEN, stdin);
    *(Message + UDP_HDR_SIZE + IP_HDR_SIZE + strlen(Message + UDP_HDR_SIZE
                                                    + IP_HDR_SIZE)) = '\0';

    //fill udp header
    Udp_Header = (struct udphdr *)(Message + IP_HDR_SIZE);
    Udp_Header->uh_dport = Server_Port;
    Udp_Header->uh_sport = Port;
    Udp_Header->uh_ulen = htons(strlen(Message + UDP_HDR_SIZE + IP_HDR_SIZE)
                                + UDP_HDR_SIZE);
    Udp_Header->uh_sum = 0;

    //fill ip header
    Ip_Header = (struct iphdr *)Message;
    Ip_Header->version = 4;
    Ip_Header->ihl = 5;
    Ip_Header->tos = 0;
    Ip_Header->tot_len = 0;//kernel do it
    Ip_Header->id = 0;
    Ip_Header->frag_off = 0;
    Ip_Header->ttl = 64;
    Ip_Header->protocol = IPPROTO_UDP;
    Ip_Header->check = 0;//kernel do it
    Ip_Header->saddr = 0;//kernel do it
    Ip_Header->daddr = Server_Addr.sin_addr.s_addr;

    if (sendto(Socket, Message, strlen(Message + IP_HDR_SIZE + UDP_HDR_SIZE)
               + IP_HDR_SIZE + UDP_HDR_SIZE, MSG_CONFIRM,
               (struct sockaddr *) &Server_Addr, Server_Addr_Len) > 0) {
                    printf("IP starts: %x\nMessage starts: %x\n", Ip_Header,
                           &Message[0]);
                    printf("UDP starts: %x\nMessage + starts: %x\n", Udp_Header,
                           &Message[IP_HDR_SIZE]);
                    printf("Send to server IP header:\nver: %x\nihl: %x\n"
                    "tos: %x\ntot_len: %x\nid: %x\nflagoff: %x\nttl: %x\n"
                    "proto: %x\ncheck: %x\nsaddr: %x\ndaddr: %x\n",
                    Ip_Header->version, Ip_Header->ihl, Ip_Header->tos,
                    Ip_Header->tot_len, Ip_Header->id, Ip_Header->frag_off,
                    Ip_Header->ttl, Ip_Header->protocol, Ip_Header->check,
                    Ip_Header->saddr, Ip_Header->daddr);
                    printf("Send to server message : %s\n", 
                           Message + IP_HDR_SIZE + UDP_HDR_SIZE);
    }
    else {
        printf("Error on sendig message: %s\n", strerror(errno));
        exit(2);
    }

    //start recieving
    while (1)
    {
        memset(Message, 0, MSG_LEN + UDP_HDR_SIZE + IP_HDR_SIZE);
        if (recvfrom(Socket, Message, MSG_LEN, MSG_WAITALL,
                 (struct sockaddr *) &Server_Addr, &Server_Addr_Len) == -1) {
            printf("Error on recieving message: %s\n", strerror(errno));
            exit(3);
        }

        //reading ip and udp headers
        Ip_Header = (struct iphdr *)Message;
        Udp_Header = (struct udphdr *)(Message + IP_HDR_SIZE);
        
        if (Ip_Header->daddr == Server_Addr.sin_addr.s_addr) {
            printf("Message to client's IP\n");
            if (Udp_Header->uh_dport == Port &&
                Udp_Header->uh_sport == Server_Port) {
                    printf("Message to client's Port. Message found: %s\n",
                           Message + IP_HDR_SIZE + UDP_HDR_SIZE);
                    break;
            }
        }
    }
    close(Socket);
    return 0;
}
