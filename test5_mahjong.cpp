#include <opencv2/opencv.hpp>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <math.h>

using namespace cv;
using namespace std;

enum ProjectTypes
{
	HORIZONTAL_PROJECT = 0,
	VERTICAL_PROJECT = 1
};

vector<int> project(const Mat& srcImage, int types, int thresh = 100)
{
	Mat tempImage;

	if (srcImage.channels() > 1)
	{
		cvtColor(srcImage, tempImage, CV_BGR2GRAY);
		threshold(tempImage, tempImage, thresh, 255, THRESH_BINARY);
	}
	else
	{
		tempImage = srcImage.clone();
	}

	int width = tempImage.cols;
	int height = tempImage.rows;
	int count = types ? width : height;
	int num = types ? height : width;
	vector<int> projVec;
	projVec.assign(num, 0);

	for (int row = 0; row < height; row++)
	{
		for (int col = 0; col < width; col++)
		{
			projVec[(types ? row : col)] += tempImage.at<uchar>(row, col);
		}
	}

	for (int idx = 0; idx < num; idx++)
	{
		projVec[idx] = projVec[idx] / count;
	}

	Mat projImage(count, num, CV_8U, Scalar::all(255));

	for (int idx = 0; idx < num; idx++)
	{
		cv::line(projImage, cv::Point(idx, count - projVec[idx]), cv::Point(idx, count - 1), cv::Scalar::all(0));
	}

	char name[30];
	sprintf_s(name, "name_%d", types);
	namedWindow(name, 0);
	imshow(name, projImage);
	return projVec;
}

int detect_point(const vector<int>& projVec, vector<int>& segPoint, int dir = 0)
{
	int num = projVec.size();
	for (int i = 0; i < num; i++)
	{
		if (!dir)
		{
			if (projVec[i] != 0 && i < num - 5)
			{
				int t = 0;
				for (t = 1; t < 5; t++)
				{
					if (projVec[i + t] == 0)
						break;
				}
				if (t == 5)
				{
					segPoint.push_back(i - 2);
					return 0;
				}
			}
		}
		else
		{
			if (projVec[num - i - 1] != 0 && i < num - 5)
			{
				int t = 0;
				for (t = 1; t < 5; t++)
				{
					if (projVec[num - i - 1 - t] == 0)
						break;
				}
				if (t == 5)
				{
					segPoint.push_back(num - i - 1 + 2);
					return 0;
				}
			}
		}
	}
	return  -1;
}

Mat segment(const Mat&srcImage, const vector<int>& horProjVec, const vector<int>& verProjVec)
{
	vector<int> segPoint;

	detect_point(horProjVec, segPoint, 0);
	detect_point(horProjVec, segPoint, 1);
	detect_point(verProjVec, segPoint, 0);
	detect_point(verProjVec, segPoint, 1);

	printf("%d, %d, %d, %d, %d, %d\n", srcImage.cols, srcImage.rows, segPoint[0], segPoint[1], segPoint[2], segPoint[3]);
	Mat segImage(srcImage, Range(segPoint[2], segPoint[3]), Range(segPoint[0], segPoint[1]));

	return segImage;
}


int mergeHei(vector<int>& patHeight, vector<Point>& posY, const int maxW, const int idx, int type)
{
	int threh = maxW;
	if (type)
		threh = maxW * 0.6;
	if (patHeight[idx] <  threh)
	{
		printf("idx:::::%d\n", idx);
		if (idx == posY.size() - 1)
		{
			posY[idx - 1].y = posY[idx].y;
			patHeight[idx - 1] = posY[idx - 1].y - posY[idx - 1].x;
			patHeight.erase(patHeight.begin() + idx);
			posY.erase(posY.begin() + idx);
			return 0;
		}
		else
		{
			posY[idx].y = posY[idx + 1].y;
			patHeight[idx] = posY[idx].y - posY[idx].x;
			patHeight.erase(patHeight.begin() + idx + 1);
			posY.erase(posY.begin() + idx + 1);
			mergeHei(patHeight, posY, maxW, idx, type);
		}
	}
	else
	{
		return 0;
	}
}

