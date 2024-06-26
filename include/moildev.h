#ifndef MOILDEV_H
#define MOILDEV_H

#include <stdio.h>
#include <string.h>
#include <iostream>
#include <math.h>
#include <vector>
#include "configdata.h"

#define PI 3.1415926
#define PI_2 1.5707963
#define PCT_UNIT_WIDTH 1.27
#define PCT_UNIT_HEIGHT 1.27
#define FOCAL_LENGTH_FOR_ZOOM 250
#define APPROX_FACT 8

using namespace std;

class Moildev
{
public:
    class ConfigData *cfg;
    Moildev();
    bool fastMode = false;
    bool Config(string cameraName, double cameraSensorWidth, double cameraSensorHeight,
                double iCx, double iCy, double i_ratio,
                double imageWidth, double imageHeight, double calibrationRatio,
                double para0, double para1, double para2, double para3, double para4, double para5);
    double getImageHeight();
    double getImageWidth();
    double getiCx();
    double getiCy();
    ConfigData *getcd();

    double AnyPointM(float *mapX, float *mapY, int w, int h, double alphaOffset, double betaOffset, double zoom, double magnification);
    double AnyPointM2(float *mapX, float *mapY, int w, int h, double thetaX_degree, double thetaY_degree, double zoom, double magnification);
    double fastAnyPointM(float *mapX, float *mapY, int w, int h, double alphaOffset, double betaOffset, double zoom, double magnification);
    double fastAnyPointM2(float *mapX, float *mapY, int w, int h, double thetaX_degree, double thetaY_degree, double zoom, double magnification);
    double ABmap(float *fmapX, float *fmapY, int w, int h, int ResMultiplier, double magnification, double alpha_max);
    double AnyPointM_ABmap(float *mapX, float *mapY, int w, int h, float *ABmapX, float *ABmapY, double alphaOffset, double betaOffset, double zoom, double ResMultiplier, double magnification, double alpha_max);
    double AnyPointM2_ABmap(float *mapX, float *mapY, int w, int h, float *ABmapX, float *ABmapY, double thetaX_degree, double thetaY_degree, double zoom, double ResMultiplier, double magnification, double alpha_max);    
    double NormFisheyeMap(float *mapX, float *mapY, int w, int h, double magnification, double alpha_max);    
    double AnyPointM_NF(float *mapX, float *mapY, int w, int h, double alphaOffset, double betaOffset, double zoom, double alpha_max );
    double AnyPointM2_NF(float *mapX, float *mapY, int w, int h, double thetaX_degree, double thetaY_degree, double zoom, double alpha_max );    
    double PanoramaM(float *mapX, float *mapY, int w, int h, double magnification, double alpha_max);
    double PanoramaM_Rt(float *mapX, float *mapY, int w, int h, double magnification, double alpha_max, double iC_alpha_degree, double iC_beta_degree);
    int getRhoFromAlpha(double alpha);
    double getAlphaFromRho(int rho);
    vector<int> getAlphaBetaFromPos(int Mode, vector<int> Pos);

private:
    double sinArray[90 * APPROX_FACT + 1];
    void initSin();
    double ApproxSin(double angle);
    double ApproxCos(double angle);

    float ApproxAtan(float z);
    float ApproxAtan2(float y, float x);
};

#endif
