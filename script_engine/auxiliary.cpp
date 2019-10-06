
#include "script_engine/auxiliary.hpp"

int lua_combine(lua_State * state);
int lua_combine_quality(lua_State * state);
int lua_skeleton(lua_State * state);

void auxiliary_register(lua_State * state)
{
	lua_register(state, "combine", lua_combine);
	lua_register(state, "combine_quality", lua_combine_quality);
	lua_register(state, "skeleton", lua_skeleton);
}

// ############################################################################ 

int lua_combine(lua_State * state)
{
	int ret = 1;
	matrix dst_mtx = NULL;
	Mat dst, * bg, * fg;

	try
	{
		const double coef_bg = lua_tonumber(state, 2);
		const double coef_fg = lua_tonumber(state, 4);

		throw_null(bg = (Mat *) lua_touserdata(state, 1), "Передано некорректное фоновое изображение");
		throw_null(fg = (Mat *) lua_touserdata(state, 3), "Передано некорректное изображение переднего плана");

		algo::combine(* bg, coef_bg, * fg, coef_fg, dst);

		throw_null(dst_mtx = matrix_create(dst.rows, dst.cols, dst.channels(), UNSIGNED_INT_8_BIT_ELEMENT), "Не удалось создать изображение");
		* (Mat *) dst_mtx = dst.clone();

		lua_pushlightuserdata(state, dst_mtx);
	}
	catch(...)
	{
		matrix_delete(dst_mtx);

		ret = 0;
	}

	return ret;
}

int lua_combine_quality(lua_State * state)
{
	int ret = 1;
	Mat * contour;

	try
	{
		throw_null(contour = (Mat *) lua_touserdata(state, 1), "Передано некорректное контурное изображение");
		lua_pushnumber(state, algo::combine_quality(* contour));
	}
	catch(...)
	{
		ret = 0;
	}

	return ret;
}

int lua_skeleton(lua_State * state)
{
	int ret = 1;
	matrix src, dst;

	try
	{
		throw_null(src = (matrix) lua_touserdata(state, 1), "Передано некорректное изображение");
		throw_null(dst = skeleton(src), "Не удалось расчитать скелеты областей на изображении");
		lua_pushlightuserdata(state, dst);
	}
	catch(...)
	{
		ret = 0;
	}

	return ret;
}

