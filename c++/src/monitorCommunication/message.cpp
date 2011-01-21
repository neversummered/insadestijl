#include "message.h"
#include "../video/jpegimage.h"


namespace robotInsa {

    void int2ByteArray(int i, char* b){
        *b = i >> 24;
        *(b+1) = i >> 16;
        *(b+2) = i >> 8;
        *(b+3) = i;
    }

    void Message::setMessage(char type, int datalen, char *d) {
        dataType = type;
        len = datalen + 5;

        if (data != NULL)
            free(data); /* liberation de la memoire */

        data = (char*)malloc(datalen + 5); /* reservation memoire pour le packet */

        /* construction du packet */
        data[0] = type;
        data[1] = datalen >> 24;
        data[2] = datalen >> 16;
        data[3] = datalen >> 8;
        data[4] = datalen;

        memcpy(data + 5, d, datalen);
    }

    void Message::setInt(int value) {
        dataType = 'I';
        len = 9;

        if (data != NULL)
            free(data); /* libération de la mémoire */

        data = (char*)malloc(len); /* réservation mémoire pour le packet */

        /* construction du packet */
        data[0] = dataType;
        data[4] = 4;
        data[3] = 0;
        data[2] = 0;
        data[1] = 0;

        data[5] = value >> 24;
        data[6] = value >> 16;
        data[7] = value >> 8;
        data[8] = value;
    }

    void Message::setChar(char c) {
        dataType = 'C';
        len = 6;

        if (data != NULL)
            free(data); /* libération de la mémoire */

        data = (char*)malloc(len); /* réservation mémoire pour le packet */

        /* construction du packet */
        data[0] = dataType;
        data[4] = 1;
        data[3] = 0;
        data[2] = 0;
        data[1] = 0;

        data[5] = c;
    }

    void Message::setString(const char* str) {
        dataType = 'S';
        int datalen = strlen(str);
        len = datalen + 5;

        if (data != NULL)
            free(data); /* libération de la mémoire */

        data = (char*)malloc(len); /* réservation mémoire pour le packet */

        /* construction du packet */
        data[0] = dataType;
        data[4] = datalen;
        data[3] = datalen >> 8;
        data[2] = datalen >> 16;
        data[1] = datalen >> 24;

        memcpy(data + 5, str, len - 5);
    }


     void Message::setJpegImage(JpegImage *cImg){
        setMessage('V', (cImg->getJpegImg()->cols)*sizeof(uchar), (char *)cImg->getJpegImg()->data.ptr);
     }

     void Message::setPosition(Position p){
         /*int len = 3*sizeof(float);
         char data[3*sizeof(float)];

         float tmp = p.getX();
         memcpy(&data[0], &tmp , sizeof(float));
         tmp = p.getY();
         memcpy(&data[sizeof(float)], &tmp , sizeof(float));
         tmp = p.getOrientation();
         memcpy(&data[2*sizeof(float)], &tmp , sizeof(float));*/

         char data[3];

         //setMessage('P', len, data);
     }
    
}
