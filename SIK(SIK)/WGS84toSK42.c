#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define PI 3.14159265358979323846

double* llh2ecef(const double* llh);
double* lla_to_ecef(const double* llh);
double* ecef2llh(const double *coord_xyz);
double* xyzPZ90_to_xyzSK42(const double* xyzPZ90);
double* xyzWSG84_to_xyzPZ90(const double* xyzWSG84);
void print(const double* arr);

int WGS84toSK42(float llat_cur ,float llong_cur, float height_cur, char *outData0x01)
{
	  //int l = 15;
	  //*(outData0x01 + l);
    double true_llh[] = {llat_cur*PI/180, llong_cur*PI/180, height_cur}; 
    double *xyzWSG84 = llh2ecef(true_llh); //?????????? ? WGS-84
    double *xyzPZ90 = xyzWSG84_to_xyzPZ90(xyzWSG84); //??????? ????????? WGS-84 - PZ-90
    double *xyzSK42 = xyzPZ90_to_xyzSK42(xyzPZ90); //??????? ????????? PZ-90 - ??-42
    //print(true_llh);
    //print(xyzWSG84);
    //print(xyzPZ90);
    //print(xyzSK42);
			*(outData0x01 + 15) = (int)(xyzPZ90[3]*1e+2);
			*(outData0x01 + 19) = (int)(xyzPZ90[2]*1e+2);
			*(outData0x01 + 23) = (int)(xyzPZ90[1]*1e+2);
		  *(outData0x01 + 27) = (int)(xyzSK42[3]*1e+2);
		  *(outData0x01 + 31) = (int)(xyzSK42[2]*1e+2);
		  *(outData0x01 + 35) = (int)(xyzSK42[1]*1e+2);
	  
    free(xyzWSG84);
    free(xyzPZ90);
    free(xyzSK42);

    return 0;
}

double* llh2ecef(const double* llh)
{
    const int    A = 6378137;       // Earth's radius (m) per WGS84, CB
    const double E = 8.18191908e-2; // Eccentricity per WGS84, CB

    double ESQ = E * E;
    double SP = sin(llh[0]);
    double CP = cos(llh[0]);
    double SL = sin(llh[1]);
    double CL = cos(llh[1]);
    double GSQ = 1.0 - (ESQ*SP*SP);
    double EN = A / sqrt(GSQ);
    double Z = (EN + llh[2])*CP;
    const double ecef0 = Z * CL;
    const double ecef1 = Z * SL;
    EN = EN - (ESQ * EN);
    const double ecef2 = (EN + llh[2])*SP;
    //static double ecef[] = {ecef0, ecef1, ecef2};



    double *ecef = (double*)malloc(sizeof(double) * 3);
    *(ecef + 0) = ecef0;
    *(ecef + 1) = ecef1;
    *(ecef + 2) = ecef2;
    

    return ecef;
}
double* xyzWSG84_to_xyzPZ90(const double *xyzWSG84)
{
    double M = 1.0 + 0.12 * pow(10, -6);
    double N[3][3] = { {                1.0, 0.9696*pow(10, -6), 0.0},
                       {-0.9696*pow(10, -6),                1.0, 0.0},
                       {                0.0,                0.0, 1.0}};
    double K[3] = {-1.1, -0.3, -0.9};
    //static double xyzPZ90[3]{};
    double* xyzPZ90 = (double*)malloc(sizeof(double) * 3);

    for (int i = 0; i < 3; i++){
        for (int j = 0; j < 3; j++)
            xyzPZ90[i] += N[i][j]*xyzWSG84[j];
        xyzPZ90[i] *= M;
        xyzPZ90[i] -= K[i];
    }
    //xyzPZ90[3] = 8539458395893 * xyzPZ90[3];
    return xyzPZ90;
}
double* xyzPZ90_to_xyzSK42(const double *xyzPZ90)
{
    double N[3][3] = {{                  1.0, 3.1998 * pow(10, -6), -1.6968 * pow(10, -6)},
                      {-3.1998 * pow(10, -6),                  1.0,                  0.0},
                      { 1.6968 * pow(10, -6),                  0.0,                  1.0}};
    float K[3] = {25.0, -141.0, -80.0};
    //static double xyzSK42[3]{0};
    double* xyzSK42 = (double*)malloc(sizeof(double) * 3);
    for (int i = 0; i < 3; i++){
        for (int j = 0; j < 3; j++)
            xyzSK42[i] += N[i][j]*xyzPZ90[j];
        xyzSK42[i] -= K[i];
    }
    return xyzSK42;
}
double* lla_to_ecef(const double* llh)
{
    double lat = llh[0];
    double lon = llh[1];
    double alt = llh[2];
    // see http://www.mathworks.de/help/toolbox/aeroblks/llatoecefposition.html
    const double Rad = 6378137.0;
    const double   F = 1.0 / 298.257223563;
    double cosLat = cos(lat);
    double sinLat = sin(lat);
    double     FF = (1.0 - F) * (1.0 - F);
    double      C = 1.0 / sqrt(cosLat*cosLat + FF * sinLat * sinLat);
    // double      S = C * FF; ????? ?? ????????????

    static double coord_xyz[3];
    coord_xyz[0] = (Rad * C + alt) * cosLat * cos(lon);
    coord_xyz[1] = (Rad * C + alt) * cosLat * sin(lon);
    coord_xyz[2] = (Rad * C + alt) * sinLat;

    return coord_xyz;
}

double* ecef2llh(const double *coord_xyz)
{
    const double   F = 1.0 / 298.257223563;

    double esq = F * (2 - F);
    const double A = 6378137;
    double x = coord_xyz[0];
    double y = coord_xyz[1];
    double z = coord_xyz[2];
    double rsq = (x * x) + (y * y);
    double   h = esq * z;
    double p, zp, r, en;
    for (int i = 0; i < 6; i++)
    {
        zp = z + h;
        r = sqrt(rsq + (zp * zp));
        double sp = zp/r;
        double gsq = 1.0 - (esq * sp * sp);
        en = A / sqrt(gsq);
        p = en * esq * sp;
        h = p;
    }

    p = atan2(zp, sqrt(rsq));
    h = r - en;
    static double wgs[3];
    wgs[0] = p * 180 / PI;
    wgs[1] = atan2(y, x) * 180 / PI;
    wgs[2] = h;

    return wgs;
}
void print(const double* arr)
{
    //printf("%f %f %f\n", arr[0], arr[1], arr[2]);
}
