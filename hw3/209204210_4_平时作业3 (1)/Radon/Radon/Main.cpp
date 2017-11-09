#include<core.hpp>
#include<highgui.hpp>
#include<imgproc.hpp>
#include<iostream>
#include"DFT.h"
#include"rotate.h"
using namespace cv;
using namespace std;
Mat radon;
void on_mouse(int event, int x, int y, int flags, void *ustc) 
{
	if (event == CV_EVENT_LBUTTONDOWN)//左键按下  
	{
		float radius = (float)x / (float)radon.cols * (float)180;
		cout << "该点对应角度为" << radius << "°" << endl;
	}
}
int main()
{
	Mat src = imread("radon.BMP", IMREAD_GRAYSCALE);
	if (!src.data)
		return -1;

	//Laplasian Filtering
	Mat Lkernal = (Mat_<float>(3, 3) << -1, -1, -1, -1, 9, -1, -1, -1, -1);
	Mat srcEnhanced;
	filter2D(src, srcEnhanced, CV_8UC1, Lkernal);
	imshow("Laplacian filtering", srcEnhanced);

	//DFT
	Mat srcDFT = DFT(srcEnhanced);
	imshow("DFT", srcDFT);
	srcDFT.convertTo(srcDFT, CV_32FC1);
	normalize(srcDFT, srcDFT, 0, 1, NORM_MINMAX);

	//二值化
	threshold(srcDFT, srcDFT, 0.45, 1.0, THRESH_BINARY);
	imshow("DFTbinary", srcDFT);
	Mat srcT(srcDFT.rows, srcDFT.cols, srcDFT.type());

	int b = ceil(sqrt(srcDFT.rows*srcDFT.rows + srcDFT.cols*srcDFT.cols) / 2 + 0.5) * 2;
	int angle = 180;
	radon = Mat::zeros(b, angle, CV_32FC1);

	for (int k = 0; k<angle; k++)
	{
		Rotate(srcDFT, srcT, k);
		for (int i = 0; i<srcT.cols; i++)
		{
			float pixel = 0;
			for (int j = 0; j<srcT.rows; j++)
			{
				pixel = pixel + srcT.at<float>(j, i); //对一列进行求和
			}
			radon.at<float>(i, k) = pixel;
		}
	}

	normalize(radon, radon, 0, 1, NORM_MINMAX);
	imshow("srcT", srcT);
	namedWindow("radon", WINDOW_NORMAL);
	imshow("radon", radon);
	setMouseCallback("radon", on_mouse);//找到最亮点对应的角度
	waitKey();
	return 0;
}

