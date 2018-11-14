#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
using namespace std;
using namespace cv;
cv::Mat frameImg;
Mat output;
int main()
{

	VideoCapture capture("1.avi");

	capture.set(CV_CAP_PROP_FRAME_WIDTH, 1920);
	capture.set(CV_CAP_PROP_FRAME_HEIGHT, 1080);

	if (!capture.isOpened())
	{
		std::cout << "fail to open video!" << std::endl;
		return -1;
	}
	
	while (true)
	{
		capture >> frameImg;
	
		if (!frameImg.empty())
		{
			cvtColor(frameImg, output, CV_BGR2GRAY);
			GaussianBlur(output, output, Size(3, 3), 0,0);
			Canny(output, output, 100, 200, 3);
			imshow("frameImg", output);
		}

		else
		{
			break;
		}

		if (char(waitKey(1)) == 'q')
		{
			break;
		}

	}

	capture.release();
	return 0;
}