//neccessart header files needed
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

//Socket librart has sockaddr(methods like bind(), connect(), etc);
#include <sys/socket.h>

//Includes funtions like htons() and structures liek sockaddr_in
#include <netinet/in.h>

//defines structure like hostent
#include <netdb.h>


//function to print error message
void error(char *msg){
    printf("%s",msg);
    exit(0);
}

//function to create client to connect to server
void createClient(char * hostname, int portno){

    int sockfd,n;

    //structure of type sockaddr_in
    struct sockaddr_in serv_addr;

    //structure of type hostent
    struct hostent *server;

    //create a string buffer
    char buffer[256];

    //open the unbound socket
    
    //AF_INET (domain): address family that is used to designate the type of addresses that your socket can communicate with

    //SOCK_STREAM (type): This is for TCP, connection_based protocol

    sockfd=socket(AF_INET, SOCK_STREAM, 0);

    //in case socket opening is failure
    if(sockfd<0){
    	error("ERROR opening, socket");
    }


    server=gethostbyname(hostname);
    if(server==NULL){
    	fprintf(stderr,"ERROR, no such host\n");
	exit(0);
    }

    bzero((char*)&serv_addr,sizeof(serv_addr));
    serv_addr.sin_family=AF_INET;

    bcopy((char *)server->h_addr,(char*)&serv_addr.sin_addr.s_addr,server->h_length);

    serv_addr.sin_port=htons(portno);

    if(connect(sockfd,&serv_addr,sizeof(serv_addr))<0){
    	error("ERROR connecting");
    }
    printf("Enter the message: ('_' separated)");
    bzero(buffer,256);
    scanf("%s",buffer);
    n=write(sockfd,buffer,strlen(buffer));

    if(n<0){
    	error("ERROR writing from socket");
    }

    bzero(buffer,256);
    n=read(sockfd,buffer,255);

    if(n<0){
    	error("ERROR reading from socket");
    }

    printf("%s\n",buffer);

}
int main(){
    char hostname[256];
    int portno;

    printf("Enter the hostname\n");
    scanf("%s",hostname);

    printf("Enter the port number\n");
    scanf("%d",&portno);

    createClient(hostname,portno);
    return 0;
}
