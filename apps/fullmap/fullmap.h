#ifndef FULLMAP_H
#define FULLMAP_H
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

#include "moildev.h"
using namespace std;
using namespace cv;

namespace Ui
{
    class FullMap;
}

class FullMap
{

public:
    FullMap();
    void Show();
    ~FullMap();

private:
    Moildev *md;
    Mat image_input, image_input_s;
    Mat image_display[7];
    Mat fmapX, fmapY;
    Mat mapX[7], mapY[7];
    double m_ratio;
    int x_base = 80;
    int y_base = 30;
    // rpi_220
    int fix_width = 2592;
    int fix_height = 1944;
    int alpha_max = 110;
    int ResMultiplier = 10;
    // T265
    //  int fix_width = 848;
    //  int fix_height = 800;
    int currCh = 0, prevCh = 0;
    int currPara = 0;
    int currAlpha = 0;
    int currBeta = 0;
    int currInc = 10;
    double currZoom = 4;
    double defaultZoom = 4;

    const double minZoom = 1;
    const double maxZoom = 12;
    int width_split = (1920 - 100) / 3;
    int height_split = width_split * 3 / 4;

    enum class MediaType
    {
        NONE,
        IMAGE_FILE,
        CAMERA,
        VIDEO_FILE
    };
    MediaType mediaType = MediaType::NONE;

    void camButtonClicked();

    void openCamara();
    void doAnyPoint();
    void readFarme();
    void closeCamara();
    void takingPictures();
    VideoCapture cap0;

    bool CaptureState = false;

    void DisplayCh(int Ch);
    void Rotate(Mat &src, Mat &dst, double angle);
    void MatWrite(const string &filename, const Mat &mat);
    Mat MatRead(const string &filename);
    void freeMemory();
};

#endif
