#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/photo/photo.hpp"
#include <iostream>

using namespace cv;

const int g_nMaxValueSlider = 100;
int g_fGammaValueSlider = 20;
static Mat srcImage;

void on_Trackbar(int, void*);

char* imagePath[] = {"test1\\1.jpg"};
int main()
{
	srcImage = imread(imagePath[0]);
	if (!srcImage.data)
	{
		printf("读取图片失败!\n");
		return -1;
	}
	namedWindow("原图");
	imshow("原图", srcImage);

	Mat rgbChannels[3];
	split(srcImage, rgbChannels);
	Mat BlueChannelDisplay;
	Mat GreenChannelDisplay;
	Mat RedChannelDisplay;

	BlueChannelDisplay = rgbChannels[0];
	GreenChannelDisplay = rgbChannels[1];
	RedChannelDisplay = rgbChannels[2];

	namedWindow("B_channel");
	imshow("B_channel", BlueChannelDisplay);

	namedWindow("G_channel");
	imshow("G_channel", GreenChannelDisplay);

	namedWindow("R_channel");
	imshow("R_channel", RedChannelDisplay);

	//直方图均衡化
	std::vector<Mat> channels;

	Mat imageBlueChannel;
	Mat imageGreenChannel;
	Mat imageRedChannel;
	Mat equalizeHistImage;

	split(srcImage, channels);
	imageBlueChannel = channels.at(0);
	imageGreenChannel = channels.at(1);
	imageRedChannel = channels.at(2);

	equalizeHist(imageBlueChannel, imageBlueChannel);
	equalizeHist(imageGreenChannel, imageGreenChannel);
	equalizeHist(imageRedChannel, imageRedChannel);

	merge(channels, equalizeHistImage);

	namedWindow("直方图均衡化");
	imshow("直方图均衡化", equalizeHistImage);

	Mat kernel = (Mat_<float>(3, 3) << 0, -1, 0, -1, 5, -1, 0, -1, 0);

	Mat filter2DImage;
	filter2D(srcImage, filter2DImage, srcImage.depth(), kernel);

	namedWindow("锐化滤波器");
	imshow("锐化滤波器", filter2DImage);

	namedWindow("伽马矫正", 1);
	char TrackbarName[50];
	sprintf_s(TrackbarName, "fGama %d", g_nMaxValueSlider);

	createTrackbar(TrackbarName, "伽马矫正", &g_fGammaValueSlider, g_nMaxValueSlider, on_Trackbar);
	on_Trackbar(g_nMaxValueSlider, 0);

	waitKey(0);

	return 0;
}

void on_Trackbar(int, void*)
{
	float fGamma = (float)g_fGammaValueSlider / 50;
	Mat GamaImage;

	unsigned char lut[256];
	for (int i = 0; i < 256; i++)
	{
		lut[i] = saturate_cast<uchar>(pow(i/255.0, fGamma) * 255.0f);
	}

	GamaImage = srcImage.clone();

	MatIterator_<Vec3b> it, end;
	for (it = GamaImage.begin<Vec3b>(), end = GamaImage.end<Vec3b>(); it != end; it++)
	{
		(*it)[0] = lut[(*it)[0]];
		(*it)[1] = lut[(*it)[1]];
		(*it)[2] = lut[(*it)[2]];
	}

	imshow("伽马矫正", GamaImage);
}