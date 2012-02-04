/*
 *  Image.h
 *  
 *
 *  Created by P.E. Hladik on 18/01/11.
 *  Copyright 2011 INSA. All rights reserved.
 *  Inspired of code from http://www.siteduzero.com/ by NoHar
 */

#ifndef IMAGE_H
#define IMAGE_H

#include <opencv/cv.h>
#include <time.h>

namespace robotInsa {

    class Image {
    public:

        // Constructeur

        Image(IplImage* img = 0) : mIplimg(img) {
        }

        // Destructeur

        virtual ~Image() {
            mIplimg = 0;
        }

        // Getter

        IplImage* ipl() const {
            return mIplimg;
        }

        long long getDate() const {
            return date;
        }

        // Setter

        void setIpl(IplImage* img) {
            mIplimg = img;
        }

        void setDate(long long time) {
            date = time;
        }

        // Désallocation

        void release() {
            cvReleaseImage(&mIplimg);
        }

    protected:

        // L'IplImage* encapsulé
        IplImage* mIplimg;

        // Date de prise de l'image
        long long date;
    };
}

#endif
