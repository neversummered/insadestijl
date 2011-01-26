/*
 *  position.h
 *  
 *
 *  Created by Pirem on 19/01/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef ANALYSEIMAGE_H
#define ANALYSEIMAGE_H

#include <opencv/cv.h>
#include <opencv/cxcore.h>
#include <opencv/highgui.h>

#include "../video/Image.h"
#include "../data/position.h"

namespace robotInsa {

    class AnalyseImage {
    public:
        Position computeRobotPosition(IplImage *image);
        CvBox2D computeAreaPosition(IplImage *image);

    private:
        /**
         * Load an image
         * @param fileName the name of the source file
         * @param img the pointer to the image
         */
        void loadSrcImage(char *fileName);

        /**
         * Resize an image according to the public ImageFactor
         */
        void resizeWithFactor();

        /**
         * Filter the image to extract the shape of the robot
         */
        void hardFilter();

        /**
         * Find the shape of the robot and compute its gravity center
         */
        void findROI();

        /* Applt the ROi to the source image */
        void applyROI();

        /* Filter the (ROI of) source image */
        void filterROI();

        /* Find the three spots for the robot's position */
        void findPositionPoints();


        /* compute the mass center of the spots  */
        CvPoint2D32f computeTriangleMasscenter(CvPoint2D32f vertex[3]);

        /* Compute tue orientation vector of the robot */
        CvPoint2D32f computeOrientation(CvPoint2D32f vertex[3]);

        /* Free the ROI of the image source and recompute the position
         * in the image source reference */
        void freeROI();

        /* Compute the mass center of a contour in space delimited by a rectangular */
        CvPoint2D32f computeMassCenter(CvSeq *c, CvRect *r);

        float polarConversion(CvPoint2D32f vec);

        void findShapeAreaAroundPoint(CvPoint2D32f pt, CvBox2D *pBox);

    };
}

#endif