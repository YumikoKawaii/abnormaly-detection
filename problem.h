#pragma once
#include "utils.h"

using namespace yumiko;

namespace yumiko2 {
	bool detectBirghtnessChange(Mat templ, Mat sample);
	int detectOrientation(Mat templ, Mat sample);
	double detectSizeDiff(Mat templ, Mat sample);
	bool detectTorn(Mat sample);
}

