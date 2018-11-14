#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/core/core.hpp"

using namespace cv;
using namespace std;

int main()
{
	Mat srcImage = imread("test4\\canny\\1.jpg");
	Mat srcImage1 = srcImage.clone();
	Mat dst;
	imshow("��ԭʼͼ��Canny��Ե���", srcImage);
	Canny(srcImage, dst, 150, 100, 3);
	imshow("��Ч��ͼ��Canny��Ե���", dst);


	Mat dstImage, edge, grayImage;
	dstImage.create(srcImage1.size(), srcImage1.type());
	cvtColor(srcImage1, grayImage, CV_BGR2GRAY);
	blur(grayImage, edge, Size(3, 3));
	Canny(edge, edge, 3, 9, 3);
	dstImage = Scalar::all(0);
	srcImage1.copyTo(dstImage, edge);
	imshow("��Ч��ͼ��Canny��Ե���2", dstImage);

	waitKey(0);
	return 0;
}