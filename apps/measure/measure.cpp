#include "measure.h"
#define CVUI_IMPLEMENTATION
#include "cvui.h"
#define WINDOW_NAME "Measure"
#define USE_PICAMERA true
Measure::Measure()
{
    md = new Moildev();
}
void Measure::Show()
{

    cvui::init(WINDOW_NAME);

    frame0 = Mat(cv::Size(1800, 1000), CV_8UC3);
    frame;

    fix_width = frame0.cols / 2;
    fix_height = fix_width * 3 / 4;
    leftBlank = 68;

    bool checked = false;
    bool checked2 = true;
    int count = 0;
    double countFloat = 0.0;
    double trackbarValue = 0.0;
    int LPt_Ns = 0, RPt_Ns = 0;
    Point lpa[2], rpa[2];
    double Result;
    // repo220_T2
    md->Config("measure", 1.4, 1.4,
               1320.0, 1017.0, 1.048,
               2592, 1944, 4.05,
               0, 0, 0, 0, -47.96, 222.86);
    double calibrationWidth = md->getImageWidth();
    double iCy = md->getiCy();
    ConfigData *cd = md->getcd();
    // image_input = imread( "images/left.jpg", IMREAD_COLOR);
    frame0 = cv::Scalar(49, 52, 49);
    image_input = imread("images/left.jpg");
    cv::resize(image_input, image_input, Size(fix_width, fix_height), 0, 0, INTER_LINEAR);
    image_input.copyTo(frame0(Rect(0, y_base, image_input.cols, image_input.rows)));

    image_input = imread("images/right.jpg");
    cv::resize(image_input, image_input, Size(fix_width, fix_height), 0, 0, INTER_LINEAR);
    image_input.copyTo(frame0(Rect(image_input.cols, y_base, image_input.cols, image_input.rows)));

    SystemState systemState = SystemState::NONE;

    // int state = 0 ;

    while (true)
    {

        if ((mediaType == MediaType::CAMERA) && cap0.isOpened() && (state == SystemState::CAMERA_LEFT || state == SystemState::CAMERA_RIGHT))
        {
            cap0 >> image_input;
            if ((image_input.cols != fix_width) || (image_input.rows != fix_height))
                cv::resize(image_input, image_input, Size(fix_width, fix_height));
            if (image_input.empty())
            {
                mediaType = MediaType::NONE;
                state = SystemState::NONE;
            }
            DisplayCh(currCh);
        }

        frame0.copyTo(frame);
        cvui::text(frame, 10, 20, "MOIL Measure");
        int posX = 100;
        if (cvui::button(frame, posX, 50, 100, 100, "Left Camera"))
        {
            if (state != SystemState::CAMERA_LEFT)
            {
                state = SystemState::CAMERA_LEFT;
                currCh = 0;
                if (!cap0.isOpened())
                    openCamara();
            }
            else
            {
                cap0.release();
                state = SystemState::NONE;
            }
        }
        if (cvui::button(frame, posX + 110, 50, 100, 100, "Right Camera"))
        {
            if (state != SystemState::CAMERA_RIGHT)
            {
                state = SystemState::CAMERA_RIGHT;
                currCh = 1;
                if (!cap0.isOpened())
                    openCamara();
            }
            else
            {
                cap0.release();
                state = SystemState::NONE;
            }
        }

        if (cvui::button(frame, posX + 110 * 2, 50, 100, 100, "Calculate"))
        {
            if (LPt_Ns == 2 && RPt_Ns == 2)
            {
                double ratio = (double)640 / fix_width;
                Point lpa_0(lpa[0].x * ratio, lpa[0].y * ratio);
                Point lpa_1(lpa[1].x * ratio, lpa[1].y * ratio);

                Point rpa_0((rpa[0].x - fix_width) * ratio, rpa[0].y * ratio);
                Point rpa_1((rpa[1].x - fix_width) * ratio, rpa[1].y * ratio);
                Result = calcDistance(lpa_0, lpa_1, rpa_0, rpa_1);
                state = SystemState::CALCULATE;
            }
            else
            {
                state = SystemState::ERROR;
            }
        }
        if (cvui::button(frame, posX + 110 * 3, 50, 100, 100, "Reset"))
        {
            LPt_Ns = 0;
            RPt_Ns = 0;
            Result = 0;
            state = SystemState::NONE;
        }
        if (cvui::button(frame, posX + 110 * 5, 50, 100, 100, "Exit"))
        {
            state = SystemState::EXIT;
        }
        if (state == SystemState::CAMERA_LEFT)
            cvui::rect(frame, posX, 50, 100, 100, 0xff0000);
        else if (state == SystemState::CAMERA_RIGHT)
            cvui::rect(frame, posX + 110, 50, 100, 100, 0xff0000);
        else if (state == SystemState::CALCULATE)
        {
            cvui::rect(frame, posX + 110 * 2, 50, 100, 100, 0xff0000);
            char buff[100];
            if (Result > 0)
            {
                snprintf(buff, sizeof(buff), "%.2f", Result);
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

        if (cvui::mouse(cvui::DOWN))
        {
            // std::cout << "mouse down." << std::endl;
            cv::Point cursor = cvui::mouse();
            std::cout << "x: " << cursor.x << " y: " << cursor.y << std::endl;
            if (cursor.x <= fix_width && cursor.y > 200)
            {
                if (LPt_Ns < 2)
                {
                    lpa[LPt_Ns].x = cursor.x;
                    lpa[LPt_Ns].y = cursor.y;
                    LPt_Ns++;
                }
            }
            else if (cursor.x <= (fix_width * 2) && cursor.y > 200)
            {
                if (RPt_Ns < 2)
                {
                    rpa[RPt_Ns].x = cursor.x;
                    rpa[RPt_Ns].y = cursor.y;
                    RPt_Ns++;
                }
            }
        }
        if (LPt_Ns == 2)
        {
            line(frame, lpa[0], lpa[1], cv::Scalar(0, 200, 200), 3, 0, 0);
        }
        if (RPt_Ns == 2)
        {
            line(frame, rpa[0], rpa[1], cv::Scalar(0, 200, 200), 3, 0, 0);
        }
        for (int i = 0; i < LPt_Ns; i++)
        {
            circle(frame, lpa[i], 5, (0, 255, 255), 3);
        }
        for (int i = 0; i < RPt_Ns; i++)
        {
            circle(frame, rpa[i], 5, (0, 255, 255), 3);
        }

        cvui::update();

        // Show window content
        cv::imshow(WINDOW_NAME, frame);

        if ((cv::waitKey(20) == 27) || (state == SystemState::EXIT))
        {
            break;
        }
    }
}
void Measure::DisplayCh(int ch) // 0: left, 1: right
{
    if (image_input.empty())
        return;
    switch (ch)
    {
    case 0:
        if ((mediaType == MediaType::CAMERA) && USE_PICAMERA)
            cvtColor(image_input, image_input, CV_BGR2RGB);
        image_input.copyTo(frame0(Rect(0, y_base, image_input.cols, image_input.rows)));
        break;
    case 1:
        if ((mediaType == MediaType::CAMERA) && USE_PICAMERA)
            cvtColor(image_input, image_input, CV_BGR2RGB);
        image_input.copyTo(frame0(Rect(image_input.cols, y_base, image_input.cols, image_input.rows)));
        break;
    }
    prevCh = currCh;
}

void Measure::openCamara()
{
    char c;
    cap0.open(0);
    cap0.set(CV_CAP_PROP_FRAME_WIDTH, 2592);
    cap0.set(CV_CAP_PROP_FRAME_HEIGHT, 1944);

    if (cap0.isOpened())
    {
        mediaType = MediaType::CAMERA;
    }
}

void Measure::freeMemory()
{
    cout << "free memory" << endl;
    image_input.release();
}
Measure::~Measure()
{
    freeMemory();
    delete md;
}

double Measure::getAlpha(int x, int y, CameraPos camera)
{
    int Cx, Cy;
    if (camera == CameraPos::LEFT) // left
    {
        Cx = cameraLeftCx;
        Cy = cameraLeftCy;
    }
    else
    {
        Cx = cameraRightCx;
        Cy = cameraRightCy;
    }

    double iv = sqrt(pow(x - Cx, 2) + pow(y - Cy, 2));
    //Socinext
    //if (isLeftCam)
    //    degrees = 0.000000000216580 * pow(iv, 4) - 0.000000057193714 * pow(iv, 3) + 0.000001587496967 * pow(iv, 2) + 0.006682956310241 * pow(iv, 1) * 180.0 / PI;
    //else
    //    degrees = 0.000000000201028 * pow(iv, 4) - 0.000000053087877 * pow(iv, 3) + 0.000001569554981 * pow(iv, 2) + 0.006690458193267 * pow(iv, 1) * 180.0 / PI;
    //Raspberry Pi
    double alpha = 0.00000000097184699590 * pow(iv, 4) - 0.00000025039358456450 * pow(iv, 3) + 0.00008360116571815010 * pow(iv, 2) + 0.25007057891514300000 * pow(iv, 1);
    return alpha;
}

double Measure::getBeta(int x, int y, CameraPos camera)
{
    int x1, y1, x2, y2, x3, y3;
    if (camera == CameraPos::LEFT) // left
    {
        x1 = cameraLeftCx;
        y1 = cameraLeftCy;
    }
    else
    {
        x1 = cameraRightCx;
        y1 = cameraRightCy;
    }
    x2 = x;
    y2 = y;
    x3 = leftBlank; // 68 for 640x480
    y3 = y1;

    double side1 = sqrt((x1 - x3) * (x1 - x3) + (y1 - y3) * (y1 - y3));
    double side2 = sqrt((x2 - x3) * (x2 - x3) + (y2 - y3) * (y2 - y3));
    double side3 = sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
    double beta = acos((side1 * side1 + side3 * side3 - side2 * side2) / (2.0 * side1 * side3));
    beta = beta * 180 / PI;
    if (y1 - y2)
    {
        beta = 360 - beta;
    }
    beta += 270;
    beta = (int)beta % 360;
    return beta;
}

Point3d Measure::calc3DPoint(double alpha1, double beta1, double alpha2, double beta2)
{
    Point3d sourcePoint1(0, 0, 0);
    Point3d sourcePoint2(6.6, 0, 0);

    alpha1 += leftBaseAlpha;
    beta1 += leftBaseBeta;

    alpha2 += rightBaseAlpha;
    beta2 += rightBaseBeta;

    beta1 = 450 - beta1;
    beta2 = 450 - beta2;
    if (beta1 >= 360)
        beta1 -= 360;
    if (beta2 >= 360)
        beta2 -= 360;

    Point3d point(0, 0, 0);
    double degreeToRad = PI / 180;
    double x1Vector = sin(alpha1 * degreeToRad) * cos(beta1 * degreeToRad);
    double y1Vector = sin(alpha1 * degreeToRad) * sin(beta1 * degreeToRad);
    double z1Vector = cos(alpha1 * degreeToRad);
    double s1x = x1Vector;
    double s1y = y1Vector;
    double s1z = z1Vector;
    double s1xc = -sourcePoint1.x;
    double s1yc = -sourcePoint1.y;
    double s1zc = -sourcePoint1.z;

    double x2Vector = sin(alpha2 * degreeToRad) * cos(beta2 * degreeToRad);
    double y2Vector = sin(alpha2 * degreeToRad) * sin(beta2 * degreeToRad);
    double z2Vector = cos(alpha2 * degreeToRad);
    double t1x = x2Vector;
    double t1y = y2Vector;
    double t1z = z2Vector;
    double t1xc = -sourcePoint2.x;
    double t1yc = -sourcePoint2.y;
    double t1zc = -sourcePoint2.z;

    double xc = -s1xc + t1xc;
    double yc = -s1yc + t1yc;
    double zc = -s1zc + t1zc;

    double sTop = -s1x * x1Vector + (-s1y * y1Vector) + (-s1z * z1Vector);
    double sButton = -s1x * x2Vector + (-s1y * y2Vector) + (-s1z * z2Vector);
    double tTop = -(t1x * x1Vector + t1y * y1Vector + t1z * z1Vector);
    double tButton = -(t1x * x2Vector + t1y * y2Vector + t1z * z2Vector);
    double cTop = -(xc * x1Vector + yc * y1Vector + zc * z1Vector);
    double cButton = -(xc * x2Vector + yc * y2Vector + zc * z2Vector);
    if (sTop != 1)
    {
        tTop /= sTop;
        cTop /= sTop;
        sTop = 1;
    }
    if (sButton != 1)
    {
        tButton /= sButton;
        cButton /= sButton;
        sButton = 1;
    }
    tTop = tButton - tTop;
    double t = cTop - cButton;
    t = t / tTop;
    double s = tButton * t + cButton;

    double px = s1x * s + s1xc;
    double py = s1y * s + s1yc;
    double pz = s1z * s + s1zc;
    double qx = t1x * t + t1xc;
    double qy = t1y * t + t1yc;
    double qz = t1z * t + t1zc;

    double midPointX = ((px + qx) / 2);
    double midPointY = ((py + qy) / 2);
    double midPointZ = ((pz + qz) / 2);

    point.x = midPointX;
    point.y = midPointY;
    point.z = midPointZ;

    return point;
}

double Measure::calcDistance(Point lp1, Point lp2, Point rp1, Point rp2)
{
    double lalpha1 = getAlpha(lp1.x, lp1.y, CameraPos::LEFT);
    double lbeta1 = getBeta(lp1.x, lp1.y, CameraPos::LEFT);
    double lalpha2 = getAlpha(lp2.x, lp2.y, CameraPos::LEFT);
    double lbeta2 = getBeta(lp2.x, lp2.y, CameraPos::LEFT);

    double ralpha1 = getAlpha(rp1.x, rp1.y, CameraPos::RIGHT);
    double rbeta1 = getBeta(rp1.x, rp1.y, CameraPos::RIGHT);
    double ralpha2 = getAlpha(rp2.x, rp2.y, CameraPos::RIGHT);
    double rbeta2 = getBeta(rp2.x, rp2.y, CameraPos::RIGHT);

    Point3d p1 = calc3DPoint(lalpha1, lbeta1, ralpha1, rbeta1);
    Point3d p2 = calc3DPoint(lalpha2, lbeta2, ralpha2, rbeta2);

    double dis = sqrt(pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2) + pow(p1.z - p2.z, 2));
    return dis;
}