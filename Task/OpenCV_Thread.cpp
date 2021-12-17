#include "OpenCV_Thread.h"
#include <windows.h>

#include "../Filter/OpenCV_Filter.h"

//����ͷ
static cv::VideoCapture capCamera;

OpenCV_Thread::OpenCV_Thread()
{
	isClose = false;
	isCamera = false;
}

OpenCV_Thread::~OpenCV_Thread()
{

}

bool OpenCV_Thread::Open()
{
	mux.lock();
	const bool isOpen = capCamera.open(0);
	mux.unlock();
	if (!isOpen)
	{
		return false;
	}
	isCamera = true;
	return true;
}

void OpenCV_Thread::Close()
{
	isClose = true;
	this->quit();
	this->wait();
}

void OpenCV_Thread::run()
{
	while (1)
	{
		mux.lock();
		if (isClose)
		{
			mux.unlock();
			break;
		}
		if (isCamera && capCamera.isOpened())
		{
			cv::Mat matCamear;
			capCamera.read(matCamear);
			matCamear = OpenCV_Filter::Get()->Filter(matCamear, cv::Mat());
			//����
			emit CameraShow(matCamear);
		}
		Sleep(_fps);

		mux.unlock();
	}
}

