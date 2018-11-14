#include "core/core.hpp"
#include "highgui/highgui.hpp"
#include "imgproc/imgproc.hpp"

using namespace cv;


int main()
{
	Mat image, imageGray, imageDilate, imageErode, imageBorder;
	image = imread("test3\\3.jpg");
	cvtColor(image, imageGray, CV_RGB2GRAY);

	//enum { MORPH_RECT=0, MORPH_CROSS=1, MORPH_ELLIPSE=2 };
	Mat element = getStructuringElement(1, Size(3, 3), Point(-1, -1));
	dilate(imageGray, imageDilate, element, Point(-1, -1));
	erode(imageGray, imageErode, element, Point(-1, -1));
	imageBorder = imageGray - imageErode;
	imshow("Source image", image);
//	imshow("Dilate", imageDilate);
	imshow("Erode", imageErode);
	imshow("Border", imageBorder);
	waitKey(0);
}