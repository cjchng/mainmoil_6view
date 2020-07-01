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

#include "moildev.h"
using namespace std;
using namespace cv;

namespace Ui
{
    class Stitch180;
}

class Stitch180
{

public:
    Stitch180();
    void Show();
    ~Stitch180();

private:
    Moildev *md;
    Mat image_input, image_input_s, image_pano, image_result, image_result_s;
    Mat mapX_pano, mapY_pano;
    Mat frame0, frame;
    Mat image0;
    double m_ratio;
    int stitchCnt = 0;
    int stitchCnt_Max = 50;
    int stitchedLength = 0;
    int x_base = 0;
    int y_base = 180;
    int fix_width = 2592;
    int fix_height = 1944;
    // int stitch_width = 640;
    // int stitch_total = 1280;
    int stitch_curr = 0;
    int stitch_elapsedTime = 0;
    clock_t tStart, tStart_overall;
    double time_clock;
    const std::string videoStreamAddress = "http://192.168.100.6:8000/stream.mjpg";
    enum class MediaType
    {
        NONE,
        IMAGE_FILE,
        CAMERA
    };
    MediaType mediaType = MediaType::NONE;

    enum class SystemState
    {
        NONE,
        CAMERA,
        STITCH,
        EXIT,
        ERROR
    };
    SystemState state = SystemState::NONE;

    enum class StitchState
    {
        OFF,
        ON
    };
    StitchState stitchState = StitchState::OFF;

    enum class RecordState
    {
        OFF,
        ON
    };
    RecordState recordState = RecordState::OFF;

    enum class CameraPos
    {
        LEFT,
        RIGHT
    };
    SystemState cameraPos;

    void openCamara();

    VideoCapture cap0;
    VideoWriter videoWriter;
    bool CaptureState = false;

    int doStitch(Mat &image_src, Mat &image_result, int alpha_all, int alpha_start, int alpha_width, int stitch_offset);
    void MatWrite(const string &filename, const Mat &mat);
    Mat MatRead(const string &filename);
    void DisplayCh();
    void saveImage(Mat img);
    void freeMemory();
};

#endif
