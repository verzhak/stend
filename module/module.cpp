
#include "module/module.hpp"

CModule::CModule(const QString fname)
{
	library.setFileName(fname);
	throw_if(! library.load(), "TODO");

	// ############################################################################ 
	// Функции

	throw_null(__init = (decltype(__init)) resolve("init"), "TODO");
	throw_null(__destroy = (decltype(__destroy)) resolve("destroy"), "TODO");
	throw_null(__reset = (decltype(__reset)) resolve("reset"), "TODO");
	throw_null(__get_name = (decltype(__get_name)) resolve("get_name"), "TODO");
	throw_null(__get_type = (decltype(__get_type)) resolve("get_type"), "TODO");
	throw_null(__get_value = (decltype(__get_value)) resolve("get_value"), "TODO");
	throw_null(__set_value = (decltype(__set_value)) resolve("set_value"), "TODO");
	throw_null(__run = (decltype(__run)) resolve("run"), "TODO");

	// ############################################################################ 
	// Переменные

#define LOAD_VARIABLE(var_name, fun_name)\
{\
	typedef decltype(fun_name) ** fun_type;\
	fun_type ptr;\
	throw_null(ptr = (fun_type) resolve(var_name), "TODO");\
	* ptr = & fun_name;\
}

	LOAD_VARIABLE("matrix_create", matrix_create);
	LOAD_VARIABLE("matrix_delete", matrix_delete);
	LOAD_VARIABLE("matrix_copy", matrix_copy);
	LOAD_VARIABLE("matrix_load_image", matrix_load_image);
	LOAD_VARIABLE("matrix_save_image", matrix_save_image);
	LOAD_VARIABLE("matrix_get_value", matrix_get_value);
	LOAD_VARIABLE("matrix_set_value", matrix_set_value);
	LOAD_VARIABLE("matrix_height", matrix_height);
	LOAD_VARIABLE("matrix_width", matrix_width);
	LOAD_VARIABLE("matrix_number_of_channels", matrix_number_of_channels);
	LOAD_VARIABLE("matrix_element_type", matrix_element_type);
	LOAD_VARIABLE("matrix_pointer_to_data", matrix_pointer_to_data);
	LOAD_VARIABLE("matrix_pointer_to_row", matrix_pointer_to_row);
	LOAD_VARIABLE("map_set_state", map_set_state);
	LOAD_VARIABLE("map_get_state", map_get_state);
	LOAD_VARIABLE("map_get_frame", map_get_frame);
	LOAD_VARIABLE("context_get", context_get);
	LOAD_VARIABLE("context_create_variable", context_create_variable);
	LOAD_VARIABLE("context_set_value", context_set_value);
	LOAD_VARIABLE("context_get_value", context_get_value);
	LOAD_VARIABLE("combine_quality", combine_quality);
	LOAD_VARIABLE("skeleton", skeleton);

	// ############################################################################ 
	// Инициализация модуля

	unsigned param_num, return_value_num;
	char buf[buf_size];

	throw_if((* __init)(buf, buf_size, & param_num, & return_value_num), "TODO");
	_name = buf;

	load_elem(true, param_num);
	load_elem(false, return_value_num);
}

CModule::~CModule()
{
	throw_if((* __destroy)(), "TODO");

	library.unload();
	// throw_if(! library.unload(), "TODO"); TODO Раскомментировать, но тогда - ошибка при выгрузке
}

QFunctionPointer CModule::resolve(const QString name)
{
	QFunctionPointer ptr = NULL;

	ptr = library.resolve(name.toLocal8Bit());

	// Если вдруг Borland'овский cdecl
	if(ptr == NULL)
		ptr = library.resolve((QString("_") + name).toLocal8Bit());

	throw_null(ptr, name.toLocal8Bit());

	return ptr;
}

void CModule::load_elem(const bool is_param, const unsigned num)
{
	unsigned v;
	char buf[buf_size];
	int type;
	auto & array = is_param ? _params : _return_values;

	array.clear();

	for(v = 0; v < num; v++)
	{
		throw_if((* __get_name)(is_param, v, buf, buf_size), "TODO");
		throw_if((* __get_type)(is_param, v, & type), "TODO");

		array.append(QPair<QString, int>(QObject::trUtf8(buf), type));
	}
}

void CModule::reset()
{
	throw_if((* __reset)(), "TODO");
}

int CModule::run(lua_State * state)
{
	unsigned v;

	v = 2; // Первый параметр - имя алгоритма

	for(auto & param : _params)
	{
		switch(param.second)
		{

#define SET_PARAM(MY_TYPE, CPP_TYPE, LUA_FUN)\
case MY_TYPE:\
{\
	CPP_TYPE value = (CPP_TYPE) LUA_FUN(state, v);\
	(* __set_value)(true, v - 2, & value);\
\
	break;\
}

			SET_PARAM(INT_TYPE, int, lua_tointeger);
			SET_PARAM(DOUBLE_TYPE, double, lua_tonumber);
			SET_PARAM(STRING_TYPE, char *, lua_tostring);
			SET_PARAM(BOOLEAN_TYPE, int, lua_toboolean);
			SET_PARAM(MATRIX_TYPE, matrix, lua_touserdata);
			SET_PARAM(VOID_POINTER_TYPE, void *, lua_touserdata);
		}

		v++;
	}

	throw_if((* __run)(), "TODO");

	v = 0;

	for(auto & return_value : _return_values)
	{
		switch(return_value.second)
		{

#define GET_RETURN_VALUES(MY_TYPE, CPP_TYPE, LUA_FUN)\
case MY_TYPE:\
{\
	CPP_TYPE value;\
\
	(* __get_value)(false, v, & value);\
	LUA_FUN(state, value);\
\
	break;\
}

			GET_RETURN_VALUES(INT_TYPE, int, lua_pushinteger);
			GET_RETURN_VALUES(DOUBLE_TYPE, double, lua_pushnumber);
			GET_RETURN_VALUES(STRING_TYPE, char *, lua_pushstring);
			GET_RETURN_VALUES(BOOLEAN_TYPE, int, lua_pushboolean);
			GET_RETURN_VALUES(MATRIX_TYPE, matrix, lua_pushlightuserdata);
			GET_RETURN_VALUES(VOID_POINTER_TYPE, void *, lua_pushlightuserdata);
		}

		v++;
	}

	return _return_values.size();
}

