#include "OpenCV_Action.h"
#include <QFile>
#include <QDebug>
#include <QIODevice>

#define QS(qstr) std::string(qstr2str(QString(qstr)))

inline QString str2qstr(const std::string& str)
{
	return QString::fromLocal8Bit(str.data());
}

inline std::string qstr2str(const QString& qstr)
{
	QByteArray cdata = qstr.toLocal8Bit();
	return std::string(cdata);
}

std::vector<std::string> objects_names_from_file(std::string const filename) {
	std::ifstream file(filename);
	std::vector<std::string> file_lines;
	if (!file.is_open()) return file_lines;
	for (std::string line; getline(file, line);) file_lines.push_back(line);
	std::cout << "object names loaded \n";
	return file_lines;
}

void draw_boxes(cv::Mat mat_img, std::vector<bbox_t> result_vec, std::vector<std::string> obj_names, double spendTime, int current_det_fps = -1, int current_cap_fps = -1)
{
	int const colors[6][3] = { { 1,0,1 },{ 0,0,1 },{ 0,1,1 },{ 0,1,0 },{ 1,1,0 },{ 1,0,0 } };

	for (auto& i : result_vec) {
		cv::Scalar color(0, 255, 0);
		cv::rectangle(mat_img, cv::Rect(i.x, i.y, i.w, i.h), color, 2);
		std::string obj_name = "";
		if (obj_names.size() >= 0)
			obj_name = obj_names[i.obj_id];
		cv::Size const text_size = getTextSize(obj_name, cv::FONT_HERSHEY_COMPLEX_SMALL, 1.2, 2, 0);
		int const max_width = (text_size.width > i.w + 2) ? text_size.width - 5 : (i.w + 2);
		cv::rectangle(mat_img, cv::Point2f(std::max((int)i.x - 1, 0), std::max((int)i.y - 30, 0)),
			cv::Point2f(std::min((int)i.x + max_width, mat_img.cols - 1), std::min((int)i.y, mat_img.rows - 1)),
			color, 1, 8, 0);
		putText(mat_img, obj_name, cv::Point2f(i.x, i.y - 10), cv::FONT_HERSHEY_COMPLEX_SMALL, 1.2, cv::Scalar(0, 0, 0), 2);
	}
	std::string  spend = QS(QString::number(spendTime, 10, 4));
	putText(mat_img, "time:" + spend + "s", cv::Point2f(0, 30), cv::FONT_HERSHEY_COMPLEX_SMALL, 1.2, cv::Scalar(0, 0, 0), 2);
	if (current_det_fps >= 0 && current_cap_fps >= 0) {
		std::string fps_str = "FPS detection: " + std::to_string(current_det_fps) + "   FPS capture: " + std::to_string(current_cap_fps);
		putText(mat_img, fps_str, cv::Point2f(10, 20), cv::FONT_HERSHEY_COMPLEX_SMALL, 1.2, cv::Scalar(50, 255, 0), 2);
	}
}

OpenCV_Action::OpenCV_Action()
{
	obj_names = objects_names_from_file(_nameFile);
	QFile file(_cfgFile.c_str());
	bool isOpen = file.open(QIODevice::ReadOnly);
	if (isOpen)
	{
		qDebug() << "True";
	}
	file.close();
	_detector = new Detector(this->_cfgFile, this->_widgetFile);

}

OpenCV_Action::~OpenCV_Action()
{
	delete _detector;
	_detector = nullptr;
}

void OpenCV_Action::Set(cv::Mat mat1, cv::Mat mat2)
{
	if (mat1.empty())
	{
		return;
	}
	_inputMat = mat1;
}

cv::Mat OpenCV_Action::Get() const
{
	return _inputMat;
}


void OpenCV_Action::Actionyolo4()
{
	auto start = std::chrono::steady_clock::now();
	std::vector<bbox_t> result_vec = this->_detector->detect(_inputMat);
	auto end = std::chrono::steady_clock::now();
	std::chrono::duration<double> spent = end - start;
	draw_boxes(_inputMat, result_vec, obj_names, spent.count());
}

#if 0

cv::String names_file = "coco.names";
cv::String cfg_file = "yolov4.cfg";
cv::String weights_file = "yolov4.weights";

cv::dnn::Net net = cv::dnn::readNetFromDarknet(cfg_file, weights_file);
net.setInput(_inputMat);

std::vector<cv::String> outNames = net.getUnconnectedOutLayersNames();
std::vector<cv::Mat> outs;
net.forward(outs, outNames);

float* data;
cv::Mat scores;
double confidence;
cv::Point classIdPoint;
float confidenceThreshold = 0.2; // ���Ŷ�����
std::vector<int> classIds;
std::vector<float> confidences;
std::vector<cv::Rect> boxes;
int centerX, centerY, width, height, left, top;
//�ҳ����е�Ŀ�꼰��λ��
for (size_t i = 0; i < outs.size(); ++i)
{
	data = (float*)outs[i].data;
	for (int j = 0; j < outs[i].rows; ++j, data += outs[i].cols)
	{
		scores = outs[i].row(j).colRange(5, outs[i].cols);
		cv::minMaxLoc(scores, 0, &confidence, 0, &classIdPoint);
		//minMaxLoc(src, minVal, maxVal, minLoc, maxLoc, mask)��һ���������ҵ�ȫ����Сֵ��ȫ�����ֵ
		if (confidence > confidenceThreshold)
		{
			centerX = (int)(data[0] * _inputMat.cols);
			centerY = (int)(data[1] * _inputMat.rows);
			width = (int)(data[2] * _inputMat.cols);
			height = (int)(data[3] * _inputMat.rows);
			left = centerX - width / 2;
			top = centerY - height / 2;

			classIds.push_back(classIdPoint.x);
			confidences.push_back((float)confidence);
			boxes.push_back(cv::Rect(left, top, width, height));
		}
	}
}
std::vector<int> indices;
cv::dnn::NMSBoxes(boxes, confidences, 0.3, 0.2, indices);

#endif