#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/core/core.hpp"

using namespace cv;
using namespace std;

int main()
{
	printf("\n\t����˵����\n"
		"\t\t���̰�����ESC��-�˳�����\n");

	Mat g_srcImage = imread("test4\\threshold\\2.tif", 0);
	if (!g_srcImage.data)
	{
		printf("ͼƬ��ȡ����\n");
		return -1;
	}
	imshow("ԭʼͼ", g_srcImage);

	Mat OtsuImage;
	threshold(g_srcImage, OtsuImage, 0, 255, THRESH_OTSU);
	imshow("OtsuImage", OtsuImage);

	Mat adaptImage;
	adaptiveThreshold(g_srcImage, adaptImage, 255, 0, THRESH_BINARY_INV, 7, 8);
	imshow("adaptImage", adaptImage);

	while (1)
	{
		int key;
		key = waitKey(20);
		if ((char)key == 27)
			break;
	}
}