#include "stitch180.h"
#include "cvui.h"
#define MAP_CACHE_ENABLED false
#define WINDOW_NAME "Stitch180"
#define USE_IP_CAMERA true
#define USE_PICAMERA false
#define STITCH_CUT_DEGREE   5 
#define STITCH_DISPLAY_X    900
#define STITCH_INTERVAL     CLOCKS_PER_SEC * 0.5 
#define STITCH_WIDTH        800
#define STITCH_MAX_LENGTH   2000
Stitch180::Stitch180()
{
    md = new Moildev();
}
void Stitch180::Show()
{

    cvui::init(WINDOW_NAME);

    frame0 = Mat(cv::Size(1920, 1080), CV_8UC3);
    frame;

    //fix_width = frame0.cols / 2;
    //fix_height = fix_width * 3 / 4;
    //leftBlank = 68 * fix_width / 640;

    bool checked = false;
    bool checked2 = true;
    int count = 0;
    double countFloat = 0.0;
    double trackbarValue = 0.0;
    double Result;
    // repo220_T2stitch_width
    //           2592, 1944, 4.05,
    //           0, 0, 0, 0, -47.96, 222.86);
    md->Config("measure", 1.4, 1.4,
               1298.0, 966.0, 1, // 1.048,
               2592, 1944, 3.4, //4.05,
               // 0, 0, 0, 0, -47.96, 222.86
               0,0,0,10.11,-85.241,282.21
               );               
    double calibrationWidth = md->getImageWidth();
    double iCy = md->getiCy();
    ConfigData *cd = md->getcd();
    // image_input = Mat(fix_height, fix_width, CV_32F);
    image_pano = Mat(fix_height, fix_width, CV_32F);
    image_result = Mat(700, STITCH_WIDTH, CV_8UC3);
    image_result_s = Mat(700, STITCH_WIDTH, CV_8UC3);
    // image_result_s = Mat(100, 100, CV_8UC3);
    image_result = cv::Scalar(0, 0, 0);
    image_result_s = cv::Scalar(0, 0, 0);

    mapX_pano = Mat(fix_height, fix_width, CV_32F);
    mapY_pano = Mat(fix_height, fix_width, CV_32F);

    m_ratio = fix_width / calibrationWidth;
    clock_t tStart = clock();

    int i = 0;
if ( MAP_CACHE_ENABLED ) {

bool map_exist = true ;

if ( map_exist ) {
    mapX_pano = MatRead("matX_pano");
    mapY_pano = MatRead("matY_pano");
}
else {
    md->PanoramaM((float *)mapX_pano.data, (float *)mapY_pano.data, mapX_pano.cols, mapX_pano.rows, m_ratio, 110 );   // panorama
    MatWrite("matX_pano",mapX_pano);
    MatWrite("matY_pano",mapY_pano);
}

}
else {
    md->PanoramaM((float *)mapX_pano.data, (float *)mapY_pano.data, mapX_pano.cols, mapX_pano.rows, m_ratio, 110);   // panorama
}

    frame0 = cv::Scalar(49, 52, 49);
    SystemState systemState = SystemState::NONE;
    while (true)
    {

        if ((mediaType == MediaType::CAMERA) && cap0.isOpened() && (state == SystemState::CAMERA ))
        {
            cap0 >> image_input;

            if (recordState == RecordState::ON)          
                videoWriter.write(image_input);
                         
            if (image_input.empty())
            {
                mediaType = MediaType::NONE;
                state = SystemState::NONE;
            }
            else {

            if ((image_input.cols != fix_width) || (image_input.rows != fix_height))
                cv::resize(image_input, image_input, Size(fix_width, fix_height));   
            cv::resize(image_input, image_input_s, Size(400, 300));                    
            remap(image_input, image_pano, mapX_pano, mapY_pano, INTER_CUBIC, BORDER_CONSTANT, Scalar(0, 0, 0));
            cv::resize(image_pano, image_pano , Size(STITCH_WIDTH, STITCH_WIDTH/2));
            if(!image_result.empty())
               cv::resize(image_result, image_result_s , Size( 800, image_result.rows * 800 / image_result.cols));
            DisplayCh();
            }
        }

 if (!image_result_s.empty())
         image_result_s.copyTo(frame0(Rect(STITCH_DISPLAY_X, y_base, image_result_s.cols, image_result_s.rows)));
        
        frame0.copyTo(frame);
        cvui::text(frame, 10, 20, "MOIL Stitch180");
        int posX = 100;
        if (cvui::button(frame, posX, 50, 100, 100, "Camera"))
        {
            if (state != SystemState::CAMERA)
            {
                state = SystemState::CAMERA;
                if (!cap0.isOpened())
                    openCamara();
            }
            else
            {
                cap0.release();
                state = SystemState::NONE;
            }
        }
        if (cvui::button(frame, posX + 110 * 2, 50, 100, 100, "Stitch"))
        {
            if (state == SystemState::CAMERA && stitchState == StitchState::OFF) {
            stitchState = StitchState::ON;
            stitchCnt = 0;
            stitchedLength = 0 ;
            image_result = cv::Scalar(0, 0, 0);
            image_result_s = cv::Scalar(0, 0, 0);              
            tStart_overall = clock();
            tStart = clock();
            }
            else
            {
                stitchState = StitchState::OFF;
            }
            
        }
        if (cvui::button(frame, posX + 110 * 3, 50, 100, 100, "Stop"))
        {
            if ( stitchState == StitchState::ON ){
                stitchState = StitchState::OFF;
                saveImage(image_result);
            }
        }
        if (cvui::button(frame, posX + 110 * 4, 50, 100, 100, "Clear"))
        {
            if ( stitchState == StitchState::OFF ){
            stitchCnt = 0;
            stitchedLength = 0 ;   
            image_result = cv::Scalar(0, 0, 0);
            image_result_s = cv::Scalar(0, 0, 0);                                       
            }


        }        
        if (cvui::button(frame, posX + 110 * 6, 50, 100, 100, "Exit"))
        {
            state = SystemState::EXIT;
        }
        if (state == SystemState::CAMERA)
        {
            cvui::rect(frame, posX, 50, 100, 100, 0xff0000);
        
          if (stitchState == StitchState::ON)
          {
            cvui::rect(frame, posX + 110 * 2, 50, 100, 100, 0xff0000);
            char buff[100];
            time_clock = (double)(clock() - tStart_overall)/ CLOCKS_PER_SEC ;
            if( clock() - tStart >= STITCH_INTERVAL )
            {       
                int len = doStitch(image_pano, image_result, 110, 88, STITCH_CUT_DEGREE, stitchedLength);
                stitchedLength += len;
                if ((image_result.cols > STITCH_MAX_LENGTH)||( stitchCnt > stitchCnt_Max )|| (len == 0) ) {
                    saveImage(image_result);
                    stitchState = StitchState::OFF;
                }
                else
                stitchCnt += 1 ;
                tStart = clock();
            }
                snprintf(buff, sizeof(buff), "%.2f", time_clock);
                std::string distanceStr = buff;
                cv::putText(frame, distanceStr, cv::Point(1000, 120), cv::FONT_HERSHEY_COMPLEX, 2, cv::Scalar(0, 255, 255), 2, 8, 0);                      
          }
        
        }
        else if (state == SystemState::ERROR)
        {
            cv::putText(frame, "Error", cv::Point(1000, 120), cv::FONT_HERSHEY_COMPLEX, 2, cv::Scalar(0, 255, 255), 2, 8, 0);
        }
        else if (state == SystemState::EXIT)
            cvui::rect(frame, posX + 110 * 5, 50, 100, 100, 0xff0000);

        cvui::update();

        if (recordState == RecordState::ON){
            cv::putText(frame, "REC", cv::Point(1000, 120), cv::FONT_HERSHEY_COMPLEX, 2, cv::Scalar(0, 0, 200), 2, 8, 0);
        } 


        // Show window content
        cv::imshow(WINDOW_NAME, frame);
        char c = cv::waitKey(20); 
        if (c == 'r')
        {
            if (recordState == RecordState::OFF) {
                recordState = RecordState::ON;

                // videoWriter.open("capture.avi", CV_FOURCC('M','J','P','G'),10, Size(frame_width, frame_height));
                videoWriter.open("capture.mp4", CV_FOURCC('D','I','V','3'), 10, Size(fix_width, fix_height));

            }
            else {
                recordState = RecordState::OFF;
                videoWriter.release();
            }
        }

        if ((c == 27) || (state == SystemState::EXIT))
        {
            break;
        }
    }
}

