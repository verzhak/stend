
#ifndef MAIN_LOOP_HPP
#define MAIN_LOOP_HPP

#include "all.hpp"
#include "script_engine/script_engine.hpp"
#include "context/context.hpp"
#include "map/map.hpp"
#include "matrix/matrix.hpp"
#include "video/video.hpp"
#include "main_loop/display.hpp"
#include "stat/stat.hpp"
#include "module/interface.h"

class CMainLoop : public QObject
{
	Q_OBJECT

	bool __is_run = false;
	CContext ctx;
	CMeasure measure;
	CMatrices mtx;

	void start();

	public:

		CScriptEngine script;
		CVectorMap vector_map;

		typedef QSharedPointer<CStat> PStat;
		QMap<QString, PStat> stats;

		CMainLoop();

		inline bool is_run() { return __is_run; };
		void start(const QString src_fname, const QString src_metadata_fname, const QString dst_fname);
		void stop();

	signals:

		void __start();
		void __stop();
};

#endif

