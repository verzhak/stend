
#ifndef CONTEXT_HPP
#define CONTEXT_HPP

#include "all.hpp"
#include "matrix/matrix.hpp"
#include "module/interface.h"
#include "map/map.hpp"
#include "script_engine/script_engine.hpp"

class CContext
{
	QSharedPointer<QFile> fl;
	QXmlStreamReader stream;
	QMap<QString, QVariant> vars;
	s_context ctx;

	public:

		CContext();

		void init(const QString fname, const unsigned frame_num, const QString src_video_fname, const QString dst_video_fname, const QString map_fname, const QString height_map_fname);
		void next_frame();
		void create(const QString vname, const QVariant::Type tp);
		context get();
		QVariant & get(const QString vname);
		QMap<QString, QVariant> get_all();
		bool is_exists(const QString vname);
};

#endif

