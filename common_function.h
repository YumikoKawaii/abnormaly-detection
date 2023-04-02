#pragma once
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

using namespace cv;
using namespace std;

namespace yumiko {

	int getMaxPixel(Mat src);
	Mat scaleHistogram(Mat src, double rate);
	void histogramGraph(Mat img, String windowName);
	Mat segmentation(Mat src);
	Point2i getTopLeft(Mat src);
	Point2i getTopRight(Mat src);
	int getRotateAngle(Point2i tl, Point2i tr);
	Mat rotate(Mat src, double angle);
	Mat getTemplateArea(Mat src, Mat _template);
	void insertionSort(int window[]);
	Mat medianFilter(Mat src);
	Mat meanFilter(Mat src);
	Mat usm(Mat original, float radius, float amount, float thresh);
}