#include "utils.h"

int* yumiko::getHistogramCount(Mat src)
{
	int histogram[yumiko::HISTOGRAM] = { 0 };

	for (int i = 0;i < src.rows;i++)
		for (int j = 0; j < src.cols; j++) {
			histogram[src.at<uchar>(i, j)]++;
		}

	return histogram;
}

int yumiko::countHorTenPixel(Mat src, int r, int c, bool v)
{
	int count = 0;
	for (int i = c - 5;i < c + 5;i++)
		if (i >= 0 && i <= src.cols) {
			if (src.at<uchar>(r, i) <= 10 && !v) count++;
			else if (src.at<uchar>(r, i) >= 100 && v) count++;
		}
	return count;
}

Point2i yumiko::getTopLeanPoint(Mat src)
{
	for (int i = 1;i < src.rows;i++)
		for (int j = 1; j < src.rows; j++) {
			if ((int) src.at<uchar>(i, j) <= 10 && countHorTenPixel(src, i , j, true) > 5)
				return(Point2i(i, j));
		}

	return Point2i(0, 0);
}

Point2i yumiko::getBotLeanPoint(Mat src)
{
	for (int i = src.rows - 1; i >= 0; i--)
		for (int j = 1; j < src.rows; j++) {
			if ((int)src.at<uchar>(i, j) <= 10 && countHorTenPixel(src, i, j, true) > 5)
				return(Point2i(i, j));
		}

	return Point2i(0, 0);
}

int yumiko::getMaxPixel(Mat src)
{
	int histogram[256] = {};
	int maxPos = 0;

	for (int i = 0; i < src.rows; i++)
		for (int j = 0; j < src.cols; j++) {
			histogram[src.at<uchar>(i, j)]++;
			if (histogram[src.at<uchar>(i, j)] > histogram[maxPos]) {
				maxPos = src.at<uchar>(i, j);
			}
		}
	return maxPos;
}

Mat yumiko::scaleHistogram(Mat src, double rate)
{
	for (int i = 0; i < src.rows; i++)
		for (int j = 0; j < src.cols; j++) {

			src.at<uchar>(i, j) = src.at<uchar>(i, j) * rate > 255 ? 255 : (src.at<uchar>(i, j) * rate < 0?0: src.at<uchar>(i, j) * rate);

		}
	return src;
}

void yumiko::histogramGraph(Mat img, String windowName)
{
	int histSize = 256;
	float range[] = { 0, 255 };
	const float* ranges[] = { range };

	MatND hist;
	calcHist(&img, 1, 0, Mat(), hist, 1, &histSize, ranges, true, false);
	int hist_w = 512; int hist_h = 400;
	int bin_w = cvRound((double)hist_w / histSize);

	Mat histImage(hist_h, hist_w, CV_8UC1, Scalar(0, 0, 0));
	normalize(hist, hist, 0, histImage.rows, NORM_MINMAX, -1, Mat());
	for (int i = 1; i < histSize; i++)
	{
		line(histImage, Point(bin_w * (i - 1), hist_h -
			cvRound(hist.at<float>(i - 1))),
			Point(bin_w * (i), hist_h - cvRound(hist.at<float>(i))),
			Scalar(255, 0, 0), 2, 8, 0);
	}
	imwrite(windowName + "_hist.jpg", histImage);
	imshow(windowName, histImage);
}

Mat yumiko::segmentation(Mat image_gray)
{
	Mat binary;
	threshold(image_gray, binary, 40, 255, THRESH_BINARY | THRESH_OTSU);

	Mat fg;
	erode(binary, fg, Mat(), Point(-1, -1), 2);

	Mat bg;
	dilate(binary, bg, Mat(), Point(-1, -1), 3);
	threshold(bg, bg, 1, 128, THRESH_BINARY_INV);

	Mat markers;
	markers = fg + bg;
	markers.convertTo(markers, CV_32S);

	Mat img_color;
	cvtColor(image_gray, img_color, COLOR_GRAY2BGR);
	watershed(img_color, markers);
	markers.convertTo(markers, CV_8U);
	threshold(markers, markers, 40, 255, THRESH_BINARY | THRESH_OTSU);
	return markers;
}

int yumiko::countHorTenPixel2(Mat src, int r, int c, bool v)
{
	int count = 0;
	for (int i = c;i < c + 10;i++)
		if (i < src.cols) {
			if (src.at<uchar>(r, i) <= 10 && !v) count++;
			else if (src.at<uchar>(r, i) >= 100 && v) count++;
		}

	return count;
}

int yumiko::countVerTenPixel(Mat src, int r, int c, bool v)
{
	int count = 0;
	for (int i = r; i < r + 10; i++)
		if (i <= src.rows) {
			if (src.at<uchar>(i, c) <= 10 && !v) count++;
			else if (src.at<uchar>(r, c) >= 100 && v) count++;
		}

	return count;
}

Point2i yumiko::getTopLeft(Mat src)
{
	for (int i = 1; i < src.rows; i++) {
		for (int j = 1; j < src.cols; j++) {
			if ((int)src.at<uchar>(i, j) <= 10 && countHorTenPixel2(src, i, j, false) >= 5 && countVerTenPixel(src, i, j, false) >= 5) {
				return Point2i(i, j);
			}
		}
	}

	return Point2i(0, 0);
}

