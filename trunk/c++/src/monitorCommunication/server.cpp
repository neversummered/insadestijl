#include "server.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>

#define BACKLOG 10

using namespace std;

namespace robotInsa {

    void sigchld_handler(int s) {
        while (waitpid(-1, NULL, WNOHANG) > 0);
    }

    // get sockaddr, IPv4 or IPv6:

    void *get_in_addr(struct sockaddr *sa) {
        if (sa->sa_family == AF_INET) {
            return &(((struct sockaddr_in*) sa)->sin_addr);
        }

        return &(((struct sockaddr_in6*) sa)->sin6_addr);
    }

    int ByteArray_To_Int(char buf[4]) {
        int i = 0;
        i = ((buf[0]& 0xFF) << 24);
        i += ((buf[1]& 0xFF) << 16);
        i += ((buf[2]& 0xFF) << 8);
        i += (buf[3] & 0xFF);

        return i;
    }

    int Server::openServer(char * port) {
        int sockfd; // listen on sock_fd, new connection on new_fd
        struct addrinfo hints, *servinfo, *p;
        struct sockaddr_storage their_addr; // connector's address information
        socklen_t sin_size;
        struct sigaction sa;
        int yes = 1;
        char s[INET6_ADDRSTRLEN];
        int rv;

        memset(&hints, 0, sizeof hints);
        hints.ai_family = AF_UNSPEC;
        hints.ai_socktype = SOCK_STREAM;
        hints.ai_flags = AI_PASSIVE; // use my IP

        if ((rv = getaddrinfo(NULL, port, &hints, &servinfo)) != 0) {
            fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
            return 1;
        }
        // loop through all the results and bind to the first we can
        for (p = servinfo; p != NULL; p = p->ai_next) {
            if ((sockfd = socket(p->ai_family, p->ai_socktype,
                    p->ai_protocol)) == -1) {
                perror("server: socket");
                continue;
            }

            if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes,
                    sizeof (int)) == -1) {
                perror("setsockopt");
                exit(1);
            }

            if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
                close(sockfd);
                perror("server: bind");
                continue;
            }

            break;
        }

        if (p == NULL) {
            fprintf(stderr, "server: failed to bind\n");
            return 2;
        }

        freeaddrinfo(servinfo); // all done with this structure

        if (listen(sockfd, BACKLOG) == -1) {
            perror("listen");
            exit(1);
        }

        sa.sa_handler = sigchld_handler; // reap all dead processes
        sigemptyset(&sa.sa_mask);
        sa.sa_flags = SA_RESTART;
        if (sigaction(SIGCHLD, &sa, NULL) == -1) {
            perror("sigaction");
            exit(1);
        }

        printf("server: waiting for connections...\n");

        sin_size = sizeof their_addr;
        socketID = accept(sockfd, (struct sockaddr *) &their_addr, &sin_size);
        if (socketID == -1) {
            perror("accept");
            exit(1);
        }

        inet_ntop(their_addr.ss_family,
                get_in_addr((struct sockaddr *) &their_addr),
                s, sizeof s);

        printf("server: got connection from %s\n", s);

        close(sockfd); // child doesn't need the listener


        active = true;
        return 0;
    }

    void Server::closeServer() {
        close(socketID);
        active = false;
        printf("Server closed\n");
    }

    int Server::sendMessage(Message* msg) {
        int iResult;

        if (active) {
            iResult = send(socketID, msg->getData(), msg->getLenght(), 0);
            if (iResult < 0) {
                perror("send() failed with error: \n");
            } else {
                printf("Message envoyé \n");
            }
        } else {
            iResult = -2;
        }
        return iResult;
    }

    int Server::receiveMessage(Message &msg) {
        char type = 'Z';
        char buf[4];
        char* pData;
        int size = 0;
        int retour = 0;

        retour = recv(socketID, &type, 1, 0); /* Lecture du type */
        if (retour > 0) {
            retour = recv(socketID, buf, 4, 0);
            if (retour > 0) { /* Lecture de la taille des données*/
                size = ByteArray_To_Int(buf);
                pData = (char *) malloc(size);
                retour = recv(socketID, pData, size, 0);
                if (retour > 0) {
                    msg = Message();
                    msg.setMessage(type, size, pData);
                }
            }
        }

        return retour;
    }

}