#pragma once
#include<core.hpp>
#include<highgui.hpp>
#include<imgproc.hpp>
#include<iostream>
using namespace cv;
using namespace std;

Mat DFT(const Mat GrayRaw) //该份代码可在Github搜索得到，为我个人GitHub的OpenCV自学repository
{
	int m = getOptimalDFTSize(GrayRaw.rows);
	int n = getOptimalDFTSize(GrayRaw.cols);

	//Expand the GrayRaw size to m*n, using 0 to fill in the blank area
	//Only expand from the right side and the bottom side. All filled with 0
	Mat ExpandedGrayRaw;
	copyMakeBorder(GrayRaw, ExpandedGrayRaw, 0, m - GrayRaw.rows, 0, n - GrayRaw.cols, BORDER_CONSTANT, Scalar::all(0));

	//Make the size of the picture odd, use "and" to remove the 1
	ExpandedGrayRaw = ExpandedGrayRaw(Rect(0, 0, ExpandedGrayRaw.cols & -2, ExpandedGrayRaw.rows & -2));
	//R_I[0] to store the real part, R_I[1] to store the imaginary part, with only 1 channel respectively
	Mat R_I[] = { Mat_<float>(ExpandedGrayRaw), Mat::zeros(ExpandedGrayRaw.size(), CV_32F) };

	//Merge them into one Mat with 2 channels.
	//The function DFT automatically fill the first channel with real part, second channel with imaginary part
	Mat merged;
	merge(R_I, 2, merged);
	dft(merged, merged);

	//Then split the 2 channels into R_I, and calculate the magnitude of the DFT results
	split(merged, R_I);
	magnitude(R_I[0], R_I[1], R_I[0]);
	R_I[0] += Scalar::all(1);
	log(R_I[0], R_I[0]);

	//Shift the center
	int centerx = n / 2;
	int centery = m / 2;
	Mat area1 = R_I[0](Rect(0, 0, centerx, centery));
	Mat area2 = R_I[0](Rect(centerx, 0, centerx, centery));
	Mat area3 = R_I[0](Rect(0, centery, centerx, centery));
	Mat area4 = R_I[0](Rect(centerx, centery, centerx, centery));
	Mat temp;
	area1.copyTo(temp);
	area4.copyTo(area1);
	temp.copyTo(area4);
	area2.copyTo(temp);
	area3.copyTo(area2);
	temp.copyTo(area3);

	//Normalization
	normalize(R_I[0], R_I[0], 0, 1, CV_MINMAX);
	return R_I[0];
}
