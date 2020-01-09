#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <stdio.h>
#include <string.h>
#include <opencv2/opencv.hpp>
#include <sstream>
#include <string>
#include <time.h>
#include <iostream>
#include <fstream>

using namespace std;
using namespace cv;

namespace Ui {
class MainWindow;
}

class MainWindow 
{

public:
    MainWindow();
    ~MainWindow();


private:


    Mat image_input, image_input_s;
    Mat image_display[6];
    Mat mapX[6], mapY[6];

    int x_base = 80;
    int y_base = 30;
    int fix_width = 2592;
    int fix_height = 1944;

    int width_split = (1920-100)/3 ;
    int height_split = width_split*3/4 ;

    void camButtonClicked();

    void openCamara();
    void readFarme();
    void closeCamara();
    void takingPictures();
    VideoCapture cap0;

    bool CaptureState = false;

    void DisplayCh(int Ch);
    void Rotate(Mat& src, Mat& dst, double angle);
    void MatWrite(const string& filename, const Mat& mat);
    Mat MatRead(const string& filename);

};

#endif // MAINWINDOW_H
