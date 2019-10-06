
#include "script_engine/context.hpp"

int lua_context_get(lua_State * state);

void context_register(lua_State * state)
{
	lua_register(state, "context_get", lua_context_get);
}

// ############################################################################ 

int lua_context_get(lua_State * state)
{
	CContext * p_ctx = CScriptEngine::get_context();
	s_map_state mstate;
	const QMap<QString, QVariant> vars = p_ctx->get_all();
	const QList<QString> names = vars.keys();
	QVariant value;

	lua_newtable(state);

	for(auto & name : names)
	{
		value = vars[name];

		lua_pushstring(state, name.toLocal8Bit().constData());

		switch(value.type())
		{
			#define PUSH(qt_type, variant_to_type, push)\
			case QVariant::qt_type:\
			{\
				push(state, value.variant_to_type());\
				\
				break;\
			}

			PUSH(Bool, toBool, lua_pushboolean)
			PUSH(Double, toDouble, lua_pushnumber)
			PUSH(Int, toInt, lua_pushinteger)
			PUSH(UInt, toUInt, lua_pushunsigned)
			PUSH(String, toString().toLocal8Bit().constData, lua_pushstring)
			
			default:
				throw_("Некорректный тип данных в файле метаданных");
		}

		lua_rawset(state, -3);
	}

	// ############################################################################ 
	
	mstate.x = vars["x"].toDouble();
	mstate.y = vars["y"].toDouble();
	mstate.h = vars["h"].toDouble();
	mstate.course = vars["course"].toDouble();
	mstate.roll = vars["roll"].toDouble();
	mstate.pitch = vars["pitch"].toDouble();
	mstate.aspect_x = vars["aspect_x"].toDouble();
	mstate.aspect_y = vars["aspect_y"].toDouble();
	mstate.coord_system = vars["coord_system"].toUInt();

	push_state_in_stack(state, mstate, -3);

	// ############################################################################ 
	
	lua_newtable(state);

#define PUSH_UNSIGNED(name, value)\
	lua_pushstring(state, name);\
	lua_pushunsigned(state, value);\
	lua_rawset(state, -3);

	PUSH_UNSIGNED("UNSIGNED_INT_8_BIT_ELEMENT", 0)
	PUSH_UNSIGNED("UNSIGNED_INT_16_BIT_ELEMENT", 2)
	PUSH_UNSIGNED("DOUBLE_ELEMENT", 1)
	PUSH_UNSIGNED("CONTEXT_STRING_MAX_SIZE", 4096)
	PUSH_UNSIGNED("INT_TYPE", 0)
	PUSH_UNSIGNED("DOUBLE_TYPE", 1)
	PUSH_UNSIGNED("STRING_TYPE", 2)
	PUSH_UNSIGNED("BOOLEAN_TYPE", 3)
	PUSH_UNSIGNED("MATRIX_TYPE", 4)
	PUSH_UNSIGNED("VOID_POINTER_TYPE", 5)
	PUSH_UNSIGNED("CT_GL", 0)
	PUSH_UNSIGNED("CT_GEO", 2)
	PUSH_UNSIGNED("CT_GAUSS_KRUEGER", 3)

	return 3;
}

