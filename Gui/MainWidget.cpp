#include "MainWidget.h"

#include <QPushButton>
#include <QLabel>
#include <QHBoxLayout>
#include <QDebug>

#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/core.hpp>

#include "GuiOpenGLWidget.h"
#include "../Task/OpenCV_Thread.h"
#include "../Pub/Pub_Function.h"
#include "../Filter/OpenCV_Filter.h"

MainWidget::MainWidget(QWidget* parent /*= nullptr*/)
{
	qRegisterMetaType<cv::Mat>("cv::Mat");
	Thread = new OpenCV_Thread();
	Thread->start();

	this->setFixedSize(800, 600);
	this->Init();

}

MainWidget::~MainWidget()
{

}

void MainWidget::Init()
{
	QHBoxLayout* mLayout = Get_HLayout();

	QWidget* mWidget = InitWidget();

	mLayout->addWidget(mWidget);

	this->setLayout(mLayout);
}

QWidget* MainWidget::InitWidget()
{
	QWidget* mWidget = new QWidget();
	QVBoxLayout* mLayout = Get_VLayout();

	GuiOpenGLWidget* glWidget = new GuiOpenGLWidget();
	glWidget->setFixedWidth(800);

	QHBoxLayout* buttonLayout = Get_HLayout();
	QPushButton* btn_Clear = Get_Button("�������");

	QPushButton* btn_Open = Get_Button("������ͷ");

	QPushButton* btn_yolo4 = Get_Button("yolo4");
	buttonLayout->addWidget(btn_Clear);
	buttonLayout->addWidget(btn_Open);
	buttonLayout->addWidget(btn_yolo4);

	mLayout->addWidget(glWidget);
	mLayout->addLayout(buttonLayout);
	mWidget->setLayout(mLayout);

	const auto OpenFunction = [=]()
	{
		const bool isOpen = Thread->Open();
		if (!isOpen)
		{
			Pub_Function::Get()->writeError("Open Camear_Error");
			return;
		}
	};
	this->connect(btn_Open, &QPushButton::clicked, this, [=]() {OpenFunction();});
	this->connect(Thread, &OpenCV_Thread::CameraShow, glWidget, &GuiOpenGLWidget::ShowImg);
	this->connect(btn_Clear, &QPushButton::clicked, this, [=]() {OpenCV_Filter::Get()->Clear(); });
	this->connect(btn_yolo4, &QPushButton::clicked, this, [=]() {OpenCV_Filter::Get()->Add(CV_Task{ TASKYOLO4 ,{} }); });
	//this->connect(Thread, &OpenCV_Thread::CameraShow, this, [=]() {qDebug() << "1111"; });

	return mWidget;
}
