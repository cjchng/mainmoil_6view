#ifndef MEASURE_H
#define MEASURE_H
#include <stdio.h>
#include <string.h>
#include <opencv2/opencv.hpp>
#include <sstream>
#include <string>
#include <time.h>
#include <iostream>
#include <fstream>
#include <termios.h>  
#include <unistd.h> 

#include "../../moildev.h"
using namespace std;
using namespace cv;

namespace Ui {
class Measure;
}

class Measure 
{

public:
    Measure();
    void Show();
    ~Measure();


private:

    Moildev *md ;
    Mat image_input;
    Mat frame0, frame;

    //Mat image_display[7];
    //Mat mapX[7], mapY[7];
    double m_ratio;
    int x_base = 0;
    int y_base = 200;
    int fix_width = 2592;
    int fix_height = 1944;
    int currCh = 0, prevCh = 0;
    int leftBlank = 68 ; 
    const int leftBaseAlpha = 0, leftBaseBeta = 0 ;
    const int rightBaseAlpha = 0, rightBaseBeta = 0 ;
//Socinext
//    int cameraLeftCx = 334, cameraLeftCy = 198;
//    int cameraRightCx = 310, cameraRightCy = 244;
//Raspberry
    int cameraLeftCx = 319, cameraLeftCy = 241;
    int cameraRightCx = 319, cameraRightCy = 241;

    enum class MediaType {NONE, IMAGE_FILE, CAMERA };
    MediaType mediaType = MediaType::NONE;

    enum class SystemState { NONE, CAMERA_LEFT, CAMERA_RIGHT, SET_POINT, CALCULATE, EXIT, ERROR };
    SystemState state = SystemState::NONE;

    enum class CameraPos { LEFT, RIGHT };
    SystemState cameraPos ; 

    void openCamara();

    VideoCapture cap0;

    bool CaptureState = false;

    void DisplayCh(int Ch);
    void freeMemory();
    double getAlpha(int x, int y, CameraPos camera);
    double getBeta(int x, int y, CameraPos camera);
    Point3d calc3DPoint(double alpha1, double beta1, double alpha2, double beta2);
    double calcDistance(Point lp1, Point lp2, Point rp1, Point rp2);
};

#endif 
