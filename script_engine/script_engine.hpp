
#ifndef SCRIPT_ENGINE_HPP
#define SCRIPT_ENGINE_HPP

#include "all.hpp"
#include "module/module.hpp"
#include "context/context.hpp"
#include "script_engine/opencv.hpp"
#include "script_engine/auxiliary.hpp"
#include "script_engine/context.hpp"
#include "script_engine/map.hpp"
#include "matrix/matrix.hpp"
#include "stat/stat.hpp"
#include "map/map.hpp"

int run_module(lua_State * state);

class CVectorMap;
class CContext;
class CMeasure;

class CScriptEngine
{
	lua_State * state;
	QMap<QString, QSharedPointer<CModule> > modules;

	friend int run_module(lua_State * state);

	public:

		CScriptEngine(CContext & ctx, CMeasure & measure, CVectorMap & vector_map);
		~CScriptEngine();
		
		static CVectorMap * p_map; // TODO Костыль
		static CContext * p_ctx; // TODO Костыль
		static CMeasure * p_measure; // TODO Костыль

		static CVectorMap * get_map();
		static CContext * get_context();
		static CMeasure * get_measure();

		QString load_module(const QString module_fname);
        void unload_module(const QString module_name);
		inline QString get_module_fname(const QString module_name) { return modules[module_name]->fname(); };
		void init();
		void load(const QString fname);
		matrix run(matrix src);
};

#endif

