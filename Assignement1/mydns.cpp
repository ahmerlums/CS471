#include <string>
#include <stdio.h> // basic I/O
#include <stdlib.h>
#include <sys/types.h> // standard system types
#include <netinet/in.h> // Internet address structures
#include <sys/socket.h> // socket API
#include <arpa/inet.h>
#include <netdb.h> // host to IP resolution
#include <unistd.h>
#include <iostream>
#define HOSTNAMELEN 40 // maximal host name length; can make it variable if you want
#define BUFLEN 1024 // maximum response size; can make it variable if you want
using namespace std;
int main(int argc, char *argv[])
{

    // check that there are enough parameters
    if (argc != 2)
    {
        fprintf(stderr, "Usage: mydns <hostname>\n");
        exit(-1);
    }

    struct hostent* a=  gethostbyname(argv[1]);

    if (a==NULL)
        cout << "Invalid Host"<<endl;
    else
    {
	int i=0;
        while(a->h_addr_list[i] != 0)
        {
            cout << inet_ntoa(*(struct in_addr *)(a->h_addr_list[i]))<<endl;
            i++;
        }
    }

  // Address resolution stage by using gethostbyname()

  // Write your code here!
  // Print to standard output

  return 0;
}
