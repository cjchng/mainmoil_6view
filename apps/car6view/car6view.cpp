#include "car6view.h"
#define MAP_CACHE_ENABLED false
#define USE_PICAMERA true
Car6view::Car6view(){
    md = new Moildev();
}
void Car6view::Show() {
// repo220_T2

//cout << "test" << endl;
//return ;
    md->Config("rpi_220", 1.4, 1.4,
        1320.0, 1017.0, 1.048,
        2592, 1944, 3.4, // 4.05
        // 0, 0, 0, 0, -47.96, 222.86
        0,0,0,10.11,-85.241,282.21
        );
/*        
    md->Config("t265", 3, 3,
        427, 394, 1,
        848, 800, 1.68,
        0, 0, -24.964, 38.2, -16.956, 183.42
        );        
*/        
    double calibrationWidth = md->getImageWidth();
double iCy = md->getiCy();
ConfigData *cd = md->getcd();
    image_input = imread( "images/image.jpg", IMREAD_COLOR);
    // image_input = imread( "images/T265_01.jpg", IMREAD_COLOR);
    MediaType mediaType = MediaType::IMAGE_FILE;
    double w = image_input.cols;
    double h = image_input.rows;

    mapX[0] = Mat(h, w, CV_32F);
    mapX[1] = Mat(h, w, CV_32F);
    mapX[2] = Mat(h, w, CV_32F);
    mapY[0] = Mat(h, w, CV_32F);
    mapY[1] = Mat(h, w, CV_32F);
    mapY[2] = Mat(h, w, CV_32F);
    for (uint i=3;i<7;i++)
         mapX[i] = Mat(h, w, CV_32F);
    for (uint i=3;i<7;i++)
         mapY[i] = Mat(h, w, CV_32F);

    Mat image_result(h, w, CV_32F);
    Mat image_resultv(h, w, CV_32F);
    m_ratio = w / calibrationWidth;
    clock_t tStart = clock();
    char str_x[12], str_y[12];
    int i = 0;
if ( MAP_CACHE_ENABLED ) {

bool map_exist = true ;

while (map_exist && (i < 7)) {
    sprintf(str_x, "matX%d", i);sprintf(str_y, "matY%d", i);
    if ( !fopen(str_x, "r") || !fopen(str_y, "r"))
        map_exist = false ;
    i++;
}
if ( map_exist ) {
for (i=0;i<7;i++) {
    sprintf(str_x, "matX%d", i);sprintf(str_y, "matY%d", i);
    mapX[i] = MatRead(str_x);
    mapY[i] = MatRead(str_y);
}
}
else {
    md->AnyPointM2((float *)mapX[0].data, (float *)mapY[0].data, mapX[0].cols, mapX[0].rows, 0, 0, 6, m_ratio);       // front view
    md->AnyPointM2((float *)mapX[1].data, (float *)mapY[1].data, mapX[1].cols, mapX[1].rows, 0, -70, 6, m_ratio);   // left view
    md->AnyPointM2((float *)mapX[2].data, (float *)mapY[2].data, mapX[2].cols, mapX[2].rows, 0, 70, 6, m_ratio);    // right view
    md->AnyPointM2((float *)mapX[3].data, (float *)mapY[3].data, mapX[3].cols, mapX[3].rows, -70, 0, 6, m_ratio);      // Down view
    md->AnyPointM2((float *)mapX[4].data, (float *)mapY[4].data, mapX[4].cols, mapX[4].rows, -45, -45, 6, m_ratio);   // left-lower view
    md->AnyPointM2((float *)mapX[5].data, (float *)mapY[5].data, mapX[5].cols, mapX[5].rows, -45, 45, 6, m_ratio);   // right-lower view
    md->PanoramaM((float *)mapX[6].data, (float *)mapY[6].data, mapX[6].cols, mapX[6].rows, m_ratio, 110 );   // panorama
for (i=0;i<7;i++) {
    sprintf(str_x, "matX%d", i);sprintf(str_y, "matY%d", i);
    MatWrite(str_x,mapX[i]);
    MatWrite(str_y,mapY[i]);
}


}

}
else {
    md->AnyPointM2((float *)mapX[0].data, (float *)mapY[0].data, mapX[0].cols, mapX[0].rows, 0, 0, 6, m_ratio);       // front view
    md->AnyPointM2((float *)mapX[1].data, (float *)mapY[1].data, mapX[1].cols, mapX[1].rows, 0, -70, 6, m_ratio);   // left view
    md->AnyPointM2((float *)mapX[2].data, (float *)mapY[2].data, mapX[2].cols, mapX[2].rows, 0, 70, 6, m_ratio);    // right view
    md->AnyPointM2((float *)mapX[3].data, (float *)mapY[3].data, mapX[3].cols, mapX[3].rows, -70, 0, 6, m_ratio);      // Down view
    md->AnyPointM2((float *)mapX[4].data, (float *)mapY[4].data, mapX[4].cols, mapX[4].rows, -45, -45, 6, m_ratio);   // left-lower view
    md->AnyPointM2((float *)mapX[5].data, (float *)mapY[5].data, mapX[5].cols, mapX[5].rows, -45, 45, 6, m_ratio);   // right-lower view
    md->PanoramaM((float *)mapX[6].data, (float *)mapY[6].data, mapX[6].cols, mapX[6].rows, m_ratio, 110);   // panorama
}

    double time_clock = (double)(clock() - tStart)/CLOCKS_PER_SEC ;
cout << "time: " << time_clock << endl ; 
    Vec3b p(0,0,0) ;
    image_input.at<Vec3b>(0, 0) = p;

    DisplayCh(0);
 char c;int cnt;
 while(1){ 
     c = waitKey( 100 );
     if(c == 27) break;
     if(c == 'c') {
            openCamara();
     } 
     }

    cvDestroyWindow("image_input");
    cvDestroyWindow("Front");
    cvDestroyWindow("Left");
    cvDestroyWindow("Right");
    cvDestroyWindow("Down");
    cvDestroyWindow("Lower left");
    cvDestroyWindow("Lower right");     
    image_result.release();
    image_resultv.release();

}
void Car6view::DisplayCh(int ch)
{
Mat image_result, image_resultv;
if( image_input.empty()) return ;
if ( currCh != prevCh ) {
    cvDestroyWindow("image_input");
    cvDestroyWindow("Front");
    cvDestroyWindow("Left");
    cvDestroyWindow("Right");
    cvDestroyWindow("Down");
    cvDestroyWindow("Lower left");
    cvDestroyWindow("Lower right");

}

// original image
    switch (ch) {
    case 0:  // 2 x 3

    cv::resize(image_input, image_input_s, Size(width_split,height_split-y_base));
    if(( mediaType == MediaType::CAMERA ) && USE_PICAMERA)
        cvtColor(image_input_s, image_input_s, CV_BGR2RGB);
    imshow("image_input", image_input_s);
    moveWindow("image_input", x_base+width_split, 0+y_base);

    remap(image_input, image_result, mapX[0], mapY[0], INTER_CUBIC, BORDER_CONSTANT, Scalar(0, 0, 0));
    cv::resize(image_result, image_display[0], Size(width_split,height_split-y_base));
    if(( mediaType == MediaType::CAMERA ) && USE_PICAMERA)
        cvtColor(image_display[0], image_display[0], CV_BGR2RGB);    
    imshow("Front", image_display[0]);
    moveWindow("Front", x_base+width_split, 0+y_base);

    remap(image_input, image_result, mapX[1], mapY[1], INTER_CUBIC, BORDER_CONSTANT, Scalar(0, 0, 0));
    cv::resize(image_result, image_display[1], Size(width_split,height_split-y_base));
    if(( mediaType == MediaType::CAMERA ) && USE_PICAMERA)
        cvtColor(image_display[1], image_display[1], CV_BGR2RGB);       
    imshow("Left", image_display[1]);
    moveWindow("Left", x_base, 0+y_base);

    remap(image_input, image_result, mapX[2], mapY[2], INTER_CUBIC, BORDER_CONSTANT, Scalar(0, 0, 0));
    cv::resize(image_result, image_display[2], Size(width_split,height_split-y_base));
    if(( mediaType == MediaType::CAMERA ) && USE_PICAMERA)
        cvtColor(image_display[2], image_display[2], CV_BGR2RGB);       
    imshow("Right", image_display[2]);
    moveWindow("Right", x_base+width_split*2, 0+y_base);

    remap(image_input, image_result, mapX[3], mapY[3], INTER_CUBIC, BORDER_CONSTANT, Scalar(0, 0, 0));
    cv::resize(image_result, image_display[3], Size(width_split,height_split-y_base));
    if(( mediaType == MediaType::CAMERA ) && USE_PICAMERA)
        cvtColor(image_display[3], image_display[3], CV_BGR2RGB);   
    imshow("Down", image_display[3]);
    moveWindow("Down", x_base+width_split, height_split+y_base);

    remap(image_input, image_result, mapX[4], mapY[4], INTER_CUBIC, BORDER_CONSTANT, Scalar(0, 0, 0));
    cv::resize(image_result, image_display[4], Size(width_split,height_split-y_base));
    if(( mediaType == MediaType::CAMERA ) && USE_PICAMERA)
        cvtColor(image_display[4], image_display[4], CV_BGR2RGB);       
    imshow("Lower left", image_display[4]);
    moveWindow("Lower left", x_base, height_split+y_base);

    remap(image_input, image_result, mapX[5], mapY[5], INTER_CUBIC, BORDER_CONSTANT, Scalar(0, 0, 0));    
    cv::resize(image_result, image_display[5], Size(width_split,height_split-y_base));
    if(( mediaType == MediaType::CAMERA ) && USE_PICAMERA)
        cvtColor(image_display[5], image_display[5], CV_BGR2RGB);       
    imshow("Lower right", image_display[5]);
    moveWindow("Lower right", x_base+width_split*2, height_split+y_base);
        break;
    case 1:
    cv::resize(image_input, image_input_s, Size(width_split*3,height_split*2));
    if(( mediaType == MediaType::CAMERA ) && USE_PICAMERA)
        cvtColor(image_input_s, image_input_s, CV_BGR2RGB);
    imshow("image_input", image_input_s);
    moveWindow("image_input", x_base, y_base);    
    break;
    case 2:
    remap(image_input, image_result, mapX[0], mapY[0], INTER_CUBIC, BORDER_CONSTANT, Scalar(0, 0, 0));
    cv::resize(image_result, image_display[0], Size(width_split*3,height_split*2));
    if(( mediaType == MediaType::CAMERA ) && USE_PICAMERA)
        cvtColor(image_display[0], image_display[0], CV_BGR2RGB);    
    imshow("Front", image_display[0]);
    moveWindow("Front", x_base, y_base);    
    break;
    case 3:
    remap(image_input, image_result, mapX[1], mapY[1], INTER_CUBIC, BORDER_CONSTANT, Scalar(0, 0, 0));
    cv::resize(image_result, image_display[1], Size(width_split*3,height_split*2));
    if(( mediaType == MediaType::CAMERA ) && USE_PICAMERA)
        cvtColor(image_display[1], image_display[1], CV_BGR2RGB);       
    imshow("Left", image_display[1]);
    moveWindow("Left", x_base, y_base);    
    break;
    case 4:
    remap(image_input, image_result, mapX[2], mapY[2], INTER_CUBIC, BORDER_CONSTANT, Scalar(0, 0, 0));    
    cv::resize(image_result, image_display[2], Size(width_split*3,height_split*2));
    if(( mediaType == MediaType::CAMERA ) && USE_PICAMERA)
        cvtColor(image_display[2], image_display[2], CV_BGR2RGB);       
    imshow("Right", image_display[2]);
    moveWindow("Right", x_base, y_base);    
    break;
    case 5:
    remap(image_input, image_result, mapX[3], mapY[3], INTER_CUBIC, BORDER_CONSTANT, Scalar(0, 0, 0));
    cv::resize(image_result, image_display[3], Size(width_split*3,height_split*2));
    if(( mediaType == MediaType::CAMERA ) && USE_PICAMERA)
        cvtColor(image_display[3], image_display[3], CV_BGR2RGB);   
    imshow("Down", image_display[3]);
    moveWindow("Down", x_base, y_base);
    break;
    case 6:
    remap(image_input, image_result, mapX[4], mapY[4], INTER_CUBIC, BORDER_CONSTANT, Scalar(0, 0, 0));
    cv::resize(image_result, image_display[4], Size(width_split*3,height_split*2));
    if(( mediaType == MediaType::CAMERA ) && USE_PICAMERA)
        cvtColor(image_display[4], image_display[4], CV_BGR2RGB);       
    imshow("Lower left", image_display[4]);
    moveWindow("Lower left", x_base, y_base);    
    break;
    case 7:
    remap(image_input, image_result, mapX[5], mapY[5], INTER_CUBIC, BORDER_CONSTANT, Scalar(0, 0, 0));    
    cv::resize(image_result, image_display[5], Size(width_split*3,height_split*2));
    if(( mediaType == MediaType::CAMERA ) && USE_PICAMERA)
        cvtColor(image_display[5], image_display[5], CV_BGR2RGB);       
    imshow("Lower right", image_display[5]);
    moveWindow("Lower right", x_base, y_base);    
    break;
    case 8:
    remap(image_input, image_result, mapX[6], mapY[6], INTER_CUBIC, BORDER_CONSTANT, Scalar(0, 0, 0));
    cv::resize(image_result, image_display[6], Size(width_split*3,height_split*2));
    if(( mediaType == MediaType::CAMERA ) && USE_PICAMERA)
        cvtColor(image_display[6], image_display[6], CV_BGR2RGB);    
    imshow("Panorama", image_display[6]);
    moveWindow("Panorama", x_base, y_base);   
    break;    
    case -1:
    cout << "test" << endl;
    break; 
    }
    prevCh = currCh ; 
}