int recognition(const Mat& singMahjong)
{
	char name[50];
	char* mahjong[34] = {"一条", "二条","三条","四条","五条","六条","七条","八条","九条",\
		"一筒", "二筒", "三筒", "四筒", "五筒", "六筒", "七筒", "八筒", "九筒",\
		"一万", "二万", "三万", "四万", "五万", "六万", "七万", "八万", "九万",\
		"中", "发", "白", "东", "南", "西", "北"};
	vector<int> disL1;
	Mat standImg;
	for (size_t i = 0; i < 34; i++)
	{
		sprintf_s(name, "test5\\标号麻将图像\\%d.jpg", i + 1);
		standImg = imread(name, 0);
		threshold(standImg, standImg, 150, 255, THRESH_BINARY);
		copyMakeBorder(standImg, standImg, 6, 6, 9, 9, BORDER_CONSTANT, Scalar::all(255));
		//	imshow(name, standImg);
		int dif = INT_MAX;
		for (size_t j = 0; j < 18; j++)
		{
			for (int m = 0; m < 12; m++)
			{
				int tL1 = 0;
				for (size_t n = 0; n < singMahjong.rows; n++)
				{
					for (size_t q = 0; q < singMahjong.cols; q++)
					{
						tL1 += abs(singMahjong.at<uchar>(n, q) - standImg.at<uchar>(n + j, q + m));
					}
				}
				if (tL1 < dif)
					dif = tL1;
			}
		}
		disL1.push_back(dif);
		//		printf("dif: %d\n", dif);
	}

	int  minIdx = 0;
	for (int i = 1; i < disL1.size(); i++)
	{
		if (disL1[i] < disL1[minIdx])
			minIdx = i;
	}

	printf("recognition_result: %d__%s\n", minIdx + 1, mahjong[minIdx]);
	return minIdx + 1;
}

void rotate(const Mat& srcImage, Mat& dstImage)
{
	Mat gray, binImg;

	cvtColor(srcImage, gray, COLOR_RGB2GRAY);
//	imshow("灰度图", gray);

	threshold(gray, binImg, 150, 200, THRESH_OTSU);
//	imshow("二值化", binImg);

	vector<vector<Point> > contours;
	vector<Rect> boundRect(contours.size());
	
	findContours(binImg, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE); //找轮廓
	cout << "contours.size: " << contours.size() << endl;
	for (int i = 0; i < contours.size(); i++)
	{
		//需要获取的坐标  
		CvPoint2D32f rectpoint[4];
		CvBox2D rect = minAreaRect(Mat(contours[i]));

		cvBoxPoints(rect, rectpoint); //获取4个顶点坐标  
									  //与水平线的角度  
		float angle = rect.angle;
	//	cout << "angle : "<< angle << endl;

		int line1 = sqrt((rectpoint[1].y - rectpoint[0].y)*(rectpoint[1].y - rectpoint[0].y) + (rectpoint[1].x - rectpoint[0].x)*(rectpoint[1].x - rectpoint[0].x));
		int line2 = sqrt((rectpoint[3].y - rectpoint[0].y)*(rectpoint[3].y - rectpoint[0].y) + (rectpoint[3].x - rectpoint[0].x)*(rectpoint[3].x - rectpoint[0].x));
		//rectangle(binImg, rectpoint[0], rectpoint[3], Scalar(255), 2);
		//面积太小的直接pass
		if (line1 * line2 < 600)
		{
			continue;
		}

		if (abs(abs(angle) - 90) < 1)
		{
			dstImage = srcImage;
			return;
		}	
		//为了让正方形横着放，所以旋转角度是不一样的。竖放的，给他加90度，翻过来  
//		if (line1 > line2)
//		{
//			angle = 90 + angle;
//		}

		//新建一个感兴趣的区域图，大小跟原图一样大  
		Mat RoiSrcImg(srcImage.rows, srcImage.cols, CV_8UC3); //注意这里必须选CV_8UC3
		RoiSrcImg.setTo(0); //颜色都设置为黑色  
							//imshow("新建的ROI", RoiSrcImg);
							//对得到的轮廓填充一下  
		drawContours(binImg, contours, -1, Scalar(255), CV_FILLED);

		//抠图到RoiSrcImg
		srcImage.copyTo(RoiSrcImg, binImg);


		//再显示一下看看，除了感兴趣的区域，其他部分都是黑色的了  
		namedWindow("RoiSrcImg", 1);
		imshow("RoiSrcImg", RoiSrcImg);

		//创建一个旋转后的图像  
		//Mat dstImage(RoiSrcImg.rows, RoiSrcImg.cols, CV_8UC1);
		dstImage.setTo(0);
		//对RoiSrcImg进行旋转  
		Point2f center = rect.center;  //中心点  
		Mat M2 = getRotationMatrix2D(center, angle, 1);//计算旋转加缩放的变换矩阵 
		printf("last_angle: %f", angle);
		warpAffine(RoiSrcImg, dstImage, M2, RoiSrcImg.size(), 1, 0, Scalar(0));//仿射变换 
//		imshow("旋转之后", dstImage);
//		imwrite("r.jpg", dstImage); //将矫正后的图片保存下来
	}
}

