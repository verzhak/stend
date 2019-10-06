
#include "map/map.hpp"

ECoordType int_to_ECoordType(const int coord_system)
{
	switch(coord_system)
	{
		case CT_GL:
		{
			return ECT_GL;
		}
		case CT_GEO:
		{
			return ECT_GEO;
		}
		case CT_GAUSS_KRUEGER:
		{
			return ECT_GAUSS_KRUEGER;
		}
	}

	throw_("Некорректная система координат");
}

int map_set_state(const map_state state)
{
	int ret = 0;
	CVectorMap * p_map = CScriptEngine::get_map();

	try
	{
		throw_null(state, "Указатель на NULL");

		throw_if(! p_map->is_init(), "ВММ не инициализирована");
		throw_if(combine_set_camera(state->x, state->y, state->h, state->course, state->roll, state->pitch, int_to_ECoordType(state->coord_system)), "Ошибка при позиционировании камеры");
		throw_if(combine_set_aspect(state->aspect_x, state->aspect_y), "Ошибка при установе углов обзора камеры");
	}
	catch(...)
	{
		ret = -1;
	}

	return ret;
}

int map_get_state(map_state state)
{
	int ret = 0;
	CVectorMap * p_map = CScriptEngine::get_map();

	try
	{
		throw_null(state, "Указатель на NULL");

		throw_if(! p_map->is_init(), "ВММ не инициализирована");
		throw_if(combine_get_camera(& state->x, & state->y, & state->h, & state->course, & state->roll, & state->pitch, int_to_ECoordType(state->coord_system)), "Ошибка при получении параметров позиционирования камеры");
		throw_if(combine_get_aspect(& state->aspect_x, & state->aspect_y), "Ошибка при получении значении аспектов");
	}
	catch(...)
	{
		ret = -1;
	}

	return ret;
}

matrix map_get_frame()
{
	matrix frame = NULL;
	CVectorMap * p_map = CScriptEngine::get_map();

	try
	{
		unsigned height, width, ch_num, bytes_per_ch;
		Mat * mtx;

		throw_if(! p_map->is_init(), "ВММ не инициализирована");
		throw_if(combine_get_frame_param(& height, & width, & ch_num, & bytes_per_ch), "Не удалось получить параметры отрисовки ракурса");
		
		throw_if(bytes_per_ch != 1, "Размер канала не равен 8-ми бит");
		throw_null(frame = matrix_create(height, width, ch_num, UNSIGNED_INT_8_BIT_ELEMENT), "Не удалось создать изображение для ракурса");

		mtx = (Mat *) frame;
		throw_if(combine_redraw_read_frame(mtx->data), "Ошибка при получении ракурса");
	}
	catch(...)
	{
		matrix_delete(frame);
		frame = NULL;
	}

	return frame;
}

// ############################################################################ 

CVectorMap::CVectorMap()
{
	CScriptEngine::p_map = this; // TODO Костыль
	height = 1024;
	width = 1024;
	texture_size = 1024;
	texture_per_row = 4;
	accuracy = EMA_HIGH;
	palette = EP_THIN_LINE;
	is_polygon_as_a_line = true;
	map_fname = "";
	height_map_fname = "";
	classifier_fname = "";

	is_need_for_map_init = false;
	is_map_init = false;
}

CVectorMap::~CVectorMap()
{
	destroy();
}

void CVectorMap::init()
{
	throw_if(! height || height % 2, "Высота ракурса в пикселях должна быть положительной и кратной двум");
	throw_if(! width || width % 2, "Ширина ракурса в пикселях должна быть положительной и кратной двум");
	throw_if(! texture_size || texture_size % 2 || texture_size > 4096, "Размер текстуры меньше либо равен нулю, или размер текстуры не кратен двум, или размер текстуры превышает 4096 пикселей по стороне");
	throw_if(! texture_per_row, "Некорректное количество текстур в строке");

	is_need_for_map_init = ! (map_fname.isEmpty() || height_map_fname.isEmpty());
	is_map_init = false;
}

void CVectorMap::destroy()
{
	if(is_map_init)
	{
		throw_if(combine_destroy(), "Не удалось удалить карту")
		font_fl.clear();

		is_map_init = false;
	}
}

bool CVectorMap::is_init()
{
	if(is_need_for_map_init)
	{
		try
		{
			is_map_init = true;
			is_need_for_map_init = false;

			QFile fl(":/font");

			font_fl.reset(QTemporaryFile::createNativeFile(fl));
			throw_null(font_fl, "Не удалось скопировать файл шрифта");

			throw_if(
				combine_init
				(
					height,
					width,
					texture_size,
					texture_per_row,
					accuracy,
					palette,
					is_polygon_as_a_line,
					false,						// is_print_coord
					map_fname.toLocal8Bit(),
					height_map_fname.toLocal8Bit(),
					classifier_fname.toLocal8Bit(),
					font_fl->fileName().toLocal8Bit()
				), "Не удалось загрузить карту");

			// Первоначальная отрисовка карты содержит дополнительную инициализацию
			throw_if(combine_redraw(), "Не удалось отрисовать карту");
		}
		catch(...)
		{
			destroy();

			throw_("Не удалось инициализировать отрисовку карты");
		}
	}

	return is_map_init;
}

