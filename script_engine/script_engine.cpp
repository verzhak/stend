
#include "script_engine/script_engine.hpp"

int run_module(lua_State * state)
{
	QString name = (const char *) lua_tostring(state, 1);
	CScriptEngine * p_script = (CScriptEngine *) lua_touserdata(state, lua_upvalueindex(1));
	CMeasure * p_measure = (CMeasure *) lua_touserdata(state, lua_upvalueindex(3));
	
	throw_if(! p_script->modules.count(name), "Вызываемый модуль не загружен");
	p_measure->run_module(name);

	return p_script->modules[name]->run(state);
}

// ############################################################################ 

CVectorMap * CScriptEngine::p_map; // TODO Костыль
CContext * CScriptEngine::p_ctx; // TODO Костыль
CMeasure * CScriptEngine::p_measure; // TODO Костыль

CVectorMap * CScriptEngine::get_map()
{
	return CScriptEngine::p_map; // TODO Костыль
}

CContext * CScriptEngine::get_context()
{
	return CScriptEngine::p_ctx; // TODO Костыль
}

CMeasure * CScriptEngine::get_measure()
{
	return CScriptEngine::p_measure; // TODO Костыль
}

// ############################################################################ 

CScriptEngine::CScriptEngine(CContext & ctx, CMeasure & measure, CVectorMap & vector_map)
{
	throw_null(state = luaL_newstate(), "Не удалось инициализировать интерпретатор Lua");
	luaL_openlibs(state);

	opencv_register(state);		// Регистрация биндингов к OpenCV
	auxiliary_register(state);	// Регистрация биндингов к дополнительному функционалу
	context_register(state);	// Регистрация биндингов к функционалу, обрабатывающем контекст
	map_register(state);		// Регистрация биндингов к функционалу, управляющему ВММ

	// ############################################################################
	// Регистрация функции - обработчика доступа к модулям

	lua_pushlightuserdata(state, this);
	lua_pushlightuserdata(state, & ctx);
	lua_pushlightuserdata(state, & measure);
	lua_pushlightuserdata(state, & vector_map);
	lua_pushcclosure(state, run_module, 4); // TODO Отладить
	lua_setglobal(state, "algo");
}

CScriptEngine::~CScriptEngine()
{
	lua_close(state);
}

QString CScriptEngine::load_module(const QString module_fname)
{
	CModule * p_module;

	if(modules.contains(module_fname))
		return modules[module_fname]->name();

	throw_null(p_module = new CModule(module_fname), "Не удалось загрузить модуль");
	const QString module_name = p_module->name();
	modules[module_name] = QSharedPointer<CModule>(p_module);

	return module_name;
}

void CScriptEngine::unload_module(const QString module_name)
{
	modules.remove(module_name);
}

void CScriptEngine::load(const QString fname)
{
	throw_if(luaL_dofile(state, fname.toLocal8Bit().constData()) != LUA_OK, "Не удалось загрузить скрипт");
}

void CScriptEngine::init()
{
	for(auto & md : modules)
		md->reset();
}

matrix CScriptEngine::run(matrix src)
{
	matrix dst;

	lua_getglobal(state, "main");
    lua_pushlightuserdata(state, src);

	throw_if(lua_pcall(state, 1, 1, 0) != LUA_OK, "Не удалось запустить скрипт на выполнение");

	dst = (matrix) lua_touserdata(state, 1);
	lua_pop(state, 1);

	return dst;
}

