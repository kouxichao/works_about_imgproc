#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;
int g_T = 30;
std::vector< std::pair<int, int> > noises;
std::pair<int, int> cal_n(cv::Mat &img, int row, int col, int kernelSize, std::vector< std::pair<int, int> >&  points, int orir, int oric)
{
	std::vector<uchar> pixels;

	uchar med;
	for (int y = -kernelSize / 2; y <= kernelSize / 2; y++)
	{
		for (int x = -kernelSize / 2; x <= kernelSize / 2; x++)
		{
			pixels.push_back(img.at<uchar>(orir + y, oric + x));
			if (y != 0 || x != 0)
			{
				if (abs(img.at<uchar>(orir, oric) - img.at<uchar>(row + y, col + x)) < g_T)
				{
					//count++;
					points.push_back(std::make_pair(row + y, col + x));
				}
			}
		}
	}
	sort(pixels.begin(), pixels.end());

	med = pixels[kernelSize*kernelSize / 2];
	int y, x;
	for ( y = -kernelSize / 2; y <= kernelSize / 2; y++)
	{
		for ( x = -kernelSize / 2; x <= kernelSize / 2; x++)
		{
			if (img.at<uchar>(orir + y, oric + x) == med)
				break;
		}
	}
	return std::make_pair(orir + y, oric + x);
//	return med;
}

void edge_det(cv::Mat &img, int row, int col, int kernelSize, std::vector< std::pair<int, int> >&  points)
{
	for (int x = -kernelSize / 2; x <= kernelSize / 2; x++)
	{
		if (abs(img.at<uchar>(row + kernelSize / 2, col + x) - img.at<uchar>(row, col)) < g_T)
			points.push_back(std::make_pair(row + kernelSize / 2, col + x));
		if (abs(img.at<uchar>(row - kernelSize / 2, col + x) - img.at<uchar>(row, col)) < g_T)
			points.push_back(std::make_pair(row - kernelSize / 2, col + x));
		if (x != -kernelSize / 2 && x != kernelSize / 2)
		{
			if (abs(img.at<uchar>(row + x, col + kernelSize / 2) - img.at<uchar>(row, col)) < g_T)
				points.push_back(std::make_pair(row + x, col + kernelSize / 2));
			if (abs(img.at<uchar>(row + x, col - kernelSize / 2) - img.at<uchar>(row, col)) < g_T)
				points.push_back(std::make_pair(row + x, col + kernelSize / 2));
		}
	}
}

// 自适应中值滤波器
std::pair<int, int> adaptiveMedianFilter(cv::Mat &img, int row, int col, int kernelSize)
{
	std::vector< std::pair<int, int> > points;
	std::pair<int, int> medPo;

	medPo = cal_n(img, row, col, kernelSize, points, row, col);

	if (points.size() == 0)
	{
		noises.push_back(std::make_pair(row, col));
		return medPo;
	}
	else if (points.size() == 1)
	{
		points.clear();
		cal_n(img, points[0].first, points[0].second, kernelSize, points, row, col);

		if (points.size() >= 2)
		{
			points.clear();
			edge_det(img, row, col, kernelSize + 4, points);
			if (points.size() == 0)
			{
				noises.push_back(std::make_pair(row, col));
				return medPo;
			}
			else
				return std::make_pair(row, col);
		}
		else
		{
			noises.push_back(std::make_pair(row, col));
			return medPo;
		}


	}
	else if (points.size() >= 2)
	{
		points.clear();
		edge_det(img, row, col, kernelSize + 2, points);

		if (points.size() == 0)
		{
			noises.push_back(std::make_pair(row, col));
			return medPo;
		}
		else
		{
			points.clear();
			edge_det(img, row, col, kernelSize + 4, points);
			if (points.size() == 0)
			{
				noises.push_back(std::make_pair(row, col));
				return medPo;
			}
			else
				return std::make_pair(row, col);
		}
	}
//	return std::make_pair(row, col);
//	return 0;
}

int main()
{
	int kernelSize = 3;
	int maxSize = 7;
	int count_n = 0;
	cv::Mat img;
	img = cv::imread("noi.jpg");
	cv::imshow("original image", img);
	
	std::pair<int, int> temp;
	// 自适应中值滤波
	cv::Mat img1;
	// 扩展图像的边界
	cv::cvtColor(img, img1, cv::COLOR_BGR2GRAY);
	cv::copyMakeBorder(img1, img1, maxSize / 2, maxSize / 2, maxSize / 2, maxSize / 2, cv::BorderTypes::BORDER_REFLECT_101);
	// 图像循环
	
	int it = 0, rate = 0;
	for (; it < 3; it++)
	{
		if (rate < 0.2)
			g_T = g_T / 2;
		else if (0.2 <= rate <= 0.9)
			g_T = g_T / 2 + (ceil(rate * 10) - 2) * g_T / 14;
		for (int j = maxSize / 2; j < img1.rows - maxSize / 2; j++)
		{
			for (int i = maxSize / 2; i < img1.cols - maxSize / 2; i++)
			{
					temp = adaptiveMedianFilter(img1, j, i, kernelSize);
					img1.at<uchar>(j, i) = img1.at<uchar>(temp.first, temp.second);
					img.at<cv::Vec3b>(j - maxSize / 2, i - maxSize / 2) = img.at<cv::Vec3b>(temp.first - maxSize / 2, temp.second - maxSize / 2);

			}
		}
	//	if (it == 0)
		{
			for (int j = 0; j < noises.size(); j++)
			{
				if (0 <= img1.at<uchar>(noises[j].first, noises[j].second) <= 15 || 240 <= img1.at<uchar>(noises[j].first, noises[j].second) <= 255)
					rate++;
			}
			rate = rate / noises.size();
			printf("rate:%d", rate);
		}
	}

	cv::namedWindow("自适应滤波结果");
	cv::imshow("自适应滤波结果", img);
	

	cv::waitKey(0);
	cv::destroyAllWindows();

	return 0;
}
