
#include "main_loop/main_loop.hpp"

CMainLoop::CMainLoop() :
	script(ctx, measure, vector_map)
{
	CStat * obj;

#define STAT_INIT(cls)\
	throw_null(obj = new cls(measure), "Не удалось инициализировать статистику");\
	stats[obj->name_en()] = PStat(obj);

	STAT_INIT(CTimeStat);
	STAT_INIT(CMemoryStat);
	STAT_INIT(CTimeMemory2DStat);
	STAT_INIT(CTimeMemory3DStat);
	STAT_INIT(CCallStat);
	STAT_INIT(CCallByFirstParameterStat);
	STAT_INIT(CCallBySecondParameterStat);
	STAT_INIT(CCombineQualityStat);
}

void CMainLoop::start(const QString src_fname, const QString src_metadata_fname, const QString dst_fname)
{
	start();

	try
	{
		struct timespec ts_before, ts_after;
		matrix src_frame, dst_frame;
		CVideoRead src(src_fname);
		CVideoWrite dst(dst_fname, src.fps());

		display::init(25);
		script.init();
		ctx.init(src_metadata_fname, src.frame_num(), src_fname, dst_fname, vector_map.map_fname, vector_map.height_map_fname);
		measure.init();
		mtx.init();
		vector_map.init();

		while(__is_run && (src_frame = src.read()) != NULL)
		{
			ctx.next_frame();

			clock_gettime(CLOCK_REALTIME, & ts_before);
			throw_null(dst_frame = script.run(src_frame), "Не удалось прочитать результирующий кадр");
			clock_gettime(CLOCK_REALTIME, & ts_after);
			measure(ts_after.tv_sec - ts_before.tv_sec + (ts_after.tv_nsec - ts_before.tv_nsec) / 1000000000.0, mtx.memory());

			dst.write(dst_frame);
			display::show_Mat(* src_frame, * dst_frame);

			mtx.clear();
		}
	}
	catch(...)
	{
		;
	}

	vector_map.destroy();
	display::destroy();

	stop();
}

void CMainLoop::start()
{
	__is_run = true;
	emit __start();
}

void CMainLoop::stop()
{
	__is_run = false;
	emit __stop();
}

