/*
 * camera.h
 *
 *  Created on: 18 janv. 2011
 *      Author: P.E. Hladik
 *  Inspired of code from http://www.siteduzero.com/ by NoHar
 */

#ifndef CAMERA_H
#define CAMERA_H

#include "sourcevideo.h"
#include "../tools/toolstime.h"
#include <opencv/highgui.h>

namespace robotInsa
{
    class Camera: public SourceVideo
    {
        public:
            // Constructeur, on lui passe l'indice de la caméra
            Camera( int index = -1 );

            // Destructeur
            virtual ~Camera();

            // Ouverture (héritée)
            virtual void open();

            // Fermeture (héritée)
            virtual void close();

            // Lecture (héritée)
            virtual void getFrame( Image &frame );

            // Ouverture surchargée ;)
            void open( int index );

        private:
            // La capture OpenCV
            CvCapture* mCapture;

            // L'indice de la caméra
            int mIndice;
    };
}
#endif
