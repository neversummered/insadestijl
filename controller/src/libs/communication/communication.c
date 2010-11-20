#include "communication.h"

int Open_Server(int port)
{
struct sockaddr_in  serverAddr;    /* server's socket address */
struct sockaddr_in  clientAddr;    /* client's socket address */
int                 sockAddrSize;  /* size of socket address structure */
int optval;

int connectSocket ;
int sd;

	if ((sd = socket (AF_INET, SOCK_STREAM, 0)) < 0)
	{
		printf ("Erreur a l'ouverture du socket\n");
		return (-1);
	}

	optval = 1;
	setsockopt(sd,SOL_SOCKET,SO_REUSEADDR,(char *)&optval,sizeof(optval));

	// initialisation de l'adresse locale
	sockAddrSize = sizeof (struct sockaddr_in);
	bzero ((char *) &serverAddr, sockAddrSize);
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons (port);
	serverAddr.sin_addr.s_addr = htonl (INADDR_ANY);

	if (bind(sd, (struct sockaddr *) (&serverAddr), sockAddrSize) < 0)
	{
		printf ("Erreur au niveau du bind\n");
		if (sd)
		{
			close(sd);
		}
		return (-1);
	}

	if (listen(sd, 5) < 0 )
	{
		printf("listen failed\n");
		close(sd);
		return (-1);
	}

	printf ("Attente de connexion client sur port %d \n",port);

	if ((connectSocket = accept(sd, (struct sockaddr *) (&clientAddr), &sockAddrSize)) < 0)
	{
		printf("erreur au niveau du accept\n");
		close(sd);
		return (-1);
	}
	close(sd);

	printf ("Socket %d ouverte \n",connectSocket);

	return connectSocket;
}

void Close_Server(int connectSocket) {
	close(connectSocket);
	printf("Server closed\n");
}
