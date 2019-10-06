
#ifndef ALGO_HPP
#define ALGO_HPP

#include "all.hpp"
#include "matrix/matrix.hpp"
#include "script_engine/script_engine.hpp"
#include "context/context.hpp"
#include "module/interface.h"

namespace algo
{
	void combine(const Mat & bg, const double coef_bg, const Mat & fg, const double coef_fg, Mat & dst);
	double combine_quality(const Mat & contour);

	Mat deblurring(const Mat & src);
	Mat skeleton(const Mat & src);
};

#endif

