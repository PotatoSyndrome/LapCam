#pragma once
#include <string>
#include <mutex>

#include <Windows.h>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <opencv2/opencv.hpp>


using namespace cv;

class CamManager
{
private:
	int quality;
	int fps;
	bool writeNow;
	bool work;
	std::string path;
	std::string extension;
	Mat currentImage;
	VideoCapture videoCapture;
	VideoWriter videoWriter;
	Mutex imageMutex;
	HWND destination;

	HBITMAP convertMatToBitmap(Mat* image);
public:
	CamManager(int aQuality, int aFps, std::string aPath, std::string aExtension, HWND aDestination);
	~CamManager();

	void setQuality(int aQuality);
	void setFps(int aFps);
	void setPath(std::string aPath);
	void setExtension(std::string aExtension);
	HBITMAP getBitmap();
	bool getWriteNow();

	void startManager();
	void stopManager();
	void makePhoto();
	void startVideo();
	void stopVideo();
};