int yumiko::countHorTenPixel3(Mat src, int r, int c, bool v)
{
	int count = 0;
	for (int i = c; i > c - 10; i--)
		if (i >= 0) {
			if (src.at<uchar>(r, i) <= 10 && !v) count++;
			else if (src.at<uchar>(r, i) >= 100 && v) count++;
		}

	return count;
}

Point2i yumiko::getTopRight(Mat src)
{
	for (int i = 1; i < src.rows; i++) {
		for (int j = src.cols - 1; j >= 0; j--) {
			if ((int)src.at<uchar>(i, j) <= 10 && countHorTenPixel3(src, i, j, false) >= 5 && countVerTenPixel(src, i, j, false) >= 5) {
				return Point2i(i, j);
			}
		}
	}

	return Point2i(0, 0);
}

int yumiko::getRotateAngle(Point2i tl, Point2i tr)
{
	int y = abs(tl.y - tr.y);
	int x = abs(tl.x - tr.x);
	return atan((double)y / x) * 180 / CV_PI;
}

Mat yumiko::rotate(Mat src, double angle)
{
	Mat dst, dst1;    
	Point2f pt(src.cols / 2., src.rows / 2.);          
	Mat r = getRotationMatrix2D(pt, angle, 1.0);      
	warpAffine(src, dst, r, Size(src.cols, src.rows));
	warpAffine(src, dst1, r, Size(src.cols, src.rows), INTER_LINEAR, BORDER_CONSTANT, Scalar(255, 255, 255));
	threshold(dst1, dst1, 250, 255, THRESH_BINARY);
	return dst + dst1;
}

Mat yumiko::getTemplateArea(Mat src, Mat _template)
{
	Point2i topLeft = getTopLeft(yumiko::segmentation(src));
	Point2i botRight = Point2i(topLeft.x + _template.rows, topLeft.y + _template.cols);

	return src(Range(topLeft.x, botRight.x), Range(topLeft.y, botRight.y));
}

void yumiko::insertionSort(int window[])
{
	int temp, i, j;
	for (i = 0; i < 9; i++) {
		temp = window[i];
		for (j = i - 1; j >= 0 && temp < window[j]; j--)
		{
			window[j + 1] = window[j];
		}
		window[j + 1] = temp;
	}
}

Mat yumiko::medianFilter(Mat src)
{
	int window[9];
	Mat dst = src.clone();

	for (int y = 0; y < src.rows; y++)
		for (int x = 0; x < src.cols; x++)
			dst.at<uchar>(y, x) = 0.0;

	for (int y = 1; y < src.rows - 1; y++)
		for (int x = 1; x < src.cols - 1; x++)
		{
			window[0] = src.at<uchar>(y - 1, x - 1);
			window[1] = src.at<uchar>(y, x - 1);
			window[2] = src.at<uchar>(y + 1, x - 1);
			window[3] = src.at<uchar>(y - 1, x);
			window[4] = src.at<uchar>(y, x);
			window[5] = src.at<uchar>(y + 1, x);
			window[6] = src.at<uchar>(y - 1, x + 1);
			window[7] = src.at<uchar>(y, x + 1);
			window[8] = src.at<uchar>(y + 1, x + 1);
			insertionSort(window);
			dst.at<uchar>(y, x) = window[4];
		}
	return dst;
}

Mat yumiko::meanFilter(Mat src)
{
	int sizeMatrix = 9;
	Mat dst = src.clone();
	for (int y = 0; y < src.rows; y++)
		for (int x = 0; x < src.cols; x++)
			dst.at<uchar>(y, x) = 0.0;

	double mean = 0.0;

	for (int y = 1; y < src.rows - 1; y++)
		for (int x = 1; x < src.cols - 1; x++)
		{
			mean = (src.at<uchar>(y - 1, x - 1)
				+ src.at<uchar>(y, x - 1)
				+ src.at<uchar>(y + 1, x - 1)
				+ src.at<uchar>(y - 1, x)
				+ src.at<uchar>(y, x)
				+ src.at<uchar>(y + 1, x)
				+ src.at<uchar>(y - 1, x + 1)
				+ src.at<uchar>(y, x + 1)
				+ src.at<uchar>(y + 1, x + 1)) /
				sizeMatrix;
			dst.at<uchar>(y, x) = mean;
		}

	return dst;
}

Mat yumiko::usm(Mat input, float radius, float amount, float thresh)
{
	Mat retbuf = input.clone();

	Mat blurred;
	cv::GaussianBlur(input, blurred, cv::Size(0, 0), radius);

	Mat unsharpMask;
	cv::subtract(input, blurred, unsharpMask);

	cv::blur(unsharpMask, unsharpMask, { 3,3 });

	for (int row = 0; row < input.rows; row++)
	{
		for (int col = 0; col < input.cols; col++)
		{

			if (norm(unsharpMask.at<uchar>(row, col)) >= thresh) {
				retbuf.at<uchar>(row, col) = input.at<uchar>(row, col) + amount * unsharpMask.at<uchar>(row, col);
			}
		}
	}

	return retbuf;

}
