#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
using namespace cv;



static Mat g_srcImage, g_srcGrayImage, g_dstImage;

//Canny��Ե�����ر���
Mat g_cannyDetectedEdges;
int g_cannyLowThreshold = 90;//TrackBarλ�ò���  

							 //Sobel��Ե�����ر���
Mat g_sobelGradient_X, g_sobelGradient_Y;
Mat g_sobelAbsGradient_X, g_sobelAbsGradient_Y;
int g_sobelKernelSize = 1;//TrackBarλ�ò���  

						  //Laplacian��Ե�����ر���
static Mat dst;
int g_laplacianScale = 1;


static void on_Canny(int, void*);
static void on_Sobel(int, void*);
static void on_Laplacian(int, void*);



int main(int argc, char** argv)
{

	system("color 2F");


	g_srcImage = imread("test4\\sobel\\1.tif");
	if (!g_srcImage.data) { printf("��ȡsrcImage���� \n"); return false; }

	namedWindow("��ԭʼͼ��");
	imshow("��ԭʼͼ��", g_srcImage);

	g_dstImage.create(g_srcImage.size(), g_srcImage.type());

	cvtColor(g_srcImage, g_srcGrayImage, COLOR_BGR2GRAY);

	namedWindow("��Ч��ͼ��Canny��Ե���", WINDOW_AUTOSIZE);
	namedWindow("��Ч��ͼ��Sobel��Ե���", WINDOW_AUTOSIZE);
	namedWindow("��Ч��ͼ��Laplacian��Ե���", WINDOW_AUTOSIZE);


	createTrackbar("����ֵ��", "��Ч��ͼ��Canny��Ե���", &g_cannyLowThreshold, 120, on_Canny);
	createTrackbar("����ֵ��", "��Ч��ͼ��Sobel��Ե���", &g_sobelKernelSize, 3, on_Sobel);
	createTrackbar("����ֵ1��", "��Ч��ͼ��Laplacian��Ե���", &g_laplacianScale, 6, on_Laplacian);


	on_Canny(0, 0);
	on_Sobel(0, 0);
	on_Laplacian(0, 0);


	//��ѯ��ȡ������Ϣ��������Q�������˳�
	while ((char(waitKey(1)) != 'q')) {}

	return 0;
}




//-----------------------------------��on_Canny( )������----------------------------------
//		������Canny��Ե��ⴰ�ڹ������Ļص�����
//-----------------------------------------------------------------------------------------------
void on_Canny(int, void*)
{
	// ��ʹ�� 3x3�ں�������
	blur(g_srcGrayImage, g_cannyDetectedEdges, Size(3, 3));

	// �������ǵ�Canny����
	Canny(g_cannyDetectedEdges, g_cannyDetectedEdges, g_cannyLowThreshold, g_cannyLowThreshold * 3, 3);

	//�Ƚ�g_dstImage�ڵ�����Ԫ������Ϊ0 
	g_dstImage = Scalar::all(0);

	//ʹ��Canny��������ı�Եͼg_cannyDetectedEdges��Ϊ���룬����ԭͼg_srcImage����Ŀ��ͼg_dstImage��
	g_srcImage.copyTo(g_dstImage, g_cannyDetectedEdges);

	//��ʾЧ��ͼ
	imshow("��Ч��ͼ��Canny��Ե���", g_dstImage);
}



//-----------------------------------��on_Sobel( )������----------------------------------
//		������Sobel��Ե��ⴰ�ڹ������Ļص�����
//-----------------------------------------------------------------------------------------
void on_Sobel(int, void*)
{
	// �� X�����ݶ�
	Sobel(g_srcImage, g_sobelGradient_X, CV_16S, 1, 0, (2 * g_sobelKernelSize + 1), 1, 1, BORDER_DEFAULT);
	convertScaleAbs(g_sobelGradient_X, g_sobelAbsGradient_X);//�������ֵ���������ת����8λ

															 // ��Y�����ݶ�
	Sobel(g_srcImage, g_sobelGradient_Y, CV_16S, 0, 1, (2 * g_sobelKernelSize + 1), 1, 1, BORDER_DEFAULT);
	convertScaleAbs(g_sobelGradient_Y, g_sobelAbsGradient_Y);//�������ֵ���������ת����8λ

															 // �ϲ��ݶ�
	addWeighted(g_sobelAbsGradient_X, 0.5, g_sobelAbsGradient_Y, 0.5, 0, g_dstImage);

	//��ʾЧ��ͼ
	imshow("��Ч��ͼ��Sobel��Ե���", g_dstImage);

}


//-----------------------------------��on_Laplacian( )������----------------------------------
//		������������˹��Ե��ⴰ�ڹ������Ļص�����
//-----------------------------------------------------------------------------------------
void on_Laplacian(int, void*)
{
	Laplacian(g_dstImage, dst, CV_16S, 3, g_laplacianScale, 0, BORDER_DEFAULT);
	convertScaleAbs(dst, dst);

	//��ʾЧ��ͼ
	imshow("��Ч��ͼ��Laplacian��Ե���", dst);

}