int Stitch180::doStitch(Mat &image_src, Mat &image_result, int alpha_all, int alpha_start, int alpha_width, int stitch_offset)
{
// 0,309,800,36
image0 = image_src(Rect(0,image_src.rows*alpha_start/alpha_all,image_src.cols,image_src.rows*alpha_width/alpha_all));
int h = image0.rows * image_result.cols / image0.cols ;
cv::resize(image0, image0, Size(image_result.cols, h ), 0, 0, INTER_LINEAR);

// copyMakeBorder( image_result, image_result, image_src.rows, 0, 0, 0, BORDER_CONSTANT, Scalar( 0, 0, 0 ) );
if( image_result.rows >= ( stitch_offset + image0.rows )){
    image0.copyTo(image_result( Rect(0, image_result.rows - stitch_offset - image0.rows, image0.cols, image0.rows)));
    return image0.rows;
}
else 
    return 0;


}

void Stitch180::MatWrite(const string& filename, const Mat& mat)
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

Mat Stitch180::MatRead(const string& filename)
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

void Stitch180::DisplayCh() 
{
    if (image_input.empty())
        return;

        if ((mediaType == MediaType::CAMERA) && USE_PICAMERA) {
            cvtColor(image_input_s, image_input_s, CV_BGR2RGB);
            cvtColor(image_pano, image_pano, CV_BGR2RGB);
        }
        image_input_s.copyTo(frame0(Rect(200, y_base, image_input_s.cols, image_input_s.rows)));
        image_pano.copyTo(frame0(Rect(0, y_base+310, image_pano.cols, image_pano.rows)));
}

void Stitch180::openCamara()
{
    char c;
    if ( USE_IP_CAMERA )
        cap0.open(videoStreamAddress);
    else
        cap0.open(0);
    
    cap0.set(CV_CAP_PROP_FRAME_WIDTH, 2592);
    cap0.set(CV_CAP_PROP_FRAME_HEIGHT, 1944);

    if (cap0.isOpened())
    {
        mediaType = MediaType::CAMERA;
    }
}

void Stitch180::saveImage(Mat img)
{
 imwrite( "stitchResult.jpg", img );
}

void Stitch180::freeMemory()
{
    cout << "free memory" << endl;
    image_input.release();
}
Stitch180::~Stitch180()
{
    freeMemory();
    delete md;
}

