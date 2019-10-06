
#include "script_engine/map.hpp"

int lua_map_set_state(lua_State * state);
int lua_map_get_state(lua_State * state);
int lua_map_get_frame(lua_State * state);

void map_register(lua_State * state)
{
	lua_register(state, "map_set_state", lua_map_set_state);
	lua_register(state, "map_get_state", lua_map_get_state);
	lua_register(state, "map_get_frame", lua_map_get_frame);
}

void push_state_in_stack(lua_State * state, const s_map_state mstate, const int offset)
{
	lua_newtable(state);

#define PUSH(name, value, fun)\
	lua_pushstring(state, name);\
	fun(state, mstate.value);\
	lua_rawset(state, offset);

#define PUSH_DOUBLE(name, value) PUSH(name, value, lua_pushnumber)
#define PUSH_UNSIGNED(name, value) PUSH(name, value, lua_pushunsigned)

	PUSH_DOUBLE("x",				x);
	PUSH_DOUBLE("y",				y);
	PUSH_DOUBLE("h",				h);
	PUSH_DOUBLE("course",			course);
	PUSH_DOUBLE("roll",				roll);
	PUSH_DOUBLE("pitch",			pitch);
	PUSH_DOUBLE("aspect_x",			aspect_x);
	PUSH_DOUBLE("aspect_y",			aspect_y);
	PUSH_UNSIGNED("coord_system",	coord_system);
}

// ############################################################################ 

int lua_map_set_state(lua_State * state)
{
	s_map_state mstate;

#define POP(name, value, fun)\
	lua_pushstring(state, name);\
	lua_rawget(state, -2);\
	mstate.value = fun(state, -1);\
	lua_pop(state, 1);

#define POP_DOUBLE(name, value) POP(name, value, lua_tonumber)
#define POP_UNSIGNED(name, value) POP(name, value, lua_tounsigned)

	POP_DOUBLE("x",				x);
	POP_DOUBLE("y",				y);
	POP_DOUBLE("h",				h);
	POP_DOUBLE("course",		course);
	POP_DOUBLE("roll",			roll);
	POP_DOUBLE("pitch",			pitch);
	POP_DOUBLE("aspect_x",		aspect_x);
	POP_DOUBLE("aspect_y",		aspect_y);
	POP_UNSIGNED("coord_system",	coord_system);

	throw_if(map_set_state(& mstate), "Ошибка при изменении состояния виртуальной модели местности");
	
	return 0;
}

int lua_map_get_state(lua_State * state)
{
	s_map_state mstate;

	mstate.coord_system = lua_tounsigned(state, 1); // TODO
	throw_if(map_get_state(& mstate), "Ошибка при получении состояния виртуальной модели местности");

	push_state_in_stack(state, mstate, -3);
	
	return 1;
}

int lua_map_get_frame(lua_State * state)
{
	matrix frame;

	throw_null(frame = map_get_frame(), "Ошибка при получении ракурса виртуальной модели местности");
	lua_pushlightuserdata(state, frame);
	
	return 1;
}

