// header files
#include <stdio.h>
#include <sys/types.h>

//Socket library has sockaddr( methods like bind(), connect(),etc)
#include <sys/socket.h>

//Includes functions like htons() which converts host to network byte order
//and structures like sockaddr_in which provides basic syscalls and functions
//that deal with internet addresses
#include <netinet/in.h>

//this function is to print error messages
void error(char *msg){
    printf("%s\n",msg);
    exit(1);
}

// function to create server
void createServer(int port){

    //string buffer for the messege to want to sent to client
    char buffer[256];

    //creation of unbound socket
    
    //AF_INET (domain): address family that is used to designate the type of addresses that your socket can communicate with. 
    
    //SOCK_STREAM (type): This is for TCP, connection-based protocol
    
    //protocol: 0 specifies default protocol
    
    //returns a file descriptor, in case of success returns non-negtive value
    int sockfd=socket(AF_INET,SOCK_STREAM,0);

    //is case socket creation is unsuccessful
    if(sockfd<0){
    	error("ERROR: Socket not opening");
    }
    
    //two structures of type sockaddr_in
    struct sockaddr_in serv_addr, cli_addr;

    //clears the serv_addr
    bzero((char*)&serv_addr,sizeof(serv_addr));

    //sin_family: address family for the transport address
    serv_addr.sin_family=AF_INET;

    //sin_addr: the IPaddress in the socket
    //sin_addr is a union, so we specify s_addr
    //a_addr: 4 bytes integer

    //INADDR_ANY: allows server to recieve packets from asny address
    serv_addr.sin_addr.s_addr=INADDR_ANY;

    //sin_port: identifies the port to which the application mist bind
    //htons: host to network byte order
    serv_addr.sin_port = htons(port);

    
    //bind assigns a local socket address 
    //return 0 on success, negetive value on failure
    if(bind(sockfd,(struct sockaddr*)&serv_addr,sizeof(serv_addr))<0){
    	error("ERROR: binding error occured");
    }

    //function to listen at he socket
    //second argument specifies the number of connections that can be made
    listen(sockfd,5);

    int clilen=sizeof(cli_addr);
	
    //accept estract the first connection on the queue of pending connections, create a new socket with the same socket type protocol and address family

    int newsockfd=accept(sockfd,(struct sockaddr*)&cli_addr,&clilen);

    int n;

    //in case of socket creation failure
    if(newsockfd<0){
    	error("Error: Cant accept request");
    }

    //clears the buffer
    bzero(buffer,256);

    //read the message send by client and put it in the buffer
    n=read(newsockfd,buffer,255);

    //in case of failure to read message
    if(n<0){
    	error("ERROR: cant read from socket");
    }

    //prints the message
    printf("Message: %s\n",buffer);

    //writes into the buffer and send to client
    n= write(newsockfd,"Server got your message",24);

    if(n<0){
    	error("ERROR: cant write to socket");
    }
}

int main(){

    int port;
    printf("Enter the port number\n");
    scanf("%d",&port);
    createServer(port);
    return 0;
}
