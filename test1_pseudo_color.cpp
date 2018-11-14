
#include "cv.h"                         
#include "highgui.h"
#include "cvaux.h"
#include "cxcore.h"
#include "opencv2/opencv.hpp"
#include "opencv2/imgproc.hpp"
#include <iostream>
#include <string>

using namespace cv;
using namespace std;


Mat image;//原图像
Mat target1;//线性变换输出图像
Mat target2;//分段线性变换输出图像
			//target3 //伪彩色图像

			//原始图像灰度范围[a,b]
int a = 0;
int b = 255;
Vec3b GreyToColorMix(Mat val, float c, float d)
{
	cv::Mat src = cv::Mat::zeros(d, c, CV_8UC3);

	for (int y = 0; y < val.rows; y++)
		for (int x = 0; x < val.cols; x++)
		{
			//red
			if (val.at<uchar>(y, x) < 128)
			{
				src.at<cv::Vec3b>(y, x)[0] = 0;
			}
			else if (val.at<uchar>(y, x) < 192)
			{
				src.at<cv::Vec3b>(y, x)[0] = 255 / 64 * (val.at<uchar>(y, x) - 128);
			}
			else
			{
				src.at<cv::Vec3b>(y, x)[0] = 255;
			}

			//green
			if (val.at<uchar>(y, x) < 64)
			{
				src.at<cv::Vec3b>(y, x)[1] = 255 / 64 * val.at<uchar>(y, x);
			}
			else if (val.at<uchar>(y, x) < 192)
			{
				src.at<cv::Vec3b>(y, x)[1] = 255;
			}
			else
			{
				src.at<cv::Vec3b>(y, x)[1] = -255 / 63 * (val.at<uchar>(y, x) - 192) + 255;
			}

			//blue
			if (val.at<uchar>(y, x) < 64)
			{
				src.at<cv::Vec3b>(y, x)[2] = 255;
			}
			else if (val.at<uchar>(y, x) < 128)
			{
				src.at<cv::Vec3b>(y, x)[2] = -255 / 63 * (val.at<uchar>(y, x) - 192) + 255;
			}
			else
			{
				src.at<cv::Vec3b>(y, x)[2] = 0;
			}
		}
	cv::imshow("target3", src);
	cv::waitKey(0);
	return 0;
}


//灰度线性变换，前提是传入的图像是灰度图
void toGray(Mat src, float c, float d)
{
	target1 = Mat::zeros(src.size(), src.type());

	for (int i = 0; i < src.rows; i++)
	{
		for (int j = 0; j < src.cols; j++)
		{
			//线性变换,saturate_cast作用是防止数组越界
			target1.at<uchar>(i, j) = saturate_cast<uchar>((d - c) / (b - a) * (src.at<uchar>(i, j) - a) + c);

		}
	}
}

//灰度分段线性变换
void piecewiseToGray(Mat src, float c, int d)
{
	target2 = Mat::zeros(src.size(), src.type());

	for (int i = 0; i < src.rows; i++)
	{
		for (int j = 0; j < src.cols; j++)
		{
			if (target2.at<uchar>(i, j) < c)
				target2.at<uchar>(i, j) = saturate_cast<uchar>((80 / c) * src.at<uchar>(i, j));
			else if (c < target2.at<uchar>(i, j) && target2.at<uchar>(i, j) < d)
				target2.at<uchar>(i, j) = saturate_cast<uchar>((180 - 80) / (d - c) * (src.at<uchar>(i, j) - c) + 80);
			else
				target2.at<uchar>(i, j) = saturate_cast<uchar>((255 - 180) / (255 - d) * (src.at<uchar>(i, j) - d) + 180);
		}
	}

}
//获取原图像灰度范围
void getGrayValue(Mat src)
{
	int value = 0;
	for (int i = 0; i < src.rows; i++)
	{
		for (int j = 0; j < src.cols; j++)
		{
			value = (int)src.at<uchar>(i, j);
			if (value > b)
				b = value;
			if (value < a)
				a = value;

		}
	}
}


int main()
{
	//加上0表示读入灰度图
	image = imread("test.jpg", 0);

	if (image.empty())
	{
		printf("could not load pic!\n");
		return -1;
	}

	namedWindow("src");
	imshow("src", image);
	//获得原图像灰度范围
	getGrayValue(image);




	//根据目标图像灰度范围生成目标图像
	toGray(image, 30, 150);
	piecewiseToGray(image, 50, 150);


	namedWindow("target1", WINDOW_AUTOSIZE);
	namedWindow("target2", WINDOW_AUTOSIZE);

	imshow("target1", target1);
	imshow("target2", target2);
	GreyToColorMix(image, image.cols, image.rows);
	imwrite("D:/opencv/testPic/d2.bmp", target1);
	imwrite("D:/opencv/testPic/d3.bmp", target2);

	waitKey(0);

	return 0;

}






