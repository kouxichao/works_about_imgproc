#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"

using namespace cv;
using namespace std;
int threshold_value = 0;
int threshold_type = 3;
int const max_value = 255;
int const max_type = 4;
int const max_BINARY_value = 255;
Mat src, src_gray, dst;
const char* window_name = "【阈值处理】";
const char* trackbar_type = "【阈值类型】";
const char* trackbar_value = "【阈值】";
void on_Threshold(int, void*);

int main()
{
	String imageName("test4\\threshold\\2.tif");
	src = imread(imageName, IMREAD_COLOR); // Load an image
	if (src.empty())
	{
		return -1;
	}
	cvtColor(src, src_gray, COLOR_BGR2GRAY);
	namedWindow(window_name, WINDOW_AUTOSIZE);
	createTrackbar(trackbar_type,
		window_name, &threshold_type,
		max_type, on_Threshold);
	createTrackbar(trackbar_value,
		window_name, &threshold_value,
		max_value, on_Threshold);
	on_Threshold(0, 0);
	while (1)
	{
		char c = (char)waitKey(0);
		if (c == 27)
		{
			break;
		}
	}
}
void on_Threshold(int, void*)
{
	/* 0: THRESH BINARY：       二进制阈值。在运用该阈值类型的时候，先要选定一个特定的阈值量，比如：125，
	这样，新的阈值产生规则可以解释为大于125的像素点的灰度值设定为最大值255，
	灰度值小于125的像素点的灰度值设定为0。
	1: THRESH BINARY INV：   反二进制阈值。设定一个初始阈值如125，则大于125的设定为0，而小于该阈值的设定为255。
	2: THRESH TRUNC：        截断阈值。同样首先需要选定一个阈值，图像中大于该阈值的像素点被设定为该阈值，小于该阈值的保持不变。
	（例如：阈值选取为125，那小于125的阈值不改变，大于125的灰度值（230）的像素点就设定为该阈值）。
	3:  THRESH TOZERO:       阈值化为0。先选定一个阈值，像素点的灰度值大于该阈值的不进行任何改变；像素点的灰度值小于该阈值的，
	其灰度值全部变为0。
	4: THRESH TOZERO INV：   反阈值化为0。原理类似于0阈值，但是在对图像做处理的时候相反，即：像素点的灰度值小于该阈值的不进行任何改变，
	而大于该阈值的部分，其灰度值全部变为0。
	*/
	threshold(src_gray, dst, threshold_value, max_BINARY_value, threshold_type);
	imshow(window_name, dst);
}