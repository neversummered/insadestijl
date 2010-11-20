#ifndef __COMMUNICATION__
#define __COMMUNICATION__

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

/**
 * Open a server
 * @param port of the socket
 * @param int id of the open socket
 */
int Open_Server(int port);

/**
 * close a socket
 * @param connectSocket id of the socket
 */
void Close_Server(int connectSocket);

#endif
