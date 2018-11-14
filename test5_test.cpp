#include <opencv2/opencv.hpp>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace cv;
using namespace std;

enum ProjectTypes
{
	HORIZONTAL_PROJECT = 0,
	VERTICAL_PROJECT = 1
};

Mat project(const Mat& srcImage, int types, int thresh = 100)
{
	Mat tempImage;

	if (srcImage.channels() > 1)
	{
		cvtColor(srcImage, tempImage, CV_BGR2GRAY);
		threshold(tempImage, tempImage, thresh, 255, THRESH_BINARY);
	}
	else 
	{
		tempImage = srcImage.clone();
	} 

	int width = tempImage.cols;
	int height = tempImage.rows;
	int num = types ? width : height;
	int count = types ? height : width;
	Mat projImage(count, num, CV_8U, Scalar::all(255));
	vector<int> pixelsCount;
	pixelsCount.assign(num, 0);

	for (int row = 0; row < height; row++)
	{
		for (int col = 0; col < width; col++)
		{
			if (tempImage.at<uchar>(row, col) == 255)
			{
				pixelsCount[(types ? col : row)]++;
			}
		}
	}
	
	for (int idx = 0; idx < num; idx++)
	{
		cv::line(projImage, cv::Point(idx, count - pixelsCount[idx]), cv::Point(idx, count - 1), cv::Scalar::all(0));
	}

	return  projImage;
}

int detect_point(const Mat& projImage, vector<int>& segPoint, int dir = 0)
{
	int col = projImage.cols;
	int row = projImage.rows;

	for (int i = row - 2; i >= 0; i--)
	{
		for (int j = 0; j < col; j++)
		{
			if (dir)
			{
				if (projImage.at<uchar>(i, col - j - 1) == 0 && j < col - 5)
				{
					int t = 0;
					for (t = 1; t < 5; t++)
					{
						if (projImage.at<uchar>(i, col - 1 - j - t) != 0)
							break;
					}
					if (t == 5)
					{
						segPoint.push_back(col - j);
						return 0;
					}
				}
			}
			else
			{
				if (projImage.at<uchar>(i, j) == 0 && j < col - 5)
				{
					int t = 0;
					for (t = 1; t < 5; t++)
					{
						if (projImage.at<uchar>(i, j + t) != 0)
							break;
					}
					if (t == 5)
					{
						segPoint.push_back(j);
						return 0;
					}
				}
			}
		}
	}
}

Mat segment(const Mat&srcImage, const Mat& horProjImage, const Mat& verProjImage)
{
	vector<int> segPoint;

	detect_point(horProjImage, segPoint, 0);
	detect_point(horProjImage, segPoint, 1);
	detect_point(verProjImage, segPoint, 0);
	detect_point(verProjImage, segPoint, 1);
	
	printf("%d, %d, %d, %d, %d, %d\n", srcImage.cols, srcImage.rows, segPoint[0], segPoint[1], segPoint[2], segPoint[3]);
	Mat segImage(srcImage, Range(segPoint[0], segPoint[1]), Range(segPoint[2], segPoint[3]));

	return segImage;
}

int main()
{
	Mat grayImage, binaryImage, horProjImg, verProjImg, segImage;
	Mat srcImage = imread("test5\\´ý·Ö¸îÊ¶±ðÍ¼Ïñ\\3.jpg");

	cvtColor(srcImage, grayImage, CV_BGR2GRAY);
	threshold(grayImage, binaryImage, 150, 255, THRESH_BINARY);

	horProjImg = project(binaryImage, 0).clone();
	verProjImg = project(binaryImage, 1).clone();

	segImage = segment(grayImage, horProjImg, verProjImg);
	imshow("¡¾·Ö¸îÍ¼Ïñ¡¿", segImage);

	threshold(segImage, binaryImage, 200, 255, THRESH_OTSU);
	imshow("bbinaryIamge", binaryImage);

	Mat element = getStructuringElement(1, Size(3, 3), Point(-1, -1));
	morphologyEx(binaryImage, binaryImage, MORPH_OPEN, element);
	imshow("binaryIamge", binaryImage);

	horProjImg = project(binaryImage, 0).clone();
	verProjImg = project(binaryImage, 1).clone();
	imshow("horizontal_projectImage", horProjImg);
	imshow("vertical_projectImage", verProjImg);


	waitKey();
	return 0;
}