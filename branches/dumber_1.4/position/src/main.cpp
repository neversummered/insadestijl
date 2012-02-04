/**********************
 *    http://www.siteduzero.com/tutoriel-3-8565-premiers-pas-avec-opencv.html
 **********************/

//Nous allons utiliser les sous-librairies suivantes :
//"cv.h" pour gérer un objet "image",
//"highgui.h" pour le charger depuis un fichier et l'afficher.

#include <opencv/cv.h>
#include <opencv/cxcore.h>
#include <opencv/highgui.h>
#include "findPosition.h"
#include "show.h"


using namespace std;

int main() {
    char *fileName;

    setImagefactor(2);

    fileName = "/Users/Piro/Documents/Enseignements/robot_INSA/images_test/test_position/Photo3.jpg";

    IplImage *image = cvLoadImage(fileName);

    computeRobotPosition(image);

    CvPoint2D32f position = getPosition();
    CvPoint2D32f orientation = getOrientation();

    float norm = sqrt(pow(orientation.x, 2) + pow(orientation.y, 2));
    CvPoint2D32f zou = cvPoint2D32f(position.x + 20 * (orientation.x / norm),
            position.y + 20 * (orientation.y / norm));
    showLine2D32F(image, position, zou, true);

    /*fileName = "/Users/Piro/Documents/Enseignements/robot_INSA/images_test/robot/Photo20.jpg";
    printf("Photo20\n");
    test(fileName);
    fileName = "/Users/Piro/Documents/Enseignements/robot_INSA/images_test/robot/Photo21.jpg";
    printf("Photo21\n");
    test(fileName);
    fileName = "/Users/Piro/Documents/Enseignements/robot_INSA/images_test/robot/Photo22.jpg";
    printf("Photo22\n");
    test(fileName);
    fileName = "/Users/Piro/Documents/Enseignements/robot_INSA/images_test/robot/Photo23.jpg";
    printf("Photo23\n");
    test(fileName);
    fileName = "/Users/Piro/Documents/Enseignements/robot_INSA/images_test/robot/Photo24.jpg";
    printf("Photo24\n");
    test(fileName);*/


    return 0;

}

