#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/core/core.hpp"

using namespace cv;
using namespace std;

int main()
{
	Mat src, src_gray, dst, abs_dst;

	src = imread("test4\\Laplacian\\1.jpg");
	imshow("��ԭʼͼ��ͼ��Laplace�仯", src);
	GaussianBlur(src, src, Size(3, 3), 0, 0, BORDER_DEFAULT);
	cvtColor(src, src_gray, CV_RGB2GRAY);
	Laplacian(src_gray, dst, CV_16S, 3, 1, 0, BORDER_DEFAULT);
	convertScaleAbs(dst, abs_dst);
	imshow("��Ч��ͼ��ͼ��Laplace�任", abs_dst);

	waitKey();
	return 0;
}