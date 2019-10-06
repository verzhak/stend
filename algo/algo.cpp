
#include "algo/algo.hpp"

void algo::combine(const Mat & bg, const double coef_bg, const Mat & fg, const double coef_fg, Mat & dst)
{
	Mat roi_fg, roi_bg;

	auto dst_roi_init = [ & ](const Mat & small, const Mat & big, Mat & roi_small, Mat & roi_big)
	{
		const Size big_size = big.size(), small_size = small.size();
		const int min_x = (big_size.width - small_size.width) / 2;
		const int min_y = (big_size.height - small_size.height) / 2;
		const Rect roi(min_x, min_y, small_size.width, small_size.height);

		roi_small = small;
		roi_big = Mat(big, roi);
		dst = small.clone(); // TODO Неэффективно
	};

	if(fg.cols > bg.cols)
		dst_roi_init(bg, fg, roi_bg, roi_fg);
	else
		dst_roi_init(fg, bg, roi_fg, roi_bg);

	throw_null(dst.data, "Не удалось создать изображение");
	addWeighted(roi_bg, coef_bg, roi_fg, coef_fg, 0, dst);
}

