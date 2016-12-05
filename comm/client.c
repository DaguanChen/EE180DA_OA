#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 

void error(const char *msg)
{
	perror(msg);
	exit(0);
}

int main(int argc, char *argv[])
{
	int client_socket_fd, portno, n;
	struct sockaddr_in serv_addr;
	struct hostent *server;
	char buffer[256];	
		
	// Read command line arguments, need to get the host IP address and port
	if (argc < 3) {
		fprintf(stderr,"usage %s hostname port\n", argv[0]);
		exit(0);
	}

	// Convert the arguments to the appropriate data types
	portno = atoi(argv[2]);

	/* setup the socket
	 *	AF_INET 	->	IPv4
	 *	SOCK_STREAM 	->	TCP
	 */

	client_socket_fd = socket(AF_INET, SOCK_STREAM, 0);

	// check if the socket was created successfully. If it wasnt, display an error and exit
	if(client_socket_fd < 0) {
		error("ERROR opening socket");
	}

	// check if the IP entered by the user is valid 
	server = gethostbyname(argv[1]);
	if (server == NULL) {
		fprintf(stderr,"ERROR, no such host\n");
		exit(0);
	}

	printf("setting up more of the socket");
	
	// clear our the serv_addr buffer
	memset((char *) &serv_addr, 0, sizeof(serv_addr));
	// set up the socket 
	serv_addr.sin_family = AF_INET;	
	memcpy((char *)&serv_addr.sin_addr.s_addr, (char *)server->h_addr, server->h_length);
	serv_addr.sin_port = htons(portno);
	
	// try to connect to the server
	if (connect(client_socket_fd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0){ 
		error("ERROR connecting");
	}
	
	/*
	// get user input
	printf("Please enter the message: ");
	memset(buffer, 0 ,256);
	fgets(buffer, 255, stdin); // the part that actually gets the user input
	
	// send user input to the server
	n = write(client_socket_fd,buffer,strlen(buffer));
	*/
	int i = 13;
	n = write(client_socket_fd,&i,2);
	
	// n contains how many bytes were received by the server
	// if n is less than 0, then there was an error
	if (n < 0) {
		error("ERROR writing to socket");
	}

	// clear out the buffer
	memset(buffer, 0, 256);

	//sleep(1);

	// get the response from the server and print it to the user
	n = read(client_socket_fd, buffer, 255);
	if (n < 0) {
		error("ERROR reading from socket");
	}
	printf("%s\n",buffer);

	// clean up the file descriptors
	close(client_socket_fd);
	
	return 0;
}
