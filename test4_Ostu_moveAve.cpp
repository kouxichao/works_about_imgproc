
#include <cv.h>
#include <highgui.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
using namespace cv;
/*
对输入图像作移动平均分割
src为单通道灰度图像
num表示计算平均的点数
*/
void averMovingSegmentation(Mat src, Mat dst, int num)
{
	assert(src.channels() == 1 && dst.channels() == 1);
	assert(src.rows == dst.rows && src.cols == dst.cols);

	uchar* dstdata = (uchar*)dst.data;
	uchar* data = (uchar*)src.data;
	int w = src.cols, h = src.rows;
	int step = src.step;
	float n = (float)num;
	float m_pre = data[0] / n, m_now;
	float b = 0.5;
	for (int i = 0; i < h; i++)
	{
		for (int j = 0; j < w; j++)
		{
			int index = i * step + j;
			float dif;
			if (index < num + 1)
				dif = data[index];
			else
				dif = data[index] - data[index - num - 1];

			dif *= 1 / n;
			m_now = m_pre + dif;
			m_pre = m_now;

			if (data[index] > cvRound(b*m_now))
				dstdata[index] = 255;
			else
				dstdata[index] = 0;
		}
	}

}
int main()
{

	Mat src_image = imread("test4\\tea\\tea.jpg"); //("test4\\Otsu\\2.tif");
	cvtColor(src_image, src_image, CV_RGB2GRAY, 1);
	cvNamedWindow("src");
	imshow("src", src_image);
	Mat dst_image(src_image.size(), src_image.type());
	Mat OtsuImage;

	averMovingSegmentation(src_image, dst_image, 20);


	namedWindow("【移动平均】");
	imshow("【移动平均】", dst_image);

	int th = threshold(src_image, OtsuImage, 255, 255, THRESH_OTSU);
	namedWindow("【大津法】");
	imshow("【大津法】", OtsuImage);

	waitKey(0);
	cvDestroyAllWindows();

	return 0;
}
