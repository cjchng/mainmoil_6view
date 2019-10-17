#include "configdata.h"

ConfigData::ConfigData()
{

}
void ConfigData::setCameraSensorWidth(double cameraSensorWidth)
{
    this->cameraSensorWidth = cameraSensorWidth;
}
void ConfigData::setCameraSensorHeight(double cameraSensorHeight)
{
    this->cameraSensorHeight = cameraSensorHeight;
}
void ConfigData::setIcx(double iCx)
{
    this->iCx = iCx;
}
void ConfigData::setIcy(double iCy)
{
    this->iCy = iCy;
}
void ConfigData :: setDcx(double dCx)
{
    this->dCx = dCx;
}
void ConfigData :: setDcy(double dCy)
{
    this->dCy = dCy;
}
void ConfigData :: setRatio(double ratio)
{
    this->ratio = ratio;
}
void ConfigData :: setImageWidth(double imageWidth)
{
    this->imageWidth = imageWidth;
}
void ConfigData :: setImageHeight(double imageHeight)
{
    this->imageHeight = imageHeight;
}
void ConfigData :: setParameter0(double parameter0)
{
    this->parameter0 = parameter0;
}
void ConfigData :: setParameter1(double parameter1)
{
    this->parameter1 = parameter1;
}
void ConfigData :: setParameter2(double parameter2)
{
    this->parameter2 = parameter2;
}
void ConfigData :: setParameter3(double parameter3)
{
    this->parameter3 = parameter3;
}
void ConfigData :: setParameter4(double parameter4)
{
    this->parameter4 = parameter4;
}
void ConfigData :: setParameter5(double parameter5)
{
    this->parameter5 = parameter5;
}
void ConfigData :: setCalibrationRatio(double calibrationRatio)
{
    this->calibrationRatio = calibrationRatio;
}
void ConfigData :: setCameraName(string cameraName)
{
    this->cameraName = cameraName;
}
double ConfigData :: getCameraSensorWidth()
{
    return this->cameraSensorWidth;
}
double ConfigData :: getCameraSensorHeight()
{
    return this->cameraSensorHeight;
}
double ConfigData :: getIcx()
{
    return this->iCx;
}
double ConfigData :: getIcy()
{
    return this->iCy;
}
double ConfigData :: getDcx()
{
    return this->dCx;
}
double ConfigData :: getDcy()
{
    return this->dCy;
}
double ConfigData :: getRatio()
{
    return this->ratio;
}
double ConfigData :: getImageWidth()
{
    return this->imageWidth;
}
double ConfigData :: getImageHeight()
{
    return this->imageHeight;
}
double ConfigData :: getParameter0()
{
    return this->parameter0;
}
double ConfigData :: getParameter1()
{
    return this->parameter1;
}
double ConfigData :: getParameter2()
{
    return this->parameter2;
}
double ConfigData :: getParameter3()
{
    return this->parameter3;
}
double ConfigData :: getParameter4()
{
    return this->parameter4;
}
double ConfigData :: getParameter5()
{
    return this->parameter5;
}
double ConfigData :: getCalibrationRatio()
{
    return this->calibrationRatio;
}
string ConfigData :: getCameraName()
{
    return this->cameraName;
}
