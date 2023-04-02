#include "problem.h"

void detectStainOnCommonBanknote()
{

	Mat templ = imread("./Image Source/banknote.jpg", IMREAD_GRAYSCALE);
	Mat sample = imread("./Image Source/dirty_banknote.jpg", IMREAD_GRAYSCALE);

	Mat result = templ - sample;
	threshold(result, result, 30, 255, THRESH_BINARY);
	imshow("template", templ);
	imshow("sample", sample);
	imshow("result", result);
}

void detectStainOnHalfBirghtnessBanknote()
{

	Mat templ = imread("./Image Source/banknote.jpg", IMREAD_GRAYSCALE);
	Mat sample = imread("./Image Source/dirty_half_brightness_banknote.jpg", IMREAD_GRAYSCALE);

	scaleHistogram(sample, (double) getMaxPixel(templ) / getMaxPixel(sample));
	Mat result = templ - sample;
	threshold(result, result, 30, 255, THRESH_BINARY);
	//imwrite("result_brightness_1.jpg", result);
	imshow("template", templ);
	imshow("sample", sample);
	imshow("result", result);

}

void detectStainOnRotatedBanknote()
{

	Mat templ = imread("./Image Source/banknote.jpg", IMREAD_GRAYSCALE);
	Mat sample = imread("./Image Source/rotated_36_dirty_banknote.jpg", IMREAD_GRAYSCALE);
	imshow("pre_crop_sample.jpg", sample);
	sample = rotate(sample, getRotateAngle(getTopLeft(sample), getTopRight(sample)));
	sample = getTemplateArea(sample, templ);

	Mat result = templ - sample;
	result = yumiko::medianFilter(result);
	threshold(result, result, 60, 255, THRESH_BINARY);
	imshow("template", templ);
	imshow("after_crop_sample", sample);
	imshow("result", result);
}

void detectStainOnResizedBanknote()
{
	//if you want to check the case that small image upscale to large image, change large_200 to small_50 
	Mat templ = imread("./Image Source/banknote.jpg", IMREAD_GRAYSCALE);
	Mat sample = imread("./Image Source/large_200_dirty_banknote.jpg", IMREAD_GRAYSCALE);
	imshow("sample_origin", sample);

	resize(sample, sample, Size(templ.cols, templ.rows), INTER_LINEAR);
	Mat result = templ - sample;
	imshow("raw_result", result);

	result = yumiko::medianFilter(result);
	imshow("after_median", result);

	threshold(result, result, 60, 255, THRESH_BINARY);
	imshow("template", templ);
	imshow("sample_resized", sample);
	imshow("result", result);
}

void restoreIncompleteBanknote()
{

	Mat templ = imread("./Image Source/banknote.jpg", IMREAD_GRAYSCALE);
	imshow("template", templ);
	Mat sample = imread("./Image Source/torn_dirty_banknote.jpg", IMREAD_GRAYSCALE);
	imshow("sample", sample);

	Mat sample_clone = sample.clone();

	threshold(sample_clone, sample_clone, 250, 255, THRESH_BINARY_INV);

	threshold(sample, sample, 250, 0, THRESH_TOZERO_INV);

	Mat missingPieces = templ - sample_clone;
	Mat result = sample + missingPieces;
	imshow("raw_result", result);
	result = yumiko::medianFilter(result);
	imshow("after_median_result", result);
	result = yumiko::usm(result, 1, 0.5, 5);
	imshow("after_usm", result);
	// enhanced 
	result = result + (templ - result);
	imshow("result", result);
}