int a()
{
	Mat inputImage, grayImage, binaryImage, segImage;
	vector<int>  horProjVec, verProjVec;
	Mat srcImage = imread("test5\\待分割识别图像\\2.jpg");
	rotate(srcImage, inputImage);
	imshow("inputImage", inputImage);
	printf("____%d____ \n ", inputImage.channels());
	cvtColor(inputImage, grayImage, CV_BGR2GRAY);
	GaussianBlur(grayImage, grayImage, Size(5, 5), 0.2);
	threshold(grayImage, binaryImage, 150, 255, THRESH_BINARY);
	imshow("binaryImage", binaryImage);
	horProjVec = project(binaryImage, 0);
	verProjVec = project(binaryImage, 1);

	segImage = segment(grayImage, horProjVec, verProjVec);
	imshow("【分割图像】", segImage);
//	waitKey();
	threshold(segImage, binaryImage, 130, 255, THRESH_BINARY);

	Mat element = getStructuringElement(1, Size(3, 3), Point(-1, -1));
	morphologyEx(binaryImage, binaryImage, MORPH_OPEN, element);
	imshow("bbinaryIamge", binaryImage);
//	waitKey();

	horProjVec = project(binaryImage, 0);
	verProjVec = project(binaryImage, 1);

	horProjVec[0] = 255;
	horProjVec[1] = 255;
	horProjVec[horProjVec.size() - 1] = 255;
	horProjVec[horProjVec.size() - 2] = 255;
	verProjVec[0] = 255;
	verProjVec[1] = 255;
	verProjVec[verProjVec.size() - 1] = 255;
	verProjVec[verProjVec.size() - 2] = 255;

	//水平分割
	for (size_t i = 0; i < horProjVec.size(); i++)
	{
		if (horProjVec[i] > 255 * 0.9)
		{
			horProjVec[i] = 255;
		}
		else
		{
			horProjVec[i] = 0;
		}
	}

	int flag = 0, preZeroIdx = 0;
	vector<int> xProjCen;
	vector<int> patWidth;
	vector<Point> posX;
	for (int i = 0; i < horProjVec.size(); i++)
	{
		if (horProjVec[i] != 0)
		{
			//	printf("idx: %d\n", i);
			if (flag == 0 && i - preZeroIdx >= 5)
			{
				patWidth.push_back(i - preZeroIdx);
				posX.push_back(Point(preZeroIdx, i));
			}
			flag = 1;
		}
		else
		{
			if (flag == 1)
				preZeroIdx = i;
			flag = 0;
		}

	}

	int maxW = 0;
	//删除宽度小于最大值一半的检测结果
	for (int i = 0; i < patWidth.size(); i++)
	{
		if (patWidth[i] > maxW)
			maxW = patWidth[i];
	}

	if (patWidth[0] * 2 < maxW)
	{
		patWidth.erase(patWidth.begin());
		posX.erase(posX.begin());
	}
	if (patWidth[patWidth.size() - 1] * 2 < maxW)
	{
		patWidth.erase(patWidth.end() - 1);
		posX.erase(posX.end() - 1);
	}

	for (int i = 0; i < patWidth.size(); i++)
	{

		mergeHei(patWidth, posX, maxW, i, 1);
	}
	for (int i = 0; i < posX.size(); i++)
	{
		xProjCen.push_back((posX[i].x + posX[i].y) / 2);
	}
	printf("_%d \n", posX.size());


	//垂直分割
	for (size_t i = 0; i < verProjVec.size(); i++)
	{
		if (verProjVec[i] > 255 * 0.9)
		{
			verProjVec[i] = 255;
		}
		else
		{
			verProjVec[i] = 0;
		}
	}

	flag = 0;
	int preZeroIdy = 0;
	vector<int> yProjCen;
	vector<int> patHeight;
	vector<Point> posY;
	for (int i = 0; i < verProjVec.size(); i++)
	{
		if (verProjVec[i] != 0)
		{
			//			printf("idx: %d\n", i);
			if (flag == 0 && i - preZeroIdy >= 5)
			{
				patHeight.push_back(i - preZeroIdy);
				posY.push_back(Point(preZeroIdy, i));
			}
			flag = 1;
		}
		else
		{
			if (flag == 1)
				preZeroIdy = i;
			flag = 0;
		}

	}

	for (int i = 0; i < patHeight.size(); i++)
	{
		
		mergeHei(patHeight, posY, maxW, i, 0);
	}
	for (int i = 0; i < patHeight.size(); i++)
	{

		printf("size_%d_maxW_%d_ttttttttttt_%d_%d \n", patHeight.size(), maxW, posY[i].x, posY[i].y);
	}

	for (int i = 0; i < posY.size(); i++)
	{
		yProjCen.push_back((posY[i].x + posY[i].y) / 2);
	}

	int singalHeight = segImage.rows / posY.size();
	int singalWidth = segImage.cols / posX.size();
	//	printf("h: %d", singalHeight);
	vector<Mat> singMahjong;
	for (int num = 0; num < posX.size() * posY.size(); num++)
	{
		//	printf("ttttttttttt_%d_%d \n",posX.size(), posY.size());
		char name[100];
		int y_top = max(yProjCen[num / posX.size()] - singalHeight / 2, 0);
		int y_bot = min(yProjCen[num / posX.size()] + singalHeight / 2, segImage.rows);
		int x_left = max(xProjCen[num % posX.size()] - singalWidth / 2, 0);
		int x_right = min(xProjCen[num % posX.size()] + singalWidth / 2, segImage.cols);
		Mat temp = segImage(Range(y_top, y_bot), Range(x_left, x_right));
		resize(temp, temp, Size(32, 48));
		singMahjong.push_back(temp);
		sprintf_s(name, "num_%d.jpg", num);
		imwrite(name, temp);
		//	namedWindow(name);
	//		imshow(name, temp);
	//		waitKey();
	}

	for (size_t idx = 0; idx < singMahjong.size(); idx++)
	{
		threshold(singMahjong[idx], singMahjong[idx], 150, 255, THRESH_BINARY);
		for (size_t i = 0; i < singMahjong[idx].rows; i++)
		{
			for (size_t j = 0; j < singMahjong[idx].cols; j++)
			{
				//if (i == 0 || i == singMahjong[idx].rows - 1 || j == 0 || j == singMahjong[idx].cols - 1)
				if (i<3 || (singMahjong[idx].rows - i) < 3 || j < 3 || (singMahjong[idx].cols - j) < 3)
				{
					printf("%d_%d\n", i, j);
					singMahjong[idx].at<uchar>(i, j) = 255;
				}
				
			}
		}
	}
	for (size_t i = 0; i < singMahjong.size(); i++)
	{
		imshow("to_be_detImage", singMahjong[i]);
		recognition(singMahjong[i]);
		//waitKey();
	}
	waitKey();
	return 0;
}




