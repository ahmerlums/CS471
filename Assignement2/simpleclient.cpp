#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <iostream>
#include <fstream>
using namespace std;

int main(int argc,char *argv[])
{
    if (argc < 3)
    {
        printf("port or address not provided\n");
        exit(1);
    }
    int sockfd=socket(AF_INET,SOCK_STREAM,0);
    if (sockfd < 0)
        printf("ERROR opening socket");

    struct hostent *server =  gethostbyname(argv[1]);
    if (server == NULL)
    {
        printf("no such host\n");
        exit(0);
    }

    struct sockaddr_in server_address;
    bzero((char *) &server_address, sizeof(server_address));
    int port = atoi(argv[2]);
    server_address.sin_family = AF_INET;
    bcopy((char *)server->h_addr,
          (char *)&server_address.sin_addr.s_addr,
          server->h_length);
    server_address.sin_port = htons(port);
    connect(sockfd,(struct sockaddr *) &server_address,sizeof(server_address));
    printf("Please enter the message:");
    char sendbuffer[256];
    char recvbuf[256];
    int recvbuflen = 256;
    while (sendbuffer[0]!='e' && sendbuffer[1]!='x' || sendbuffer[2]!='i' || sendbuffer[3]!='t')
    {
        bzero(sendbuffer,256);
        cin >> sendbuffer;
        int n = send(sockfd,sendbuffer,strlen(sendbuffer),0);
        if (n<0)
            cout <<  "Error in sending messege"<<endl;
        bzero(recvbuf,256);
        n = recv(sockfd,recvbuf,recvbuflen,0);
        if (n<0)
            cout <<  "Error in recieving messege"<<endl;
        printf(recvbuf);
      
    }
}



