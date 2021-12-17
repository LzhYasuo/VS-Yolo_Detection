#ifndef _PUB_FUNCTION_H_
#define _PUB_FUNCTION_H_

#include <QObject>

class Pub_Function : public QObject
{
	Q_OBJECT
public:
	static Pub_Function* Get()
	{
		static Pub_Function cx;
		return &cx;
	}
	//���س����ļ�����
	QString appName() const; 
	//���س���ǰ����·��
	QString appPath() const;
	//д����Ϣ������ĵ���Ϣ��־�ļ�
	void writeInfo(const QString& info, bool needWrite = false, const QString& filePath = "log", const QString& FileName = "");
	void writeError(const QString& info, bool needWrite = false, const QString& filePath = "log", const QString& FileName = "");
private:
	Pub_Function();
	virtual ~Pub_Function();
};

#endif //