/*
 *  Image.h
 *  
 *
 *  Created by P.E. Hladik on 18/01/11.
 *  Copyright 2011 INSA. All rights reserved.
 *  Inspired of code from http://www.siteduzero.com/ by NoHar
 */

#ifndef JPEGIMAGE_H
#define JPEGIMAGE_H

#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <time.h>

#include "Image.h"

namespace robotInsa {

    class JpegImage {
    public:

        // Constructeur

        JpegImage(Image img) {
            if (img.ipl() == 0) {
                jpegImg = 0;
                date = 0;
            } else {
                int jpeg_params[] = {CV_IMWRITE_JPEG_QUALITY, 80, 0};
                jpegImg = cvEncodeImage(".jpeg", img.ipl(), jpeg_params);
                date = img.getDate();
            }
        }

        // Destructeur

        virtual ~JpegImage() {
            jpegImg = 0;
        }


        // Setter

        void setJpegImage(Image img) {
            int jpeg_params[] = {CV_IMWRITE_JPEG_QUALITY, 80, 0};
            jpegImg = cvEncodeImage(".jpeg", img.ipl(), jpeg_params);
            date = img.getDate();

        }

        void setJpegImage(IplImage* img) {
            int jpeg_params[] = {CV_IMWRITE_JPEG_QUALITY, 80, 0};
            jpegImg = cvEncodeImage(".jpeg", img, jpeg_params);

        }

        void setDate(long long time) {
            date = time;
        }

        // Getter

        CvMat* getJpegImg() const {
            return jpegImg;
        }

        long long getDate() const {
            return date;
        }


        // Désallocation

        void release() {
            cvReleaseMat(&jpegImg);
        }

    protected:

        // L'IplImage* encapsulé
        CvMat* jpegImg;

        // Date de prise de l'image
        long long date;
    };
}

#endif
