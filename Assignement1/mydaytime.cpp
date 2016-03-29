#include <stdio.h> // basic I/O
#include <stdlib.h>
#include <sys/types.h> // standard system types
#include <netinet/in.h> // Internet address structures
#include <sys/socket.h> // socket API
#include <arpa/inet.h>
#include <netdb.h> // host to IP resolution
#include <string.h>
#include <unistd.h>
#include <iostream>
using namespace std;

#define HOSTNAMELEN 40 // maximal host name length; can make it variable if you want
#define BUFLEN 1024 // maximum response size; can make it variable if you want
#define DEST_PORT 13 // port of daytime server

int main(int argc, char *argv[])
{
  // define your variables here
  string DEST_IP = argv[1];

printf( "something");

  // check that there are enough parameters
  if (argc != 2)
    {
      fprintf(stderr, "Usage: mydaytime <hostname>\n");
      exit(-1);
    }

  // Write your code here
int sockfd;
struct sockaddr_in dest_addr; // will hold the destination addr

sockfd = socket(AF_INET, SOCK_STREAM, 0); 	// do some error checking!
dest_addr.sin_family = AF_INET; 		// host byte order
dest_addr.sin_port = htons(DEST_PORT); 		// short, network byte order

dest_addr.sin_addr.s_addr = inet_addr(DEST_IP.c_str());
memset(&(dest_addr.sin_zero),'\0', 8); // zero the rest of the struct

connect(sockfd, (struct sockaddr *) &dest_addr, sizeof(struct sockaddr));

char buff[30] = "GET TIME\r\n";
int result = send(sockfd, buff, 30, 0);

char returnedBuff[100];
int received = recv(sockfd, returnedBuff, 100, 0);
cout << "Received: " << received;
cout << returnedBuff;
  return 0;
}
