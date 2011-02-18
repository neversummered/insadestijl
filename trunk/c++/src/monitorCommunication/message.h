#ifndef MESSAGE_H
#define MESSAGE_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/socket.h>
#include "../video/jpegimage.h"
#include "../data/position.h"
#include "../data/battery.h"


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

namespace robotInsa {

    class Message {
    public:
        Message (){
            dataType = 0;
            len = 0;
            data = NULL;
        }

        void setMessage(char type, int l, char *d);
        void setInt(int value);
        void setChar(char c);
        void setString(const char* str);
        void setJpegImage(JpegImage *cImg);
        void setPosition(Position p);
        void setBattery(Battery b);

        char getType(){
            return dataType;
        }
        int getLenght(){
            return len;
        }
        char * getData(){
            return data;
        }

        void print(int maxChar);

    private:
        char dataType;
        int len;
        char *data;
    };
}

#endif
