#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/photo/photo.hpp"
#include <iostream>
using namespace cv;
using namespace std;

#define WINDOW_NAME0 "【原始图】"
#define WINDOW_NAME1 "【中值滤波】"
#define WINDOW_NAME2 "【修补后效果图】"

Mat srcImage, medianBlurImage, inpaintMask;
Point previousPoint(-1, 1);//原始坐标点
char* imagesPath[] = { "test2\\2.jpg", "test2\\3(椒盐噪声5%).jpg", "test2\\4(椒盐噪声10%)" };
static void On_Mouse(int event, int x, int y, int flags, void*);

int main(int argc, char** argv)
{

	char key;
	
	{
		Mat srcImage;
		key = getchar();
		printf("%c\n", key);
		switch (key)
		{
		case '0':
			srcImage = imread(imagesPath[0], -1); break;
		case '1':
			srcImage = imread(imagesPath[1], -1); break;
		case '2':
			srcImage = imread(imagesPath[2], -1); break;
		default:
			srcImage = imread(imagesPath[0], -1); break;
		}
		if (!srcImage.data)
		{
			fprintf(stderr, "读取图片失败!\n");
			return -1;
		}
		imshow(WINDOW_NAME0, srcImage);

		srcImage = srcImage.clone();
		medianBlurImage = srcImage.clone();
		inpaintMask = Mat::zeros(medianBlurImage.size(), CV_8U);

		medianBlur(srcImage, medianBlurImage, 7);
		imshow(WINDOW_NAME1, medianBlurImage);

		setMouseCallback(WINDOW_NAME1, On_Mouse, 0);
		waitKey(0);

		
		while (1)
		{
			key = waitKey();
			if (key == 's')
			{
				Mat inpaintedImage;
				inpaint(medianBlurImage, inpaintMask, inpaintedImage, 3, CV_INPAINT_TELEA);
				imshow(WINDOW_NAME2, inpaintedImage);
			}
			if (key == 27)
				break;
		}
	}

	return 0;
}
static void On_Mouse(int event, int x, int y, int flags, void*)
{
	if (event == CV_EVENT_LBUTTONUP || !(flags & CV_EVENT_FLAG_LBUTTON))
		previousPoint = Point(-1, -1);
	else if (event == CV_EVENT_LBUTTONDBLCLK)
		previousPoint = Point(x, y);
	else if (event == CV_EVENT_MOUSEMOVE && (flags & CV_EVENT_FLAG_LBUTTON))
	{
		Point pt(x, y);
		if (previousPoint.x < 0)
			previousPoint = pt;
		line(inpaintMask, previousPoint, pt, Scalar::all(255), 5, 8, 0);
		line(medianBlurImage, previousPoint, pt, Scalar::all(255), 5, 8, 0);
		previousPoint = pt;
		imshow(WINDOW_NAME1, medianBlurImage);
	}
}
