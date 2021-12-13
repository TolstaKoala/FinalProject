#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define PI 3.14159265358979323846

float path(float llat_prev, float llong_prev, float llat_cur, float llong_cur)
{
float rad = 6372795;


float lat1 = llat_prev*PI/180;
float lat2 = llat_cur*PI/180;
float long1 = llong_prev*PI/180;
float long2 = llong_cur*PI/180;
 

float cl1 = cos(lat1);
float cl2 = cos(lat2);
float sl1 = sin(lat1);
float sl2 = sin(lat2);
float delta = long2 - long1;
float cdelta = cos(delta);
float sdelta = sin(delta);
 

float y = sqrt(pow(cl2*sdelta,2) + pow(cl1*sl2-sl1*cl2*cdelta,2));
float x = sl1*sl2+cl1*cl2*cdelta;
float ad = atan2(y,x);
float dist = ad*rad;
float dist1 = 0;
 
/* if(dist != 0)
 {
printf("Distance: ");
printf("%f" , dist);
printf("[meters]\n");
	dist = 0;
	 llat_cur = 0;
 }
 else
 {
llat_prev = llat_cur;
llong_prev = llong_cur;
dist = dist + dist1;
	 
 } */
return dist;
}