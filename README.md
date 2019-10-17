# mainmoil_6view
moil demo of 6 views on ubuntu or raspberrypi

## 1. Requirement 

### a. opencv 

sudo apt update
sudo apt upgrade
sudo apt install build-essential cmake pkg-config
sudo apt install libjpeg-dev libpng-dev libtiff-dev

sudo apt install software-properties-common
sudo add-apt-repository "deb http://security.ubuntu.com/ubuntu xenial-security main"
sudo apt update
sudo apt install libjasper1 libjasper-dev
sudo apt install libgtk-3-dev
sudo apt install libatlas-base-dev gfortran

## 2. Build

g++ -o main.out main.cpp mainwindow.cpp moildevslim.cpp configdata.cpp `pkg-config --cflags opencv` `pkg-config --libs opencv`

## 3. Run
 
./main.out


