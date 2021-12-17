#include "Pub_Function.h"
#include <QApplication>
#include <QIODevice>
#include <QDateTime>
#include <QTextStream>
#include <QFile>

QString Pub_Function::appName() const
{
	QString name;
	if (name.isEmpty())
	{
		name = qApp->applicationName();
		QStringList list = name.split("/");
		name = list.at(list.count() - 1).split(".").at(0);
	}
	return name;
}

QString Pub_Function::appPath() const
{
	//���������Android����
#ifdef Q_OS_ANDROID
	const QString path = QString("/sdcard/Android/%1").arg(appName());
	return path;
#else
	const QString path = qApp->applicationDirPath();
	return path;
#endif
}

void Pub_Function::writeInfo(const QString& info, bool needWrite /*= false*/, const QString& filePath /*= "log"*/, const QString& FileName /*= ""*/)
{
	if (!needWrite)
		return;

	QString fileName = QString("%1/%2%3_%4reinfo.txt")
		.arg(this->appPath()).arg(filePath).arg(this->appName()).arg(FileName)/*.arg(QDate::currentDate().toString())*/;

	QFile File(fileName);
	if (File.open(QIODevice::WriteOnly | QIODevice::Append | QFile::Text))
	{
		QTextStream stream(&File);
		stream << "\n" << QDateTime::currentDateTime().toString() << " " << info;
		File.close();
	}
}

void Pub_Function::writeError(const QString& info, bool needWrite /*= false*/, const QString& filePath /*= "log"*/, const QString& FileName /*= ""*/)
{
	if (!needWrite)
		return;
	QString fileName = QString("%1/%2%3_%4reerror.txt")
		.arg(this->appPath()).arg(filePath).arg(this->appName()).arg(FileName)/*.arg(QDate::currentDate().toString())*/;

	QFile File(fileName);
	if (File.open(QIODevice::WriteOnly | QIODevice::Append | QFile::Text))
	{
		QTextStream stream(&File);
		stream << "\n" << QDateTime::currentDateTime().toString() << " " << info;
		File.close();
	}
}

Pub_Function::Pub_Function()
{

}

Pub_Function::~Pub_Function()
{

}

