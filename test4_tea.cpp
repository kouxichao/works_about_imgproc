#include <opencv2/opencv.hpp>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace cv;
using namespace std;

int main(int argc, char** argv)
{

	Mat srcImage = imread("test4\\tea\\tea.jpg", 1);
	Mat hsvImage;
	vector<Mat> hsv;
	hsv.resize(3);
	imshow("原始图", srcImage);
	cvtColor(srcImage, hsvImage, CV_BGR2HSV);

	Mat dstImage, dstImageH, dstImageS, dstImageV, tempArray;// = Mat::zeros(srcImage.rows, srcImage.cols, CV_8UC3);

	split(hsvImage, hsv);

	threshold(hsv[0], dstImageH, 60, 255, THRESH_BINARY_INV);
	threshold(hsv[1], dstImageS, 60, 255, THRESH_BINARY);
	threshold(hsv[2], dstImageV, 100, 255, THRESH_BINARY);

	imshow("H", dstImageH);
	imshow("S", dstImageS);
	imshow("V", dstImageV);

	bitwise_and(dstImageH, dstImageS, tempArray);
	bitwise_and(tempArray, dstImageV, dstImage);
	imshow("dst", dstImage);

	for (int i = 0; i < dstImage.rows; i++)
	{
		for (int j = 0; j < dstImage.cols; j++)
		{
			if (dstImage.at<uchar>(i, j) == 255)
			{
				srcImage.at<Vec3b>(i, j)[0] = 0;
				srcImage.at<Vec3b>(i, j)[1] = 0;
				srcImage.at<Vec3b>(i, j)[2] = 255;
			}
		}
	}
	imshow("【分割结果】", srcImage);
	waitKey(0);

}