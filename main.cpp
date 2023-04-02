#include <opencv2/core/core.hpp>
#include "problem.h"

using namespace yumiko2;

int main() {

	Mat templ = imread("./Image Source/banknote.jpg", IMREAD_GRAYSCALE);
	Mat sample = imread("./Image Source/small_50_dirty_2_banknote.jpg", IMREAD_GRAYSCALE);

	int angle = yumiko2::detectOrientation(templ, sample);
	if (angle != 0) {
		sample = rotate(sample, angle);
	}
	
	double rate = yumiko2::detectSizeDiff(templ, sample);
	if (rate > 1.001 || rate < 0.999) {
		resize(sample, sample, Size(sample.cols/rate, sample.rows/rate), INTER_LINEAR);
	}
	
	sample = getTemplateArea(sample, templ);

	if (detectBirghtnessChange(templ, sample)) {
		sample = scaleHistogram(sample,(double) getMaxPixel(templ) / getMaxPixel(sample));
	}

	sample = yumiko::usm(sample, 1, 0.5, 5);
	imshow("sample", sample);

	Mat diff = templ - sample;
	diff = yumiko::medianFilter(diff);
	threshold(diff, diff, 70, 255, THRESH_BINARY);
	
	imshow("template", templ);
	imshow("sample", sample);
	imshow("result", diff);

	Mat sample2 = imread("./Image Source/torn_dirty_banknote.jpg", IMREAD_GRAYSCALE);
	if (detectTorn(sample2)) {

		Mat sample2_clone = sample2.clone();
		threshold(sample2_clone, sample2_clone, 250, 255, THRESH_BINARY_INV);

		threshold(sample2, sample2, 250, 0, THRESH_TOZERO_INV);
		Mat missingPieces = templ - sample2_clone;

		Mat restore = sample2 + missingPieces;
		restore = yumiko::medianFilter(restore);
		restore = yumiko::usm(restore, 1, 0.5, 5);
		imshow("", restore);
	}

	waitKey(0);
	return 0;
}