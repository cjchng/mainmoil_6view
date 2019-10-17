#include "moildevslim.h"

using namespace std;

Moildevslim::Moildevslim()
{
configData = new ConfigData();
}


bool Moildevslim::Config(string cameraName, double cameraSensorWidth, double cameraSensorHeight,
    double iCx, double iCy, double i_ratio,
    double imageWidth, double imageHeight, double calibrationRatio,
    double para0, double para1, double para2, double para3, double para4, double para5
    )
{
    configData->setCameraName( cameraName );
    configData->setCameraSensorWidth( cameraSensorWidth );
    configData->setCameraSensorHeight( cameraSensorHeight );
    configData->setIcx( iCx );
    configData->setIcy( iCy );
    configData->setRatio( i_ratio );
    configData->setImageWidth( imageWidth );
    configData->setImageHeight( imageHeight );
    configData->setCalibrationRatio( calibrationRatio );
    configData->setParameter0( para0 );
    configData->setParameter1( para1 );
    configData->setParameter2( para2 );
    configData->setParameter3( para3 );
    configData->setParameter4( para4 );
    configData->setParameter5( para5 );
    return true;
}

double Moildevslim::getImageWidth() {
    return configData->getImageWidth();
}
double Moildevslim::getImageHeight() {
    return configData->getImageHeight();
}
double Moildevslim::getiCy() {
    return configData->getIcy();
}
ConfigData* Moildevslim::getcd() {
    return configData;
}


double Moildevslim::AnyPointM(float *mapX, float *mapY, int w, int h, double alphaOffset, double betaOffset, double zoom, double magnification)
{

    double iCx = configData->getIcx();
    double iCy = configData->getIcy();
    double dCx = configData->getImageWidth()/2;
    double dCy = configData->getImageHeight()/2;
    double ratio = configData->getRatio();
    double parameter[6];
    parameter[0] = configData->getParameter0();
    parameter[1] = configData->getParameter1();
    parameter[2] = configData->getParameter2();
    parameter[3] = configData->getParameter3();
    parameter[4] = configData->getParameter4();
    parameter[5] = configData->getParameter5();
    double calibrationRatio = configData->getCalibrationRatio();
    double cameraSensorWidth = configData->getCameraSensorWidth();
    double cameraSensorHeight = configData->getCameraSensorHeight();

    double icx = iCx* magnification;
    double icy = iCy * magnification;
    double dcx = dCx * magnification;
    double dcy = dCy * magnification;
              /* if (alphaOffset >= 50)
               {
                   dcy = dcy - (alphaOffset - 50) * 19;
                   if (dcy < 0)
                       dcy = 0;
               }*/
              betaOffset += 180;
              double mAlphaOffset = alphaOffset * (PI / 180);
              double mBetaOffset = betaOffset * (PI / 180);
              double senH, senV, tempX, tempY, tempZ, beta, alpha;

              // int rows = h;
              // int cols = w;
              double originalPostionX, originalPostionY;
              int positionX, positionY;
              double widthCosB = PCT_UNIT_WIDTH * cos(mBetaOffset);
              double heightCosASinB = PCT_UNIT_HEIGHT * cos(mAlphaOffset) * sin(mBetaOffset);
              double flZoomSinASinB = FOCAL_LENGTH_FOR_ZOOM * zoom * sin(mAlphaOffset) * sin(mBetaOffset);
              double widthSinB = PCT_UNIT_WIDTH * sin(mBetaOffset);
              double heightCosACosB = PCT_UNIT_HEIGHT * cos(mAlphaOffset) * cos(mBetaOffset);
              double flZoomSinACosB = FOCAL_LENGTH_FOR_ZOOM * zoom * sin(mAlphaOffset) * cos(mBetaOffset);
              double heightSinA = PCT_UNIT_HEIGHT * sin(mAlphaOffset);
              double flZoomCosA = FOCAL_LENGTH_FOR_ZOOM * zoom * cos(mAlphaOffset);
              int mem_inc = 0;
              for (positionY = 0; positionY < h; positionY++)
              {
                  for (positionX = 0; positionX < w; positionX++)
                  {

                      tempX = (positionX - dcx) * widthCosB - (positionY - dcy) * heightCosASinB + flZoomSinASinB;

                      tempY = (positionX - dcx) * widthSinB + (positionY - dcy) * heightCosACosB - flZoomSinACosB;

                      tempZ = (positionY - dcy) * heightSinA + flZoomCosA;

                      alpha = atan2(sqrt(tempX * tempX + tempY * tempY), tempZ);
                      if (alpha <= 1.919862)
                      {
                          if (tempX != 0)
                              beta = atan2(tempY, tempX);
                          else
                              if (tempY >= 0)
                              beta = PI / 2;
                          else
                              beta = -(PI / 2);


                          senH = icx * cameraSensorWidth * ratio -
                             (
                              parameter[0] * alpha * alpha * alpha * alpha * alpha * alpha
                              + parameter[1] * alpha * alpha * alpha * alpha * alpha
                              + parameter[2] * alpha * alpha * alpha * alpha
                              + parameter[3] * alpha * alpha * alpha
                              + parameter[4] * alpha * alpha
                              + parameter[5] * alpha) * calibrationRatio * cameraSensorHeight * magnification
                              * cos(beta);

                          senV = icy * cameraSensorHeight -
                              (
                              parameter[0] * alpha * alpha * alpha * alpha * alpha * alpha
                              + parameter[1] * alpha * alpha * alpha * alpha * alpha
                              + parameter[2] * alpha * alpha * alpha * alpha
                              + parameter[3] * alpha * alpha * alpha
                              + parameter[4] * alpha * alpha
                              + parameter[5] * alpha) * calibrationRatio * cameraSensorHeight * magnification
                              * sin(beta);
                          originalPostionX = round(senH / (cameraSensorWidth * ratio));
                          originalPostionY = round(senV / cameraSensorHeight);
                          if (originalPostionX >= 0 && originalPostionX < w && originalPostionY >= 0 && originalPostionY < h) {
                              *(mapX + ( positionY * w + positionX )) = (float)originalPostionX;
                              *(mapY + ( positionY * w + positionX )) = (float)originalPostionY;
                              // *(mapX + ( mem_inc )) = (float)originalPostionX;
                              // *(mapY + ( mem_inc++ )) = (float)originalPostionY;
                          }
                          else
                          {
                              *(mapX + ( positionY * w + positionX )) = 0;
                              *(mapY + ( positionY * w + positionX )) = 0;
                              // *(mapX + ( mem_inc )) = 0;
                              // *(mapY + ( mem_inc++ )) = 0;
                          }

                      }
                  }
              }

              return 0;

}


