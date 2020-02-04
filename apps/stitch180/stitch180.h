#ifndef STITCH180_H
#define STITCH180_H
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
class Stitch180;
}

class Stitch180 
{

public:
    Stitch180();
    void Show();
    ~Stitch180();


private:

    Moildev *md ;
    Mat image_input, image_input_s, image_pano;
    Mat mapX_pano, mapY_pano;
    Mat frame0, frame;
    double m_ratio;
    int x_base = 0;
    int y_base = 180;
    int fix_width = 2592;
    int fix_height = 1944;
    const std::string videoStreamAddress = "http://192.168.100.8:8000/stream.mjpg";
    enum class MediaType {NONE, IMAGE_FILE, CAMERA };
    MediaType mediaType = MediaType::NONE;

    enum class SystemState { NONE, CAMERA, STITCH, EXIT, ERROR };
    SystemState state = SystemState::NONE;

    enum class CameraPos { LEFT, RIGHT };
    SystemState cameraPos ; 

    void openCamara();

    VideoCapture cap0;

    bool CaptureState = false;
    void MatWrite(const string& filename, const Mat& mat);
    Mat MatRead(const string& filename);
    void DisplayCh();
    void freeMemory();

};

#endif 
