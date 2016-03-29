#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include <netinet/in.h> // Internet address structures
#include <arpa/inet.h>
#include <netdb.h> // host to IP resolution
#include <unistd.h>
#include <ctime>
#include <sstream>
#include <fstream>
#include <dirent.h>
using namespace std;
string converttoString(int a) ;
int main(int argc,char *argv[])
{
    if (argc < 2)
    {
        printf("no port provided\n");
        exit(1);
    }
    struct hostent* a=  gethostbyname("venus.lums.edu.pk");
    string ip;
    int i=0;
    while(a->h_addr_list[i] != 0)
    {
        ip = inet_ntoa(*(struct in_addr *)(a->h_addr_list[i]));
        i++;
    }
    ip = ip + "\n";
    int sockfd=socket(AF_INET,SOCK_STREAM,0);
    if (sockfd < 0)
        printf("ERROR opening socket");


    struct sockaddr_in serveraddress;
    bzero((char *) &serveraddress, sizeof(serveraddress));
    int port = atoi(argv[1]);
    serveraddress.sin_family = AF_INET;
    serveraddress.sin_addr.s_addr = INADDR_ANY;
    serveraddress.sin_port = htons(port);
    bind(sockfd,(struct sockaddr *) &serveraddress,sizeof(serveraddress));
    listen(sockfd,5);
    struct sockaddr clientaddress;
    socklen_t clientlen=sizeof(clientaddress);
    int clientsockfd = accept(sockfd,
                              (struct sockaddr *) &clientaddress,
                              &clientlen);
    if (clientsockfd < 0)
        cout << "ERROR on accept";

    else 
 	cout << "Client connected \n";
    char recvbuf[256];
    int recvbuflen = 256;
    char sendbuf[256];
    int sendbuflen = 256;



    while (recvbuf[0]!='e'||recvbuf[1]!='x'||recvbuf[2]!='i'||recvbuf[3]!='t')
    {
        bzero(recvbuf,256);
        int n = recv(clientsockfd,recvbuf,recvbuflen,0);
	cout << recvbuf << endl;
        if (n<0)
            printf("Error in recieving");
        if (recvbuf[0]=='i' && recvbuf[1]=='p')
        {
            bzero(sendbuf,256);
            strcpy(sendbuf,ip.c_str());
            n = send(clientsockfd,sendbuf,sendbuflen,0);
            if (n<0)
                printf("Error in sending");
        }
	else if (recvbuf[0]=='l' && recvbuf[1]=='i' && recvbuf[2]=='s' && recvbuf[3] == 't')
        {
            DIR *mydir;
            mydir = opendir(".");
            struct dirent *dp;
            string filenames ;
            while( (dp=readdir(mydir))!=NULL)
            {
                string temp=dp->d_name;
                filenames=filenames+temp+"\n";
            }

            bzero(sendbuf,256);
            strcpy(sendbuf,filenames.c_str());
            n = send(clientsockfd,sendbuf,sendbuflen,0);


        }

	else if (recvbuf[0]=='h' && recvbuf[1]=='e' && recvbuf[2]=='l' && recvbuf[3] == 'p')
	{
	    string temp = "Current commands available are: \nip \ntime \nhelp \nlist \nos \nexit\n";
            strcpy(sendbuf,temp.c_str());
            n = send(clientsockfd,sendbuf,sendbuflen,0);
            if (n<0)
                printf("Error in sending");
	}

	else if (recvbuf[0]=='o' && recvbuf[1]=='s')
	{
	   strcpy(sendbuf,"Linux \n");
           n = send(clientsockfd,sendbuf,sendbuflen,0);
	   if (n<0)
                printf("Error in sending");

	}

	else if (recvbuf[0]=='t' && recvbuf[1]=='i' && recvbuf[2]=='m' && recvbuf[3] == 'e')
        {

            time_t currentTime;
            struct tm *localTime;

            time( &currentTime );
            localTime = localtime( &currentTime );
            int Day    = localTime->tm_mday;
            int Month  = localTime->tm_mon + 1;
            int Year   = localTime->tm_year + 1900;
            int Hour   = localTime->tm_hour;
            int Min    = localTime->tm_min;
            int Sec    = localTime->tm_sec;
            string toreturn = "The current local time at the server is: " + converttoString (Hour) + ":"+converttoString(Min)+":"+converttoString(Sec)+ "  "+ converttoString(Day)+ "/" +converttoString( Month) + "/"+ converttoString(Year)+"\r\n";

            strcpy(sendbuf,toreturn.c_str());
            n = send(clientsockfd,sendbuf,sendbuflen,0);
            if (n<0)
                printf("Error in sending");


        }
        else
        {
            bzero(sendbuf,256);
            strcpy(sendbuf,"Enter valid command \n");
            n = send(clientsockfd,sendbuf,sendbuflen,0);
            if (n<0)
                printf("Error in sending");
        }
    }
}


string converttoString(int a)
{
    string Result;

    ostringstream convert;

    convert << a;

    Result = convert.str();
    return Result;
}

