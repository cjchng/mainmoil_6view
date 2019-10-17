#include "mainwindow.h"
#include "moildevslim.h"
#define MAP_CACHE_ENABLED true

MainWindow::MainWindow()
{

    Moildevslim md ;
    Mat image_input, image_input_s;
    Mat image_display[6];

// repo220_T2
    md.Config("car", 1.4, 1.4,
        1320.0, 1017.0, 1.048,
        2592, 1944, 4.05,
        0, 0, 0, 0, -47.96, 222.86
        );
    double calibrationWidth = md.getImageWidth();
double iCy = md.getiCy();
ConfigData *cd = md.getcd();
    image_input = imread( "image.jpg", IMREAD_COLOR);
    double w = image_input.cols;
    double h = image_input.rows;
    Mat mapX[6], mapY[6];

    mapX[0] = Mat(h, w, CV_32F);
    mapX[1] = Mat(w, h, CV_32F);
    mapX[2] = Mat(w, h, CV_32F);
    mapY[0] = Mat(h, w, CV_32F);
    mapY[1] = Mat(w, h, CV_32F);
    mapY[2] = Mat(w, h, CV_32F);
    for (uint i=3;i<6;i++)
         mapX[i] = Mat(h, w, CV_32F);
    for (uint i=3;i<6;i++)
         mapY[i] = Mat(h, w, CV_32F);

    Mat image_result(h, w, CV_32F);
    Mat image_resultv(w, h, CV_32F);
    double m_ratio = w / calibrationWidth;
    clock_t tStart = clock();
    char str_x[12], str_y[12];
    int i = 0;
if ( MAP_CACHE_ENABLED ) {

bool map_exist = true ;

while (map_exist && (i < 6)) {
    sprintf(str_x, "matX%d", i);sprintf(str_y, "matY%d", i);
    if ( !fopen(str_x, "r") || !fopen(str_y, "r"))
        map_exist = false ;
    i++;
}
if ( map_exist ) {
for (i=0;i<6;i++) {
    sprintf(str_x, "matX%d", i);sprintf(str_y, "matY%d", i);
    mapX[i] = MatRead(str_x);
    mapY[i] = MatRead(str_y);
}
}
else {
    md.AnyPointM((float *)mapX[0].data, (float *)mapY[0].data, mapX[0].cols, mapX[0].rows, 0, 0, 4, m_ratio);       // front view
    md.AnyPointM((float *)mapX[1].data, (float *)mapY[1].data, mapX[1].cols, mapX[1].rows, 70, 270, 4, m_ratio);   // left view, rotate 90
    md.AnyPointM((float *)mapX[2].data, (float *)mapY[2].data, mapX[2].cols, mapX[2].rows, 70, 90, 4, m_ratio);    // right view, rotate -90
    md.AnyPointM((float *)mapX[3].data, (float *)mapY[3].data, mapX[3].cols, mapX[3].rows, -70, 0, 4, m_ratio);      // Down view ( zoom: 2/4 )
    md.AnyPointM((float *)mapX[4].data, (float *)mapY[4].data, mapX[4].cols, mapX[4].rows, 70, 225, 4, m_ratio);   // left-lower view, rotate 180
    md.AnyPointM((float *)mapX[5].data, (float *)mapY[5].data, mapX[5].cols, mapX[5].rows, 70, 135, 4, m_ratio);   // right-lower view, rotate 180
for (i=0;i<6;i++) {
    sprintf(str_x, "matX%d", i);sprintf(str_y, "matY%d", i);
    MatWrite(str_x,mapX[i]);
    MatWrite(str_y,mapY[i]);
}


}

}
else {
    md.AnyPointM((float *)mapX[0].data, (float *)mapY[0].data, mapX[0].cols, mapX[0].rows, 0, 0, 4, m_ratio);       // front view
    md.AnyPointM((float *)mapX[1].data, (float *)mapY[1].data, mapX[1].cols, mapX[1].rows, 70, 270, 4, m_ratio);   // left view, rotate 90
    md.AnyPointM((float *)mapX[2].data, (float *)mapY[2].data, mapX[2].cols, mapX[2].rows, 60, 90, 4, m_ratio);    // right view, rotate -90
    md.AnyPointM((float *)mapX[3].data, (float *)mapY[3].data, mapX[3].cols, mapX[3].rows, -70, 0, 4, m_ratio);      // Down view ( zoom: 2/4 )
    md.AnyPointM((float *)mapX[4].data, (float *)mapY[4].data, mapX[4].cols, mapX[4].rows, 70, 225, 4, m_ratio);   // left-lower view, rotate 180
    md.AnyPointM((float *)mapX[5].data, (float *)mapY[5].data, mapX[5].cols, mapX[5].rows, 70, 135, 4, m_ratio);   // right-lower view, rotate 180
}

    double time_clock = (double)(clock() - tStart)/CLOCKS_PER_SEC ;

    Vec3b p(0,0,0) ;
    image_input.at<Vec3b>(0, 0) = p;
int x_base = 80;
int y_base = 30;

int width_split = (1920-100)/3 ;
int height_split = width_split*3/4 ;

// original image

    cv::resize(image_input, image_input_s, Size(width_split,height_split-y_base));
    imshow("image_input", image_input_s);
    moveWindow("image_input", x_base+width_split, 0+y_base);
    remap(image_input, image_result, mapX[0], mapY[0], INTER_CUBIC, BORDER_CONSTANT, Scalar(0, 0, 0));

    cv::resize(image_result, image_display[0], Size(width_split,height_split-y_base));
    imshow("Front", image_display[0]);
    moveWindow("Front", x_base+width_split, 0+y_base);

    remap(image_input, image_resultv, mapX[1], mapY[1], INTER_CUBIC, BORDER_CONSTANT, Scalar(0, 0, 0));
    Rotate(image_resultv, image_result, 90.0);
    cv::resize(image_result, image_display[1], Size(width_split,height_split-y_base));
    imshow("Left", image_display[1]);
    moveWindow("Left", x_base, 0+y_base);

    remap(image_input, image_resultv, mapX[2], mapY[2], INTER_CUBIC, BORDER_CONSTANT, Scalar(0, 0, 0));
    Rotate(image_resultv, image_result, -90.0);
    cv::resize(image_result, image_display[2], Size(width_split,height_split-y_base));
    imshow("Right", image_display[2]);
    moveWindow("Right", x_base+width_split*2, 0+y_base);

    remap(image_input, image_result, mapX[3], mapY[3], INTER_CUBIC, BORDER_CONSTANT, Scalar(0, 0, 0));
    cv::resize(image_result, image_display[3], Size(width_split,height_split-y_base));
    imshow("Down", image_display[3]);
    moveWindow("Down", x_base+width_split, height_split+y_base);

    remap(image_input, image_result, mapX[4], mapY[4], INTER_CUBIC, BORDER_CONSTANT, Scalar(0, 0, 0));
    Rotate(image_result, image_result, 180.0);
    cv::resize(image_result, image_display[4], Size(width_split,height_split-y_base));
    imshow("Lower left", image_display[4]);
    moveWindow("Lower left", x_base, height_split+y_base);

    remap(image_input, image_result, mapX[5], mapY[5], INTER_CUBIC, BORDER_CONSTANT, Scalar(0, 0, 0));
    Rotate(image_result, image_result, 180.0);
    cv::resize(image_result, image_display[4], Size(width_split,height_split-y_base));
    imshow("Lower right", image_display[4]);
    moveWindow("Lower right", x_base+width_split*2, height_split+y_base);

 while(1){ if(waitKey(100)== 27) break; }

}

