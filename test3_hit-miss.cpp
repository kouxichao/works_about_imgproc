#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include <opencv2/core.hpp>

using namespace cv;


static char* imagePath[] = { "test3\\A.png", "test3\\B.png" };
static Mat srcImage, kernelImage, rePart1, rePart2, reImage;

int main()
{
	//����ͼ��
	srcImage = imread(imagePath[0], CV_LOAD_IMAGE_GRAYSCALE);
	kernelImage = imread(imagePath[1], CV_LOAD_IMAGE_GRAYSCALE);
	
	imshow("��ԭͼ��", srcImage);
	namedWindow("��Ŀ��ͼ��", WINDOW_NORMAL);
	imshow("��Ŀ��ͼ��", kernelImage);
	copyMakeBorder(kernelImage, kernelImage, kernelImage.rows/10, kernelImage.rows/10, kernelImage.cols/10, kernelImage.cols/10, BorderTypes::BORDER_CONSTANT, Scalar(255));
	
	morphologyEx(srcImage, rePart1, cv::MORPH_ERODE, kernelImage, Point(-1, -1), 1);
	imshow("����ʴ���1��", rePart1);

	srcImage = ~srcImage;
	kernelImage = ~kernelImage;
	morphologyEx(srcImage, rePart2, cv::MORPH_ERODE, kernelImage, Point(-1, -1), 1);
	imshow("����ʴ���2��", rePart2);

	reImage = rePart1 & rePart2;
	imshow("�����ͼ��", reImage);
	waitKey(0);
	return 0;
}