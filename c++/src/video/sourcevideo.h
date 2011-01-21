/*
 * sourceVideo.h
 *
 *  Created on: 18 janv. 2011
 *      Author: P.E. Hladik
 *  Inspired of code from http://www.siteduzero.com/ by NoHar
 */

#ifndef SOURCEVIDEO_H
#define SOURCEVIDEO_H

#include <string>
#include "image.h"

namespace robotInsa
{

    class SourceVideo
    {
        public:

            // Destructeur
            virtual ~SourceVideo()
            {}

            // Récupération d'infos
            virtual std::string getInfos()
            {
                return mInfos;
            }

            // Ouverture du flux
            virtual void open() = 0;

            // Fermeture du flux
            virtual void close() = 0;

            // Récupération d'une frame
            virtual void getFrame( Image &frame ) = 0;

        protected:

            // Informations sur le flux
            std::string mInfos;
    };
}
#endif
