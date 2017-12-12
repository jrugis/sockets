#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <pari/pari.h>

#include "solver.h"

#define MAXPENDING 5  // max connection requests

void Die(char const *mess){
  perror(mess); exit(1);
}

void HandleClient(int sock){
  char buffer[BUFFSIZE];
  int received;

  // check for incoming data in loop
  while(true){
    // receive message
    if ((received = recv(sock, buffer, BUFFSIZE, 0)) < 0) {
      Die("Failed to receive bytes from client");
    }
    if(received == 0) break;
    fprintf(stdout, "Received %d bytes, ", received);
    if(buffer[0] != '.'){
      Die("Bad token from client");
    }

    // get results from solver
    int bytes = solve(buffer);
    fprintf(stdout, "Sending %d bytes\n", bytes);
    if (send(sock, buffer, bytes, 0) != bytes) {
      Die("Failed to send bytes to client");
    }
  }
  close(sock);
  fprintf(stdout, "Client disconnected.\n");
}

int main(int argc, char *argv[]) {
  int serversock, clientsock;
  struct sockaddr_in echoserver, echoclient;

  if(argc != 2) {
    fprintf(stderr, "USAGE: <port>\n");
    exit(1);
  }
  pari_init(1000000000,pow(2,12)); // initialize pari

  // create the TCP socket
  if((serversock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0){
    Die("Failed to create socket");
  }

  // construct the server sockaddr_in structure
  memset(&echoserver, 0, sizeof(echoserver));      // clear struct
  echoserver.sin_family = AF_INET;                 // internet ip
  echoserver.sin_addr.s_addr = htonl(INADDR_ANY);  // incoming addr
  echoserver.sin_port = htons(atoi(argv[1]));      // server port

  // bind the server socket
  if(bind(serversock, (struct sockaddr *)
		  &echoserver, sizeof(echoserver)) < 0){
    Die("Failed to bind the server socket");
  }

  // listen on the server socket
  if(listen(serversock, MAXPENDING) < 0){
    Die("Failed to listen on server socket");
  }

  // run until cancelled
  while(1){
    unsigned int clientlen = sizeof(echoclient);
    // wait for client connection
    fprintf(stdout, "Waiting for client.\n");
    if((clientsock =
         accept(serversock, (struct sockaddr *) &echoclient,
                &clientlen)) < 0) {
      Die("Failed to accept client connection");
    }
    fprintf(stdout, "Client connected: %s\n",
                    inet_ntoa(echoclient.sin_addr));
    HandleClient(clientsock);
  }
}
