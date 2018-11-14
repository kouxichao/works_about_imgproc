#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"

using namespace cv;
using namespace std;
int threshold_value = 0;
int threshold_type = 3;
int const max_value = 255;
int const max_type = 4;
int const max_BINARY_value = 255;
Mat src, src_gray, dst;
const char* window_name = "����ֵ����";
const char* trackbar_type = "����ֵ���͡�";
const char* trackbar_value = "����ֵ��";
void on_Threshold(int, void*);

int main()
{
	String imageName("test4\\threshold\\2.tif");
	src = imread(imageName, IMREAD_COLOR); // Load an image
	if (src.empty())
	{
		return -1;
	}
	cvtColor(src, src_gray, COLOR_BGR2GRAY);
	namedWindow(window_name, WINDOW_AUTOSIZE);
	createTrackbar(trackbar_type,
		window_name, &threshold_type,
		max_type, on_Threshold);
	createTrackbar(trackbar_value,
		window_name, &threshold_value,
		max_value, on_Threshold);
	on_Threshold(0, 0);
	while (1)
	{
		char c = (char)waitKey(0);
		if (c == 27)
		{
			break;
		}
	}
}
void on_Threshold(int, void*)
{
	/* 0: THRESH BINARY��       ��������ֵ�������ø���ֵ���͵�ʱ����Ҫѡ��һ���ض�����ֵ�������磺125��
	�������µ���ֵ����������Խ���Ϊ����125�����ص�ĻҶ�ֵ�趨Ϊ���ֵ255��
	�Ҷ�ֵС��125�����ص�ĻҶ�ֵ�趨Ϊ0��
	1: THRESH BINARY INV��   ����������ֵ���趨һ����ʼ��ֵ��125�������125���趨Ϊ0����С�ڸ���ֵ���趨Ϊ255��
	2: THRESH TRUNC��        �ض���ֵ��ͬ��������Ҫѡ��һ����ֵ��ͼ���д��ڸ���ֵ�����ص㱻�趨Ϊ����ֵ��С�ڸ���ֵ�ı��ֲ��䡣
	�����磺��ֵѡȡΪ125����С��125����ֵ���ı䣬����125�ĻҶ�ֵ��230�������ص���趨Ϊ����ֵ����
	3:  THRESH TOZERO:       ��ֵ��Ϊ0����ѡ��һ����ֵ�����ص�ĻҶ�ֵ���ڸ���ֵ�Ĳ������κθı䣻���ص�ĻҶ�ֵС�ڸ���ֵ�ģ�
	��Ҷ�ֵȫ����Ϊ0��
	4: THRESH TOZERO INV��   ����ֵ��Ϊ0��ԭ��������0��ֵ�������ڶ�ͼ���������ʱ���෴���������ص�ĻҶ�ֵС�ڸ���ֵ�Ĳ������κθı䣬
	�����ڸ���ֵ�Ĳ��֣���Ҷ�ֵȫ����Ϊ0��
	*/
	threshold(src_gray, dst, threshold_value, max_BINARY_value, threshold_type);
	imshow(window_name, dst);
}