#ifndef __MESSAGE__
#define __MESSAGE__

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// déclaration des identificateurs des types de données
// entier 16 bits
#define TYPE_ENTIER  'I'
// reel simple précision sur 4 octets          
#define TYPE_REEL  'R' 
// tableau d entiers codés sur  16 bits
#define TYPE_TABLEAU_ENTIERS  'A'
// tableau de réels simple précision
#define TYPE_TABLEAU_REELS 'T'    
// caractère 1 octet
#define TYPE_CAR  'C'             
// chaine de caractères
#define TYPE_CHAINE  'S'          

struct message
{
	char dataType;
	unsigned short len; // size of the data in byte
	char * pPacket;
};
typedef struct message MESSAGE;

// packet
// bit offset
// 0			type (8 bits) type of the message
// 8			size (32 bits) size of the data in byte
// 40			data (size bytes) data to transmit

void Set_Message (MESSAGE * pMsg, char type, int datalen, char *pData) ;
void Print (MESSAGE * pMsg, int MaxSize);
void Put_Int (MESSAGE * pMsg, int value);
void Put_Char (MESSAGE * pMsg, char c);
void Put_String (MESSAGE * pMsg, const char * str) ;

#endif
