#pragma once
#include<core.hpp>
#include<highgui.hpp>
#include<imgproc.hpp>
#include<iostream>
using namespace cv;
using namespace std;
void Rotate(Mat &inputIm, Mat &tempImg, float angle)
{

	CV_Assert(!inputIm.empty());
	Mat inputImg;
	inputIm.copyTo(inputImg);

	float radian = (float)(angle / 180.0 * CV_PI);      //弧度

	int uniSize = (int)(max(inputImg.cols, inputImg.rows)* 1.414);
	int dx = (int)(uniSize - inputImg.cols) / 2;//x方向上扩展的宽度
	int dy = (int)(uniSize - inputImg.rows) / 2;//y方向上扩展的宽度

	copyMakeBorder(inputImg, tempImg, dy, dy, dx, dx, BORDER_CONSTANT);

	//旋转中心
	Point2f center((float)(tempImg.cols / 2), (float)(tempImg.rows / 2));
	Mat affine_matrix = (Mat_<float>(2, 3) << cos(-radian), sin(-radian),
		(1 - cos(-radian))*(float)center.x - sin(-radian)*(float)center.y,
		-sin(-radian), cos(-radian),
		(1 - cos(-radian))*(float)center.y + sin(-radian)*(float)center.x);

	//仿射旋转
	//warpAffine(tempImg, tempImg, affine_matrix, tempImg.size());
	Mat temp = Mat::zeros(tempImg.size(), tempImg.type());
	for (int i = 0; i < tempImg.rows; i++)
	{
		for (int j = 0; j < tempImg.cols; j++)
		{
			float x = (float)j * cos(radian) + (float)i * sin(radian) + (1 - cos(radian))*(float)center.x - sin(radian)*(float)center.y;
			float y = (float)j * (-sin(radian)) + (float)i * cos(radian) + (1 - cos(radian))*(float)center.x + sin(radian)*(float)center.y;
			int idx = (int)x, idy = (int)y;
			int idx1 = ceil(x), idy1 = ceil(y);
			if ((idx >= 0) && (idx <= tempImg.cols - 1) && (idx1 <= tempImg.cols - 1) && (idy >= 0) && (idy <= tempImg.rows - 1) && (idy1 <= tempImg.rows - 1))
			{
				float u = y - idy, v = x - idx;
				float ch = (1 - v) * ((1 - u) * tempImg.at<float>(idy, idx) + u * tempImg.at<float>(idy1, idx)) +
					v * ((1 - u) * tempImg.at<float>(idy, idx1) + u * tempImg.at<float>(idy1, idx1));

				temp.at<float>(i, j) = ch;
			}

		}
	}

	//选择后图像
	float sinVal = fabs(sin(radian));
	float cosVal = fabs(cos(radian));
	tempImg = temp;

}
