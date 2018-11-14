#include <opencv2/opencv.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include<iostream>


using namespace std;
using namespace cv;


int main()
{
	Mat img_h, img_s, img_v, imghsv, imgqiangge;
	Mat Image;
	Image = imread("test.jpg");      //原图
	vector<cv::Mat> hsv_vec;
	cvtColor(Image, imghsv, CV_BGR2HSV);

	// 分割hsv通道  
	split(imghsv, hsv_vec);
	img_h = hsv_vec[0];
	img_s = hsv_vec[1];
	img_v = hsv_vec[2];
	imshow("h", img_h);

	imshow("s", img_s);

	imshow("h_v", img_v);

	waitKey(0);

	return 0;
}



