#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/photo/photo.hpp"

using namespace cv;
using namespace std;

static Mat g_srcImage, g_dstImage;
int g_nElementShape = MORPH_RECT;

int g_nMaxIterationNum = 10; 
int g_nOpenCloseNum = 0;
int g_nErodeDilateNum = 0;

static void on_OpenClose(int, void*);
static void on_ErodeDilate(int, void*);
static void on_TopBlackHat(int, void*);

static char* imagesPath[] = { "test3\\3.jpg" };

int main()
{
	g_srcImage = imread(imagesPath[0], -1);
	if (!g_srcImage.data)
	{
		fprintf(stderr, "��ȡͼƬʧ��!\n");
		return -1;
	}
	cvtColor(g_srcImage, g_srcImage, CV_BGR2GRAY);
	g_srcImage = ~g_srcImage;
	namedWindow("��ԭʼͼ��");
	imshow("��ԭʼͼ��", g_srcImage);

	namedWindow("��������/�����㡿");
	namedWindow("����ʴ/���͡�");

	g_nOpenCloseNum = 9;
	g_nErodeDilateNum = 9;

	createTrackbar("����ֵ", "��������/�����㡿", &g_nOpenCloseNum, g_nMaxIterationNum * 2 + 1, on_OpenClose);
	createTrackbar("����ֵ", "����ʴ/���͡�", &g_nErodeDilateNum, g_nMaxIterationNum * 2 + 1, on_ErodeDilate);

	on_OpenClose(g_nOpenCloseNum, 0);
	on_ErodeDilate(g_nErodeDilateNum, 0);

	while (1)
	{
		int c;
		c = waitKey(0);

		if ((char)c == 'q' || (char)c == 27)
			break;
	}

	return 0;
}

static void on_OpenClose(int, void*)
{
	int offset = g_nOpenCloseNum - g_nMaxIterationNum;
	int Absolute_offset = offset > 0 ? offset : -offset;

	Mat element = getStructuringElement(g_nElementShape, Size(Absolute_offset*2+1, Absolute_offset*2+1), Point(Absolute_offset, Absolute_offset));

	if (offset < 0)
	{
		morphologyEx(g_srcImage, g_dstImage, CV_MOP_OPEN, element);
	}
	else
	{
		morphologyEx(g_srcImage, g_dstImage, CV_MOP_CLOSE, element);
	}

	imshow("��������/�����㡿", g_dstImage);
}

static void on_ErodeDilate(int, void*)
{
	int offset = g_nErodeDilateNum - g_nMaxIterationNum;
	int Absolute_offset = offset > 0 ? offset : -offset;

	Mat element = getStructuringElement(g_nElementShape, Size(Absolute_offset * 2 + 1, Absolute_offset * 2 + 1), Point(Absolute_offset, Absolute_offset));

	if (offset < 0)
	{
		erode(g_srcImage, g_dstImage, element);
	}
	else
	{
		dilate(g_srcImage, g_dstImage, element);
	}

	imshow("����ʴ/���͡�", g_dstImage);
}

