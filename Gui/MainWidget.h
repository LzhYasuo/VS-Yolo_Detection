#ifndef _MAIN_WIDGET_H_
#define _MAIN_WIDGET_H_

#include "GuiBaseWidget.h"


class OpenCV_Thread;
class MainWidget : public GuiBaseWidget
{
	Q_OBJECT
public:
	MainWidget(QWidget* parent = nullptr);
	virtual ~MainWidget();

private:
	void Init();
	QWidget* InitWidget();

private:
	OpenCV_Thread* Thread = nullptr;

};


#endif //MAIN_WIDGET_H_