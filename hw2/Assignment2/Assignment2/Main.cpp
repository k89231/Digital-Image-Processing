#include<iostream>
#include<opencv2\core.hpp>
#include<opencv2\highgui.hpp>
#include<opencv2\imgproc.hpp>
using namespace std;
using namespace cv;
const int MaxBright = 100;
int CurrentBright = 0;
Mat RawImg;
Mat RevisedImg;

void On_TrackBar_Changed(int, void*);		//回调函数
int main()
{
	RawImg = imread("lena.jpg");
	if (!RawImg.data)
	{
		cout << "Fail to load the image";
		return -1;
	}

	int height = RawImg.rows;
	int width = RawImg.cols;
	RevisedImg = Mat::zeros(Size(width, height), RawImg.type());

	namedWindow("亮度调节", WINDOW_NORMAL);
	createTrackbar("亮度增加", "亮度调节", &CurrentBright, MaxBright, On_TrackBar_Changed);
	On_TrackBar_Changed(CurrentBright, 0);
	waitKey(0);
	return 0;
}
void On_TrackBar_Changed(int, void*)
{
	int height = RawImg.rows;
	int width = RawImg.cols;
	int channels = RawImg.channels();
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			if (channels == 1)
			{
				//灰度图
				int Ideal = RawImg.at<uchar>(i, j) + CurrentBright;
				if (Ideal > 255)
					RevisedImg.at<uchar>(i, j) = 255;
				else
					RevisedImg.at<uchar>(i, j) = RawImg.at<uchar>(i, j) + CurrentBright;
				continue;
			}
			else if (channels == 3)
			{
				for (int k = 0; k < 3; k++)
				{
					//色彩图
					int Ideal = RawImg.at<Vec3b>(i, j)[k] + CurrentBright;
					if (Ideal > 255)
						RevisedImg.at<Vec3b>(i, j)[k] = 255;
					else
						RevisedImg.at<Vec3b>(i, j)[k] = RawImg.at<Vec3b>(i, j)[k] + CurrentBright;
				}
			}
		}
	}
	imshow("亮度调节", RevisedImg);
}