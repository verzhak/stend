
#ifndef DISPLAY_HPP
#define DISPLAY_HPP

#include "all.hpp"

namespace display
{
	void init(const unsigned fps);
	void destroy();
	QString get_fname_to_save_stat();
	void show_Mat(const Mat & src, const Mat & dst);
	void show_png(const QString & name_ru, const QString & fname);
};

#endif

