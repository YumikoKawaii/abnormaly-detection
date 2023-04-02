#include "problem.h"

bool yumiko2::detectBirghtnessChange(Mat templ, Mat sample)
{

	int* template_histogram = getHistogramCount(templ);
	int* sample_histogram = getHistogramCount(sample);
	
	int count = 0;

	for (int i = 5; i < HISTOGRAM - 5; i++) {
		if (abs(template_histogram[i] - sample_histogram[i]) >= 0.02 * template_histogram[i]) count++;
	}
	
	return count >= 200;
}

int yumiko2::detectOrientation(Mat templ, Mat sample)
{
	Mat shape;
	threshold(sample, shape, 250, 255, THRESH_BINARY);
	Point2i top = getTopLeanPoint(shape);
	Point2i bot = getBotLeanPoint(shape);
	if (top.x == 0 && top.y == 0) return 0;
	return (top.y < bot.y?1:-1)*nearbyint((atan((double)abs(top.x - bot.x) / abs(top.y - bot.y)) - atan((double)templ.rows / templ.cols)) / CV_PI * 180);
}

double yumiko2::detectSizeDiff(Mat templ, Mat sample)
{
	Mat shape;
	threshold(sample, shape, 250, 255, THRESH_BINARY);
	Point2i tl = getTopLeft(shape);
	Point2i tr = getTopRight(shape);

	return (double)(tr.y - tl.y) / templ.cols;
}

bool yumiko2::detectTorn(Mat sample)
{
	Mat shape;
	threshold(sample, shape, 250, 255, THRESH_BINARY_INV);

	int count = 0;
	for (int i = 0;i < shape.rows;i++)
		for (int j = 0; j < shape.cols; j++) {
			if (shape.at<uchar>(i, j) <= 10) count++;
		}

	return count > (double) 0.2 * shape.cols * shape.rows;
}

