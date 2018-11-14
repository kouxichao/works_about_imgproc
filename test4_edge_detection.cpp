#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
using namespace cv;



static Mat g_srcImage, g_srcGrayImage, g_dstImage;

//Canny边缘检测相关变量
Mat g_cannyDetectedEdges;
int g_cannyLowThreshold = 90;//TrackBar位置参数  

							 //Sobel边缘检测相关变量
Mat g_sobelGradient_X, g_sobelGradient_Y;
Mat g_sobelAbsGradient_X, g_sobelAbsGradient_Y;
int g_sobelKernelSize = 1;//TrackBar位置参数  

						  //Laplacian边缘检测相关变量
static Mat dst;
int g_laplacianScale = 1;


static void on_Canny(int, void*);
static void on_Sobel(int, void*);
static void on_Laplacian(int, void*);



int main(int argc, char** argv)
{

	system("color 2F");


	g_srcImage = imread("test4\\sobel\\1.tif");
	if (!g_srcImage.data) { printf("读取srcImage错误！ \n"); return false; }

	namedWindow("【原始图】");
	imshow("【原始图】", g_srcImage);

	g_dstImage.create(g_srcImage.size(), g_srcImage.type());

	cvtColor(g_srcImage, g_srcGrayImage, COLOR_BGR2GRAY);

	namedWindow("【效果图】Canny边缘检测", WINDOW_AUTOSIZE);
	namedWindow("【效果图】Sobel边缘检测", WINDOW_AUTOSIZE);
	namedWindow("【效果图】Laplacian边缘检测", WINDOW_AUTOSIZE);


	createTrackbar("参数值：", "【效果图】Canny边缘检测", &g_cannyLowThreshold, 120, on_Canny);
	createTrackbar("参数值：", "【效果图】Sobel边缘检测", &g_sobelKernelSize, 3, on_Sobel);
	createTrackbar("参数值1：", "【效果图】Laplacian边缘检测", &g_laplacianScale, 6, on_Laplacian);


	on_Canny(0, 0);
	on_Sobel(0, 0);
	on_Laplacian(0, 0);


	//轮询获取按键信息，若按下Q，程序退出
	while ((char(waitKey(1)) != 'q')) {}

	return 0;
}




//-----------------------------------【on_Canny( )函数】----------------------------------
//		描述：Canny边缘检测窗口滚动条的回调函数
//-----------------------------------------------------------------------------------------------
void on_Canny(int, void*)
{
	// 先使用 3x3内核来降噪
	blur(g_srcGrayImage, g_cannyDetectedEdges, Size(3, 3));

	// 运行我们的Canny算子
	Canny(g_cannyDetectedEdges, g_cannyDetectedEdges, g_cannyLowThreshold, g_cannyLowThreshold * 3, 3);

	//先将g_dstImage内的所有元素设置为0 
	g_dstImage = Scalar::all(0);

	//使用Canny算子输出的边缘图g_cannyDetectedEdges作为掩码，来将原图g_srcImage拷到目标图g_dstImage中
	g_srcImage.copyTo(g_dstImage, g_cannyDetectedEdges);

	//显示效果图
	imshow("【效果图】Canny边缘检测", g_dstImage);
}



//-----------------------------------【on_Sobel( )函数】----------------------------------
//		描述：Sobel边缘检测窗口滚动条的回调函数
//-----------------------------------------------------------------------------------------
void on_Sobel(int, void*)
{
	// 求 X方向梯度
	Sobel(g_srcImage, g_sobelGradient_X, CV_16S, 1, 0, (2 * g_sobelKernelSize + 1), 1, 1, BORDER_DEFAULT);
	convertScaleAbs(g_sobelGradient_X, g_sobelAbsGradient_X);//计算绝对值，并将结果转换成8位

															 // 求Y方向梯度
	Sobel(g_srcImage, g_sobelGradient_Y, CV_16S, 0, 1, (2 * g_sobelKernelSize + 1), 1, 1, BORDER_DEFAULT);
	convertScaleAbs(g_sobelGradient_Y, g_sobelAbsGradient_Y);//计算绝对值，并将结果转换成8位

															 // 合并梯度
	addWeighted(g_sobelAbsGradient_X, 0.5, g_sobelAbsGradient_Y, 0.5, 0, g_dstImage);

	//显示效果图
	imshow("【效果图】Sobel边缘检测", g_dstImage);

}


//-----------------------------------【on_Laplacian( )函数】----------------------------------
//		描述：拉普拉斯边缘检测窗口滚动条的回调函数
//-----------------------------------------------------------------------------------------
void on_Laplacian(int, void*)
{
	Laplacian(g_dstImage, dst, CV_16S, 3, g_laplacianScale, 0, BORDER_DEFAULT);
	convertScaleAbs(dst, dst);

	//显示效果图
	imshow("【效果图】Laplacian边缘检测", dst);

}
