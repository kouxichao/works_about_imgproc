#include <opencv2/opencv.hpp>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <math.h>

using namespace cv;
using namespace std;

char* imagePath[] = { "poly.jpg", "test6\\1.png", "test6\\交通1.jpg", "test6\\交通2.jpg",
"test6\\交通3.jpg", "test6\\交通5.jpg",  "test6\\交通6.jpg",  "test6\\交通7.jpeg" };

int main()
{
	Mat srcImage = imread(imagePath[0]);
	resize(srcImage, srcImage, Size(480, 360));
	Mat filterImage, grayImage, binaryImage;
	bilateralFilter(srcImage, filterImage, 25, 25 * 2, 25 / 2);
	cvtColor(filterImage, grayImage, CV_RGB2GRAY);
	threshold(grayImage, binaryImage, 200, 255, THRESH_BINARY_INV);

	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;
	findContours(binaryImage, contours, hierarchy, RETR_CCOMP, CHAIN_APPROX_NONE); //找轮廓

	Mat canvas(srcImage.rows, srcImage.cols, CV_8UC3);
	canvas = Scalar::all(255);
	
	cout << contours.size() << endl;
	for (int index = 0; index >= 0; index = hierarchy[index][0])
	{
		cout << "index" << index << endl;
		CvPoint2D32f rectpoint[4];
		CvBox2D rect = minAreaRect(Mat(contours[index]));

		cvBoxPoints(rect, rectpoint); //获取4个顶点坐标  
									  //与水平线的角度  
		int line1 = sqrt((rectpoint[1].y - rectpoint[0].y)*(rectpoint[1].y - rectpoint[0].y) + (rectpoint[1].x - rectpoint[0].x)*(rectpoint[1].x - rectpoint[0].x));
		int line2 = sqrt((rectpoint[3].y - rectpoint[0].y)*(rectpoint[3].y - rectpoint[0].y) + (rectpoint[3].x - rectpoint[0].x)*(rectpoint[3].x - rectpoint[0].x));
		float centerX = (rectpoint[2].x + rectpoint[0].x) / 2;
		float centerY = (rectpoint[2].y + rectpoint[0].y) / 2;
		if (line1*line2 > srcImage.cols * srcImage.rows * 0.9)
			continue;
		Vec3b color = srcImage.at<Vec3b>(centerY, centerX);
		drawContours(binaryImage, contours, index, Scalar(255), CV_FILLED, 8, hierarchy, INT_MAX);
		if (color[0] < 120 && color[1] > 200 && color[2] > 200)
		
		{
			putText(srcImage, "YELLOW", Point(centerX, centerY), 1, 1, Scalar(0, 0, 0),1);
		}
		else if(color[0] > 200 && color[1] < 20 && color[2] < 20)
		{
			putText(srcImage, "BLUE", Point(centerX, centerY), 1, 1, Scalar(0, 0, 0), 1);
		}
		else if (color[0] < 20 && color[1] < 20 && color[2] > 200)
		{
			putText(srcImage, "RED", Point(centerX, centerY), 1, 1, Scalar(0, 0, 0), 1);
		}
		else if (color[0] < 100 && color[1] > 100 && color[2] < 10)
		{
			putText(srcImage, "GREEN", Point(centerX, centerY), 1, 1, Scalar(0, 0, 0), 1);
		}
		else if (color[0] < 20 && color[1] < 20 && color[2] < 20)
		{
			putText(srcImage, "BLACK", Point(centerX, centerY), 1, 1, Scalar(0, 0, 255), 1);
		}
		rectangle(srcImage, rectpoint[0], rectpoint[2], Scalar(255), 2);
	//	imshow("canvas", canvas);
//		waitKey();
	}
	srcImage.copyTo(canvas, binaryImage);
	imshow("src", srcImage);
	imshow("bin", binaryImage);
	imshow("canvas", canvas);
	waitKey();

	return 0;
}