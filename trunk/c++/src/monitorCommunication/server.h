#ifndef SERVER_H
#define SERVER_H

#include "message.h"

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

namespace robotInsa {

    class Server {
    public:

        int openServer(char * port);

        void closeServer();

        int sendMessage(Message *msg);

        int receiveMessage(Message &msg);

        int getSocketID() {
            return socketID;
        }

        bool isActive() {
            return active;
        }

    private:
        int socketID;
        bool active;
    };
}

#endif
