
#ifndef MAP_HPP
#define MAP_HPP

#include "all.hpp"
#include "algo/algo.hpp"
#include "matrix/matrix.hpp"
#include "module/interface.h"
#include "script_engine/script_engine.hpp"

class CVectorMap
{
	bool is_map_init, is_need_for_map_init;
	QSharedPointer<QTemporaryFile> font_fl;

	public:

		bool is_polygon_as_a_line;
		unsigned height, width, texture_size, texture_per_row;
		QString map_fname, height_map_fname, classifier_fname;
		EMapAccuracy accuracy;
		EPalette palette;

		CVectorMap();
		~CVectorMap();

		void init();
		void destroy();
		bool is_init();
};

#endif

