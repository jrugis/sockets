#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define BUFFSIZE 4096

void Die(char const *mess) { perror(mess); exit(1); }

int main(int argc, char *argv[]) {
  int sock;
  struct sockaddr_in echoserver;
  char buffer[BUFFSIZE];
  unsigned int echolen;
  int received = 0;

  if (argc != 10) {
    fprintf(stderr, "USAGE: client <server_ip> <port>\n");
	fprintf(stderr, "  <|ZTA>\n");
	fprintf(stderr, "  <real base> <real increment> <real count>\n");
	fprintf(stderr, "  <imaginary base> <imaginary increment> <imaginary count>.\n");
    exit(1);
  }

  /* Create the TCP socket */
  if ((sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
    Die("Failed to create socket");
  }

  /* Construct the server sockaddr_in structure */
  memset(&echoserver, 0, sizeof(echoserver));       /* Clear struct */
  echoserver.sin_family = AF_INET;                  /* Internet/IP */
  echoserver.sin_addr.s_addr = inet_addr(argv[1]);  /* IP address */
  echoserver.sin_port = htons(atoi(argv[2]));       /* server port */

  /* Establish connection */
  fprintf(stdout, "Connecting to server.\n");
  if (connect(sock, (struct sockaddr *) &echoserver, sizeof(echoserver)) < 0) {
    Die("Failed to connect with server");
  }

  /* Send data to the server */
  strcpy(buffer, argv[3]);
  double* vals = (double*)(buffer+8);
  vals[0] = atof(argv[4]);
  vals[1] = atof(argv[5]);
  vals[2] = atof(argv[6]);
  vals[3] = atof(argv[7]);
  vals[4] = atof(argv[8]);
  vals[5] = atof(argv[9]);
  int n = vals[2] * vals[5]; // number of complex values to compute
  *(int*)(buffer+4) = n;
  unsigned int bytes = 16*n;
  if(bytes > BUFFSIZE) {
  	Die("Results exceed buffer size");
  }
  fprintf(stdout, "Sending bytes: %d\n", 7*8);
  if (send(sock, buffer, 7*8, 0) != 7*8) {
    Die("Mismatch in number of sent bytes");
  }

  /* Receive results back from the server */
  unsigned int total = 0;
  unsigned int ret;
  while(total < bytes){
    if ((ret=recv(sock, buffer+total, BUFFSIZE-1, 0)) < 1) {
      Die("Failed to receive bytes from server");
    }
    fprintf(stdout, "Received bytes: %d\n", ret);
	total += ret;
  }
  vals = (double*)(buffer);
  for(int i=0; i<n; i++) fprintf(stdout, "%f %f\n", vals[2*i], vals[(2*i)+1]);
  close(sock);

  exit(0);
}
