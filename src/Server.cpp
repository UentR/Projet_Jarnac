#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <iostream>

#define PORT 4000

using namespace std;

void error(char *msg) {
  perror(msg);
  exit(1);
}

int main() {
  int sockfd, newsockfd, portno, n;
  socklen_t clilen;
  char buffer[256];

  // Struct for an internet adress
  struct sockaddr_in serv_addr, cli_addr;

  sockfd = socket(AF_INET, SOCK_STREAM, 0);

  // Check if socket was created successfully
  if (sockfd < 0) error("ERROR opening socket");

  // Empty buffer
  bzero((char *)&serv_addr, sizeof(serv_addr));

  // Port number
  portno = PORT;
  int opt = 1;

  if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt,
                 sizeof(opt))) {
    perror("setsockopt");
    exit(EXIT_FAILURE);
  }

  // Set up the server adress
  serv_addr.sin_port =
      htons(portno);               // Convert port number to network byte order
  serv_addr.sin_family = AF_INET;  // Symbolic constant for the address family
  serv_addr.sin_addr.s_addr = INADDR_ANY;  // IP address of the host

  // Bind the socket to the adress
  if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    error("ERROR on binding");

  // Listen for connections
  listen(sockfd, 1);

  clilen = sizeof(cli_addr);

  for (int i = 0; i < 5; i++) {
    // Accept a connection

    newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);
    if (newsockfd < 0) error("ERROR on accept");
    // Empty buffer
    bzero(buffer, 256);

    // Read from the socket
    n = read(newsockfd, buffer, 255);
    if (n < 0) error("ERROR reading from socket");
    printf("Here is the message: %s\n", buffer);

    // Write to the socket
    n = write(newsockfd, "I got your message\n", 19);
    if (n < 0) error("ERROR writing to socket");

    // Close the socket
    // close(newsockfd);
    close(sockfd);
  }
  return 0;
}