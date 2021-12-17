#include "GuiOpenGLWidget.h"
#include <QPainter>

GuiOpenGLWidget::GuiOpenGLWidget(QWidget* parent /*= nullptr*/)
	:QOpenGLWidget(parent)
{

	//_img = QImage();
	//QImage::Format fmt = QImage::Format_RGB888;
	//unsigned char* imgSize = new unsigned char[(this->width() * this->height() * 3)];
	//_img = QImage(imgSize, this->width(), this->height(), fmt);
}

GuiOpenGLWidget::~GuiOpenGLWidget()
{

}

void GuiOpenGLWidget::ShowImg(cv::Mat mat)
{
	QImage::Format fmt = QImage::Format_RGB888;
	int pixSize = 3;
	//����ǻҶ�ͼ ��Ϊrgb�� w*h*3 ���Ҷ�ͼ���� w*h*1;
	if (mat.type() == CV_8UC1)
	{
		fmt = QImage::Format_Grayscale8;
		pixSize = 1;
	}
	if (fmt != _img.format() || _img.isNull())
	{
		//ͼƬ�Ĵ�СӦ�����ĵ� w*h*pixSize (PixSize������rgbͼ���ǻҶ�ͼ)
		delete _img.bits();
		unsigned char* imgSize = new unsigned char[(this->width()) * (this->height()) * pixSize];
		_img = QImage(imgSize, this->width(), this->height(), fmt);
	}
	cv::Mat des;
	cv::resize(mat, des, cv::Size(_img.size().width(), _img.size().height()));
	if (pixSize > 1)
	{
		//QT�������ɫ������BGR
		cv::cvtColor(des, des, cv::COLOR_BGR2RGB);
	}
	memcpy(_img.bits(), des.data, des.cols * des.rows * des.elemSize());
	update();
}

void GuiOpenGLWidget::paintEvent(QPaintEvent* e)
{
	QPainter p;
	p.begin(this);
	p.drawImage(QPointF(0, 0), _img);
	p.end();
}