void MainWindow::Rotate(Mat& src, Mat& dst, double angle)
{
        Point2f center((src.cols-1)/2.0, (src.rows-1)/2.0);
        Mat rot = cv::getRotationMatrix2D(center, angle, 1.0);
        Rect2f bbox = cv::RotatedRect(cv::Point2f(), src.size(), angle).boundingRect2f();
        rot.at<double>(0,2) += bbox.width/2.0 - src.cols/2.0;
        rot.at<double>(1,2) += bbox.height/2.0 - src.rows/2.0;
        warpAffine(src, dst, rot, bbox.size());
}

void MainWindow::MatWrite(const string& filename, const Mat& mat)
{
    ofstream fs(filename, fstream::binary);

    // Header
    int type = mat.type();
    int channels = mat.channels();
    fs.write((char*)&mat.rows, sizeof(int));    // rows
    fs.write((char*)&mat.cols, sizeof(int));    // cols
    fs.write((char*)&type, sizeof(int));        // type
    fs.write((char*)&channels, sizeof(int));    // channels

    // Data
    if (mat.isContinuous())
    {
        fs.write(mat.ptr<char>(0), (mat.dataend - mat.datastart));
    }
    else
    {
        int rowsz = CV_ELEM_SIZE(type) * mat.cols;
        for (int r = 0; r < mat.rows; ++r)
        {
            fs.write(mat.ptr<char>(r), rowsz);
        }
    }
}

Mat MainWindow::MatRead(const string& filename)
{
    ifstream fs(filename, fstream::binary);

    // Header
    int rows, cols, type, channels;
    fs.read((char*)&rows, sizeof(int));         // rows
    fs.read((char*)&cols, sizeof(int));         // cols
    fs.read((char*)&type, sizeof(int));         // type
    fs.read((char*)&channels, sizeof(int));     // channels

    // Data
    Mat mat(rows, cols, type);
    fs.read((char*)mat.data, CV_ELEM_SIZE(type) * rows * cols);

    return mat;
}


void MainWindow::camButtonClicked()
{

}

void MainWindow::openCamara()
{

}

void MainWindow::readFarme()
{

}

void MainWindow::takingPictures()
{

}


void MainWindow::closeCamara()
{

}


MainWindow::~MainWindow()
{

}
