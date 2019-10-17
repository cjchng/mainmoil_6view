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
    void camButtonClicked();

    void openCamara();
    void readFarme();
    void closeCamara();
    void takingPictures();


    bool CaptureState = false;


    void Rotate(Mat& src, Mat& dst, double angle);
    void MatWrite(const string& filename, const Mat& mat);
    Mat MatRead(const string& filename);

};

#endif // MAINWINDOW_H
