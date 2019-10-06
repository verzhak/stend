
#include "algo/algo.hpp"

Mat algo::skeleton(const Mat & src)
{
	// Гонсалес, Вудс - 11.1.5 (с. 927)

	const unsigned height = src.rows, width = src.cols, ch_num = src.channels();
	unsigned v, u, q, value, N, T, counter;
	int tv, tu;
	vector<Point> detour = { Point(0, 1), Point(0, 2), Point(1, 2), Point(2, 2), Point(2, 1), Point(2, 0), Point(1, 0), Point(0, 0), Point(0, 1) };
	Mat mask(height, width, CV_8U), wnd(3, 3, CV_8U), dst;
	Point pnt;

	if(ch_num == 1)
		dst = src.clone();
	else if(ch_num == 3)
		cvtColor(src, dst, CV_RGB2GRAY);
	else
		throw_("Некорректное количество каналов");

	do
	{
		for(q = 0, counter = 0; q < 2; q++)
		{
			mask = Mat::zeros(height, width, CV_8U);

			for(v = 0; v < height; v++)
				for(u = 0; u < width; u++)
					if(dst.at<uint8_t>(v, u))
					{
						N = T = 0;

						for(tv = -1; tv <= 1; tv++)
							for(tu = -1; tu <= 1; tu++)
							{
								pnt = Point(u + tu, v + tv);
								value = (pnt.x >= 0 && pnt.x < width && pnt.y >= 0 && pnt.y < height && dst.at<uint8_t>(pnt)) ? 1 : 0;
								wnd.at<uint8_t>(tv + 1, tu + 1) = value;

								if(! value && (tv || tu))
									N++;
							}

						for(tv = 0; tv < detour.size() - 1; tv++)
							if(wnd.at<uint8_t>(detour[tv]) == 0 && wnd.at<uint8_t>(detour[tv + 1]) == 1)
								T++;

						if(
							(N >= 2 && N <= 6)
							&&
							(T == 1)
							&&
							(
								(
									! q
									&&
									! (wnd.at<uint8_t>(0, 1) * wnd.at<uint8_t>(1, 2) * wnd.at<uint8_t>(2, 1))
									&&
									! (wnd.at<uint8_t>(1, 2) * wnd.at<uint8_t>(2, 1) * wnd.at<uint8_t>(1, 0))
								)
								||
								(
									q
									&&
									! (wnd.at<uint8_t>(0, 1) * wnd.at<uint8_t>(1, 2) * wnd.at<uint8_t>(1, 0))
									&&
									! (wnd.at<uint8_t>(0, 1) * wnd.at<uint8_t>(2, 1) * wnd.at<uint8_t>(1, 0))
								)
							)
						  )
							mask.at<uint8_t>(v, u) = 1;
					}

			for(v = 0; v < height; v++)
				for(u = 0; u < width; u++)
					if(mask.at<uint8_t>(v, u))
					{
						dst.at<uint8_t>(v, u) = 0;
						counter++;
					}
		}
	}
	while(counter);

	return dst;
}

matrix skeleton(matrix mtx)
{
	matrix dst = NULL;

	try
	{
		Mat dst_mtx = algo::skeleton(* mtx);

		throw_null(dst = matrix_create(dst_mtx.rows, dst_mtx.cols, 1, UNSIGNED_INT_8_BIT_ELEMENT), "Не удалось создать результирующую матрицу");
		* dst = dst_mtx.clone();
	}
	catch(...)
	{
		if(dst != NULL)
			matrix_delete(dst);

		dst = NULL;
	}

	return dst;
}

