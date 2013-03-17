#include "message.h"

void Set_Message (MESSAGE * pMsg, char type, int datalen, char *pData)
{
	pMsg->dataType = type ;
	pMsg->len = datalen+5;

	if (pMsg->pPacket != NULL)
		free(pMsg->pPacket); /* liberation de la memoire */

	pMsg->pPacket = malloc(datalen+5); /* reservation memoire pour le packet */

	/* construction du packet */
	pMsg->pPacket[0] = type ;
	pMsg->pPacket[1] = datalen >> 24 ;
	pMsg->pPacket[2] = datalen >> 16 ;
	pMsg->pPacket[3] = datalen >> 8 ;
	pMsg->pPacket[4] = datalen ;

	memcpy(pMsg->pPacket+5, pData, datalen);
}

void Put_Int (MESSAGE * pMsg, int value)
{
	pMsg->dataType = 'I' ;
	pMsg->len = 9;

	if (pMsg->pPacket != NULL)
		free(pMsg->pPacket); /* libération de la mémoire */

	pMsg->pPacket = malloc(pMsg->len); /* réservation mémoire pour le packet */

	/* construction du packet */
	pMsg->pPacket[0] = pMsg->dataType ;
	pMsg->pPacket[4] = 4 ;
	pMsg->pPacket[3] = 0 ;
	pMsg->pPacket[2] = 0 ;
	pMsg->pPacket[1] = 0 ;

	pMsg->pPacket[8] = value ;
	pMsg->pPacket[7] = value >> 8 ;
	pMsg->pPacket[6] = value >> 16 ;
	pMsg->pPacket[5] = value >> 24 ;
}

void Put_Char (MESSAGE * pMsg, char c)
{
	pMsg->dataType = 'C' ;
	pMsg->len = 6;

	if (pMsg->pPacket != NULL)
		free(pMsg->pPacket); /* libération de la mémoire */

	pMsg->pPacket = malloc(pMsg->len); /* réservation mémoire pour le packet */

	/* construction du packet */
	pMsg->pPacket[0] = pMsg->dataType ;
	pMsg->pPacket[4] = 1 ;
	pMsg->pPacket[3] = 0 ;
	pMsg->pPacket[2] = 0 ;
	pMsg->pPacket[1] = 0 ;

	pMsg->pPacket[5] = c ;
}

void Put_String (MESSAGE * pMsg, const char * str)
{
	pMsg->dataType = 'S' ;
	int datalen = strlen(str);
	pMsg->len = datalen + 5;

	if (pMsg->pPacket != NULL)
		free(pMsg->pPacket); /* libération de la mémoire */

	pMsg->pPacket = malloc(pMsg->len); /* réservation mémoire pour le packet */

	/* construction du packet */
	pMsg->pPacket[0] = pMsg->dataType ;
	pMsg->pPacket[4] = datalen ;
	pMsg->pPacket[3] = datalen >> 8 ;
	pMsg->pPacket[2] = datalen >> 16 ;
	pMsg->pPacket[1] = datalen >> 24 ;

	memcpy(pMsg->pPacket+5, str, pMsg->len-5);
}

void Print (MESSAGE * pMsg, int MaxSize)
{
	int i;
	printf("type: %c\n", pMsg->dataType);
	printf("size: %d\n", pMsg->len);
	//for (i=0 ; i< pMsg->len ; i++)
	for (i=0 ; i< 5 ; i++)
		printf("%x ", (pMsg->pPacket[i] & (0x0000ff)));
	printf("\n");
	if (MaxSize > pMsg->len-5)
		MaxSize = pMsg->len-5;
	for (i=0; i< MaxSize; i++){
		printf("%x ", (pMsg->pPacket[5+i] & (0x0000ff)));
		if ((i+1)%25 == 0)
			printf("\n");
	}
	printf("\n");
}

int Read_Message(MESSAGE * pMsg, int idSocket){
	char type = 'Z';
	char buf[4];
	char *pData;
	int size = 0;
	int retour = 0;

	retour = recv(idSocket, &type, 1, 0); 			/* Ecriture du type */
	retour = recv(idSocket, buf, 4, 0);				/* Data size*/
	size = ByteArray_To_Int(buf);
	pData = malloc(size);
	retour = recv(idSocket, pData, size, 0);

	Set_Message(pMsg, type, size, pData);
	return retour;
}

void Send_Message(int connectSocket, char * pData, int len) {
	int iResult;
	iResult = send( connectSocket, pData, len, 0 );
	if (iResult < 0) {
		perror("send() failed with error: \n");
	} else {
		printf("iresul: %d \n",iResult);
	}
}

int ByteArray_To_Int(char buf[4])
{
	int i = 0;
	i = ((buf[0]& 0xFF) << 24 );
	i += ((buf[1]& 0xFF) << 16 );
	i += ((buf[2]& 0xFF) << 8 );
	i += (buf[3] & 0xFF);

	return i;
}