void Car6view::Rotate(Mat& src, Mat& dst, double angle)
{
        Point2f center((src.cols-1)/2.0, (src.rows-1)/2.0);
        Mat rot = cv::getRotationMatrix2D(center, angle, 1.0);
        Rect2f bbox = cv::RotatedRect(cv::Point2f(), src.size(), angle).boundingRect2f();
        rot.at<double>(0,2) += bbox.width/2.0 - src.cols/2.0;
        rot.at<double>(1,2) += bbox.height/2.0 - src.rows/2.0;
        warpAffine(src, dst, rot, bbox.size());
}

void Car6view::MatWrite(const string& filename, const Mat& mat)
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

Mat Car6view::MatRead(const string& filename)
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


void Car6view::camButtonClicked()
{

}

void Car6view::openCamara()
{
    char c;
    cap0.open(0);
    cap0.set(CV_CAP_PROP_FRAME_WIDTH,2592);
    cap0.set(CV_CAP_PROP_FRAME_HEIGHT,1944); 

    if ( cap0.isOpened() ) {   
        mediaType = MediaType::CAMERA;
        currCh = 0; 
    for(;;)
    {
    // Mat frame;
    cap0 >> image_input;
    if (( image_input.cols != fix_width ) || ( image_input.rows != fix_height ))
    cv::resize(image_input, image_input, Size(fix_width, fix_height));


          if( image_input.empty() ) break; // end of video stream
          DisplayCh(currCh);
          c = waitKey(33);
          // if (c!=-1) cout << "(" << (int)c << ")";
          
          if( c == 27 ) break; // stop capturing by pressing ESC 
          else if ((c >= '0' ) && (c <='8')) { 
              currCh = (int)c -int('0');             
          }
          else if (((int)c == 82) && (currCh == 2)) { // up
     if ( currAlpha + currInc <= 90 )
        currAlpha = currAlpha + currInc ;
    else
        currAlpha = 90 ;   
        md->fastAnyPointM((float *)mapX[0].data, (float *)mapY[0].data, mapX[0].cols, mapX[0].rows, currAlpha, currBeta, currZoom, m_ratio);       // front view      
          }
          else if (((int)c == 84) && (currCh == 2)) { // Down
    if ( currAlpha - currInc >= -90 )
        currAlpha = currAlpha - currInc ;
    else
        currAlpha = -90 ;          
        md->fastAnyPointM((float *)mapX[0].data, (float *)mapY[0].data, mapX[0].cols, mapX[0].rows, currAlpha, currBeta, currZoom, m_ratio);       // front view      
          }
          else if (((int)c == 81) && (currCh == 2)) { // left
    if( currBeta - currInc >= 0 )
        currBeta = currBeta - currInc ;
    else
        currBeta = ( currBeta - currInc ) + 360 ;          
        md->fastAnyPointM((float *)mapX[0].data, (float *)mapY[0].data, mapX[0].cols, mapX[0].rows, currAlpha, currBeta, currZoom, m_ratio);       // front view      
          }
          else if (((int)c == 83) && (currCh == 2)) { // right
    currBeta = ( currBeta + currInc ) % 360 ;
    if (currBeta < 0) currBeta += 360;          
    md->fastAnyPointM((float *)mapX[0].data, (float *)mapY[0].data, mapX[0].cols, mapX[0].rows, currAlpha, currBeta, currZoom, m_ratio);       // front view      
          }
          else if (((int)c == 43) && (currCh == 2)) { // + 
    currZoom += 1;          
    if (currZoom > maxZoom) currZoom = maxZoom;              
    md->fastAnyPointM((float *)mapX[0].data, (float *)mapY[0].data, mapX[0].cols, mapX[0].rows, currAlpha, currBeta, currZoom, m_ratio);       // front view      
          }          
          else if (((int)c == 45) && (currCh == 2)) { // - 
    currZoom -= 1;          
    if (currZoom < minZoom) currZoom = minZoom;              
    md->fastAnyPointM((float *)mapX[0].data, (float *)mapY[0].data, mapX[0].cols, mapX[0].rows, currAlpha, currBeta, currZoom, m_ratio);       // front view      
          }   
          else if (((c == 'r')||(c == 'R')) && (currCh == 2)) { // R : Reset 
    currAlpha = 0 ; 
    currBeta = 0;      
    currZoom = defaultZoom;                        
    md->fastAnyPointM((float *)mapX[0].data, (float *)mapY[0].data, mapX[0].cols, mapX[0].rows, currAlpha, currBeta, currZoom, m_ratio);       // front view      
          }             

    }
    mediaType = MediaType::NONE;
    }
}

void Car6view::readFarme()
{

}

void Car6view::takingPictures()
{

}


void Car6view::closeCamara()
{

}

void Car6view::freeMemory()
{
    cout << "free memory" << endl;
    image_input.release();
    image_input_s.release();
for(int i=0;i<7;i++) {
    image_display[i].release();
    mapX[i].release();
    mapY[i].release();
}
}
Car6view::~Car6view()
{
    freeMemory();
    delete md;
}

