
#include "algo/algo.hpp"
#include "algo/vector.hpp"

double __combine_quality(const Mat & video, const Mat & map);

double combine_quality(matrix contour)
{
	// __combine_quality сам обработает те исключения, которые можно обработать (и вернет -1)
	return algo::combine_quality(* contour);
}

double algo::combine_quality(const Mat & contour)
{
	matrix map_frame;
	Mat map_skeleton;

	throw_null(map_frame = map_get_frame(), "Не удалось получить ракурс ВММ");

	cvtColor(* map_frame, * map_frame, CV_RGB2GRAY);
	threshold(* map_frame, * map_frame, 254, 255, THRESH_BINARY_INV);
	map_skeleton = algo::skeleton(* map_frame);

	return __combine_quality(contour, map_skeleton);
}

// ############################################################################ 

void find_contours(const Mat & img, vector<CVector> & G, const double lambda)
{
	unsigned u;
	vector<vector<Point> > I;

	findContours(img.clone(), I, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_TC89_KCOS); 
	throw_if_wm(! I.size());

	for(auto & t_I : I)
	{
		const unsigned pnt_num = t_I.size();

		for(u = 1; u < pnt_num; u++)
		{
			const CVector vct(t_I[u - 1], t_I[u]);

			if(vct.norm() >= lambda)
				G.push_back(vct);
		}
	}
}

double scale_quality(const double value)
{
	const double max_value = 100, abs_value = abs(value);

	if(abs_value > max_value)
		return 1;

	return abs_value / max_value;
}

double __combine_quality(const Mat & video, const Mat & map)
{
	const double lambda = 50;
	const double r = 50;

	// ############################################################################ 

	throw_if(video.channels() != 1, "В исходном контурном изображении больше одного канала");
	throw_if(map.channels() != 1, "В изображении ракурса ВММ больше одного канала");
	
	// ############################################################################ 

	const int threads_num = omp_get_max_threads();
	double K = 0, __K[threads_num], quality;
	unsigned v, tind, G_v_size, k_num = 0, __k_num[threads_num];
	vector<CVector> G_r, G_v;
	CContext * p_ctx;
	CMeasure * p_measure;

	throw_null(p_ctx = CScriptEngine::get_context(), "Не удалось получить описатель контекста");
	throw_null(p_measure = CScriptEngine::get_measure(), "Не удалось получить описатель аккумулятора статистики");

	try
	{
		find_contours(video, G_r, lambda);
		find_contours(map, G_v, lambda);

		G_v_size = G_v.size();

		for(tind = 0; tind < threads_num; tind++)
		{
			__K[tind] = 0;
			__k_num[tind] = 0;
		}

		#pragma omp parallel for
		for(v = 0; v < G_v_size; v++)
		{
			const CVector & ok = G_v[v];

			const CRect D = ok.rect(r);

			double min_d = DBL_MAX;
			bool is_min_exists = false;
			CVector min_ol;

			for(auto & ol : G_r)
				if(D.is_in(ol))
				{
					const double cur_d = ok.distance_muratov(ol);

					if(cur_d < min_d)
					{
						min_d = cur_d;
						min_ol = ol;
						is_min_exists = true;
					}
				}

			if(is_min_exists)
			{
				const unsigned tind = omp_get_thread_num();

				__K[tind] += min_d * cos(ok ^ min_ol);
				__k_num[tind] ++;
			}
		}

		for(tind = 0; tind < threads_num; tind++)
		{
			K += __K[tind];
			k_num += __k_num[tind];
		}

		throw_if_wm(! k_num);
		quality = scale_quality(K / k_num);
		p_measure->quality(p_ctx->get("frame_ind").toUInt(), quality);
	}
	catch(...)
	{
		quality = -1;
	}

	return quality;
}

