
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
using namespace cv;
using namespace std;
// 分水岭分割显示
cv::Mat displaySegResult(cv::Mat & segments, int numOfSegments, cv::Mat & image)
{
	cv::Mat wshed(segments.size(), CV_8UC3);
	// 创建对于颜色分量
	vector<Vec3b> colorTab;
	for (int i = 0; i < numOfSegments; i++)
	{
		int b = theRNG().uniform(0, 255);
		int g = theRNG().uniform(0, 255);
		int r = theRNG().uniform(0, 255);
		colorTab.push_back(Vec3b((uchar)b, (uchar)g, (uchar)r));
	}
	//应用不同颜色对每个部分
	for (int i = 0; i < segments.rows; i++)
	{
		for (int j = 0; j < segments.cols; j++)
		{
			int index = segments.at<int>(i, j);
			
			if (index == -1)
				wshed.at<Vec3b>(i, j) = Vec3b(255, 255, 255);
			else if (index <= 0 || index > numOfSegments)
				wshed.at<Vec3b>(i, j) = Vec3b(0, 0, 0);
			else
				wshed.at<Vec3b>(i, j) = colorTab[index - 1];
		}
	}
	if (image.dims > 0)
		wshed = wshed * 0.5 + image * 0.5;
	return wshed;
}

int main()
{
	cv::Mat srcImage = imread("test4\\watershed\\2.tif");
	if (!srcImage.data)
		return 1;
	cv::imshow("【原始图片】", srcImage);
	
	cv::Mat gray, blurImage, edgeImage;
	cv::cvtColor(srcImage, gray, CV_BGR2GRAY);

	GaussianBlur(gray, blurImage, Size(5, 5), 2);
	Canny(blurImage, edgeImage, 150, 90);
	int i, j, compCount = 0;
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;
	findContours(edgeImage, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE);
	if (contours.empty())
		return -1;

	Mat markers(edgeImage.size(), CV_32S);
	markers = Scalar::all(0);
	int idx = 0;
	// 绘制区域块
	for (; idx >= 0; idx = hierarchy[idx][0], compCount++)
	{
		drawContours(markers, contours, idx, Scalar::all(compCount + 1), -1, 8, hierarchy, INT_MAX);

	}

	if (compCount == 0)
		return -1;
	watershed(srcImage, markers);

	Mat wshed = displaySegResult(markers, compCount, srcImage);
	imshow("【分水岭结果】", wshed);

	cv::waitKey(0);
	return 0;
}