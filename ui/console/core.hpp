
#ifndef CORE_HPP
#define CORE_HPP

#include "all.hpp"
#include "script_engine/script_engine.hpp"
#include "module/module.hpp"
#include "matrix/matrix.hpp"
#include "db/db.hpp"
#include "map/map.hpp"
#include "main_loop/main_loop.hpp"
#include "stat/stat.hpp"

class CConsole : public QObject
{
	Q_OBJECT

	QString script_fname, src_video_fname, src_metadata_fname, dst_video_fname, map_fname, height_map_fname, classifier_fname;

	public:

		CConsole(int argc, char * argv[]);

	public slots:

		void run();

	signals:

		void finished();
};

#endif

