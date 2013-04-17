/* 
 * File:    main.cpp
 * Author:  Piro
 *
 * Created on 6 décembre 2011, 07: 47
 */


#include <stdlib.h> 
#include "../headers/dincludes.h"

#define NBE 100

double moyenne(long long mesures[NBE]);
double ecart_type(long long mesures[NBE], double moyenne);
double min(long long mesures[NBE]);
double max(long long mesures[NBE]);

double moyenne(long long mesures[NBE]) {
    int i;
    long long somme = 0;
    for (i = 0; i < NBE; i++) {
        somme += mesures[i];
    }
    return (double) somme / (double) NBE;
}

double ecart_type(long long mesures[NBE], double moyenne) {
    long long somme = 0;
    int i;
    for (i=0; i<NBE; i++){
        somme += pow((double)mesures[i] - moyenne,2.0);
    }
    return (double) somme / (double) NBE;
}

double min(long long mesures[NBE]) {
	long long m = 1000000000000000000.0;
    int i;
	for (i=0; i< NBE; i++){
        if (mesures[i] < m) m = mesures[i]; 
    }
    return (double) m;
}

double max(long long mesures[NBE]) {
	long long m = 0;
    int i;
	for (i=0; i<NBE; i++){
        if (mesures[i] > m) m = mesures[i]; 
    }
    return (double) m;
}

int main(int argc, char** argv) {
	
    long long t1;
    long long t2;
    long long mesures[NBE];
    double moy, e, mi, ma;
	
	int real = 0;
	
	DCamera* camera;
    DImage* img = d_new_image();
    int i;
	
    DArena* arena = NULL;
	
	if (!real) { //Utilisation d'une image fixe
		printf("Chargement image\n");
		IplImage* imgSrc = cvLoadImage("Photo1c.jpg", CV_LOAD_IMAGE_COLOR);
		d_image_set_ipl(img, imgSrc);
		cvReleaseImage(&imgSrc);
	} else {
		printf("ouverture camera\n");
		camera = d_new_camera();
		d_camera_open(camera);
	}
	
	
	/*****************************
	 Temps d'acquistion d'une image
	 d_camera_get_frame
 	*****************************/
	if (real) {
		printf("Evalution du temps pour d_camera_get_frame\n");
		for (i = 0; i < NBE; i++) {
			t1 = d_tools_get_time_in_millisec();
			d_camera_get_frame(camera, img);
			t2 = d_tools_get_time_in_millisec();
			mesures[i] = t2 - t1;
		}
		moy = moyenne(mesures);
		e = ecart_type(mesures, moy);
		mi = min(mesures);
		ma = max(mesures);
		printf(">  Temps moyen: %f ecart type: %f min: %f max: %f\n", moy, e, mi, ma);
	}    
	
	/*****************************
	 Temps pour le calcul de la position du robot avec arene
	 d_image_compute_arena_position
	 *****************************/
	printf("Evalution du temps pour d_image_compute_arena_position\n");
    for (i = 0; i < NBE; i++) {
        if (real) d_camera_get_frame(camera, img);
        t1 = d_tools_get_time_in_millisec();
        d_image_compute_arena_position(img);
        t2 = d_tools_get_time_in_millisec();
        mesures[i] = t2 - t1;
    }
    moy = moyenne(mesures);
    e = ecart_type(mesures, moy);
	mi = min(mesures);
	ma = max(mesures);
	printf(">  Temps moyen: %f ecart type: %f min: %f max: %f\n", moy, e, mi, ma);
	
	
	/*****************************
	 Temps pour le calcul de la position du robot sans arene
	 d_image_compute_robot_position
	 *****************************/
	printf("Evalution du temps pour d_image_compute_robot_position sans arene\n");
	arena = NULL;
    for (i = 0; i < NBE; i++) {
        if (real) d_camera_get_frame(camera, img);
        t1 = d_tools_get_time_in_millisec();
        d_image_compute_robot_position(img, arena);
        t2 = d_tools_get_time_in_millisec();
        mesures[i] = t2 - t1;
    }
    moy = moyenne(mesures);
    e = ecart_type(mesures, moy);
	mi = min(mesures);
	ma = max(mesures);
	printf(">  Temps moyen: %f ecart type: %f min: %f max: %f\n", moy, e, mi, ma);

	/*****************************
	 Temps pour le calcul de la position du robot avec arene
	 d_image_compute_robot_position
	 *****************************/
	printf("Evalution du temps pour d_image_compute_robot_position avec arene\n");
	if (real) d_camera_get_frame(camera, img);
	i = 0;
	while (arena == NULL) {
		arena = d_image_compute_arena_position(img);
		i++;
		if (i > 10) exit(EXIT_FAILURE);
	}
	
    for (i = 0; i < NBE; i++) {
        if (real) d_camera_get_frame(camera, img);
        t1 = d_tools_get_time_in_millisec();
        d_image_compute_robot_position(img, arena);
        t2 = d_tools_get_time_in_millisec();
        mesures[i] = t2 - t1;
    }
    moy = moyenne(mesures);
    e = ecart_type(mesures, moy);
	mi = min(mesures);
	ma = max(mesures);
	printf(">  Temps moyen: %f ecart type: %f min: %f max: %f\n", moy, e, mi, ma);
    
	
	/*****************************
	 Temps pour compresser une image
	 d_jpegimage_compress
	 *****************************/
	printf("Evalution du temps pour d_jpegimage_compress\n");
    DJpegimage* jpeg = d_new_jpegimage();
    for (i = 0; i < NBE; i++) {
        if (real) d_camera_get_frame(camera, img);
        t1 = d_tools_get_time_in_millisec();
        d_jpegimage_compress(jpeg, img);
        t2 = d_tools_get_time_in_millisec();
        mesures[i] = t2 - t1;
    }
    moy = moyenne(mesures);
    e = ecart_type(mesures, moy);
	mi = min(mesures);
	ma = max(mesures);
	printf(">  Temps moyen: %f ecart type: %f min: %f max: %f\n", moy, e, mi, ma);
	    
    return 0;
}

