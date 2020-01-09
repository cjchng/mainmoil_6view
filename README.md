# Mainmoil 6view

A sample project for moil system 6 views display, tested both on ubuntu 18.04 and raspberrypi( Raspbian Buster ) 

![screenshot](https://github.com/cjchng/mainmoil_6view/blob/master/images/screenshot.png?raw=true)

## 1. Requirement 

For RaspberryPi, please download Raspbian opetating system image :
https://www.raspberrypi.org/downloads/raspbian/
following the installing steps : 
https://www.raspberrypi.org/documentation/installation/installing-images/
or tutorial :
https://oranwind.org/-raspberry-pi-win32-disk-imager-shao-lu-sd-qia-jiao-xue/

If you already have Opencv installed, the followings can be skipped. Opencv can be any version, 3.2.0 is recommented.  

	sudo apt update
	sudo apt upgrade
	sudo apt install build-essential cmake pkg-config
	sudo apt install libjpeg-dev libpng-dev libtiff-dev
	sudo apt install software-properties-common
	sudo add-apt-repository "deb http://security.ubuntu.com/ubuntu xenial-security main"
	sudo apt update
	sudo apt install libjasper1 libjasper-dev
	sudo apt update
	sudo apt install libgtk-3-dev
	sudo apt install libatlas-base-dev gfortran
	sudo apt install libopencv-dev python-opencv

## 2. Build

./build

## 3. Run

to run mainmoil,  
./mainmoil

Key operations:

	c : start/stop camera
	0 : switch to original image
	1..6 : switch to channel 1..6
	esc : exit

## 4. Options 

1. Disk Cache

  By default, MAP_CACHE_ENABLED is set as true in mainwindow.cpp for system performance consideration. Both xMaps and yMaps are generated by the calling of AnyPoint function will be store in files and read back next time. It can take about 50 sec. on Raspberry Pi for the first time.