double Moildevslim::PanoramaM(float *mapX, float *mapY, int w, int h, double magnification )
        {
    double iCx = configData->getIcx();
    double iCy = configData->getIcy();
    double dcx = configData->getImageWidth() / 2 * magnification;
    double dcy = configData->getImageHeight() / 2 * magnification;
    int height = (int)configData->getImageHeight();
    int width = (int)configData->getImageWidth();
    double ratio = configData->getRatio();
    double parameter[6];
    parameter[0] = configData->getParameter0();
    parameter[1] = configData->getParameter1();
    parameter[2] = configData->getParameter2();
    parameter[3] = configData->getParameter3();
    parameter[4] = configData->getParameter4();
    parameter[5] = configData->getParameter5();
    double calibrationRatio = configData->getCalibrationRatio();
    double cameraSensorWidth = configData->getCameraSensorWidth();
    double cameraSensorHeight = configData->getCameraSensorHeight();

            double icx = iCx * magnification;
            double icy = iCy * magnification;
            double alpha, beta, senH, senV;
            int rows = h;
            int cols = w;
            int positionX, positionY;
            double originalPostionX, originalPostionY;

            int mem_inc = 0;
            for (positionY = 0; positionY < rows; positionY++)
            {
                alpha = (double)positionY / (double)rows * PI / 2;
                for (positionX = 0; positionX < cols; positionX++)
                {
                    beta = (2 * PI * (double)positionX / (double)cols);
                    senH = icx * cameraSensorWidth * ratio -
                       (
                            parameter[0] * alpha * alpha * alpha * alpha * alpha * alpha
                            + parameter[1] * alpha * alpha * alpha * alpha * alpha
                            + parameter[2] * alpha * alpha * alpha * alpha
                            + parameter[3] * alpha * alpha * alpha
                            + parameter[4] * alpha * alpha
                            + parameter[5] * alpha)
                            * calibrationRatio * cameraSensorHeight * magnification * cos(beta);
                    senV = icy * cameraSensorHeight -
                      (
                            parameter[0] * alpha * alpha * alpha * alpha * alpha * alpha
                            + parameter[1] * alpha * alpha * alpha * alpha * alpha
                            + parameter[2] * alpha * alpha * alpha * alpha
                            + parameter[3] * alpha * alpha * alpha
                            + parameter[4] * alpha * alpha
                            + parameter[5] * alpha)
                            * calibrationRatio * cameraSensorHeight * magnification * sin(beta);

                    originalPostionX = round(senH / (cameraSensorWidth * ratio));
                    originalPostionY = round(senV / cameraSensorHeight);
                    if (originalPostionX >= 0 && originalPostionX < cols && originalPostionY >= 0 && originalPostionY < rows)
                    {
                        *(mapX + ( mem_inc )) = (float)originalPostionX;
                        *(mapY + ( mem_inc++ )) = (float)originalPostionY;
                    }
                    else
                    {
                        *(mapX + ( mem_inc )) = 0;
                        *(mapY + ( mem_inc++ )) = 0;
                    }
                }
            }

            return 0;
        }





