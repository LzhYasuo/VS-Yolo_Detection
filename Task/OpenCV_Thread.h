#ifndef _OPENCV_THREAD_H_
#define _OPENCV_THREAD_H_

#include <QThread>
#include <QMutex>

#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/core.hpp>

class OpenCV_Thread : public QThread
{
	Q_OBJECT
public:
	OpenCV_Thread();
	~OpenCV_Thread();

	bool Open();
	void Close();
	virtual void run();

signals:
	void CameraShow(cv::Mat);


private:
	QMutex mux;
	//�Ƿ��˳�
	bool isClose;
	//����ͷ�Ƿ��
	bool isCamera;

	//��������ͷ������� fps����д��
	const int _fps = 0;
};



#endif //_OPENCV_THREAD_H_