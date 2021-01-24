#include "CamManager.h"


CamManager::CamManager(int aFps = 25, int aQuality = 720, std::string aPath = "", std::string aExtension = "AVI", HWND aDestination = NULL)
	: videoCapture{0}, videoWriter{"outcpp.avi", VideoWriter::fourcc('D', 'I', 'V', 'X'), aFps, Size(640, 480), true}
{
	fps = aFps;
	quality = aQuality;
	writeNow = false;
	work = true;
	path = aPath;
	extension = aExtension;
	destination = aDestination;
}

CamManager::~CamManager()
{
	videoCapture.release();
	videoWriter.release();
}

void CamManager::setQuality(int aQuality)
{
	quality = aQuality;
}

void CamManager::setFps(int aFps)
{
	fps = aFps;
}

void CamManager::setPath(std::string aPath)
{
	path = aPath;
}

void CamManager::setExtension(std::string aExtension)
{
	extension = aExtension;
}

HBITMAP CamManager::getBitmap()
{
	imageMutex.lock();
	
	HBITMAP toReturn = convertMatToBitmap(&currentImage);	
	imageMutex.unlock();
	return toReturn;
}

bool CamManager::getWriteNow()
{
	return writeNow;
}

void CamManager::startManager()
{ 
	while (work) {

		imageMutex.lock();

		videoCapture >> currentImage;

		flip(currentImage, currentImage, 1);
		std::cout << "worjs";
		if (writeNow)
		{
			videoWriter << currentImage;
		}
		imageMutex.unlock();

		RECT rc;
		rc.left = 400;
		rc.right = 400 + 640;
		rc.top = 50;
		rc.bottom = 50 + 480;

		InvalidateRect(destination, &rc, TRUE);

		
		//imshow("window label", currentImage);

		char c = (char)waitKey(1000 / fps);	
	}
}

void CamManager::stopManager()
{
	work = false;
}

void CamManager::makePhoto()
{
	imageMutex.lock();
	if (!currentImage.empty())
	{
		imwrite(path + "photo.png", currentImage);
	}
	imageMutex.unlock();
}

void CamManager::startVideo()
{
	writeNow = true;
}

void CamManager::stopVideo()
{
	writeNow = false;
}

HBITMAP CamManager::convertMatToBitmap(Mat* image)
{
	HBITMAP hbmp = NULL;
	int bpp = image->channels() * 8;

	Mat tmp;

	cvtColor(*image, tmp, 1);

	BITMAPINFO bmpInfo = { 0 };
	LONG lBmpSize = image->rows * image->cols * 3;
	bmpInfo.bmiHeader.biBitCount = bpp;
	bmpInfo.bmiHeader.biHeight = image->rows;
	bmpInfo.bmiHeader.biWidth = image->cols;

	bmpInfo.bmiHeader.biPlanes = 1;
	bmpInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	// Pointer to access the pixels of bitmap
	BYTE* pPixels = 0, * pP;
	hbmp = CreateDIBSection(NULL, (BITMAPINFO*)&bmpInfo, DIB_RGB_COLORS, (void**)&pPixels, NULL, 0);

	if (!hbmp)
		return hbmp; // return if invalid bitmaps

	   //SetBitmapBits( hBitmap, lBmpSize, pBits);
	   // Directly Write
	int left_width = ((bpp * image->cols + 31) / 32) * 4;
	pP = pPixels;
	for (int y = image->rows - 1, row = 0; row < image->rows; row++, y--) {
		for (int x = 0, col = 0; col < image->cols; x += 3, col++) {
			pP[x] = tmp.at<Vec3b>(y, col).val[0];
			pP[x + 1] = tmp.at<Vec3b>(y, col).val[1];
			pP[x + 2] = tmp.at<Vec3b>(y, col).val[2];
		}
		pP += left_width;
	}
	
	return hbmp;
}