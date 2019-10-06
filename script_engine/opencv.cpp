
#include "script_engine/opencv.hpp"

void opencv_register(lua_State * state)
{
	lua_register(state, "threshold", lua_threshold);
	lua_register(state, "split", lua_split);
	lua_register(state, "merge", lua_merge);
	lua_register(state, "bitwise_and", lua_bitwise_and);
	
	lua_register(state, "bitwise_not", lua_bitwise_not);
	lua_register(state, "bitwise_or", lua_bitwise_or);
	lua_register(state, "bitwise_xor", lua_bitwise_xor);
	lua_register(state, "adaptiveThreshold", lua_adaptiveThreshold);
	lua_register(state, "mod", lua_abs);
	lua_register(state, "dct", lua_dct);
	lua_register(state, "idct", lua_idct);
	lua_register(state, "dft", lua_dft);	
	lua_register(state, "idft", lua_idft);
	lua_register(state, "approxPolyDP", lua_approxPolyDP);
	lua_register(state, "arcLength", lua_arcLength);
	lua_register(state, "bilateralFilter", lua_bilateralFilter);
	
	lua_register(state, "matrix_create", lua_matrix_create);
	lua_register(state, "matrix_delete", lua_matrix_delete);
	lua_register(state, "matrix_get_value", lua_matrix_get_value);
	lua_register(state, "matrix_set_value", lua_matrix_set_value);
	
	lua_register(state, "erode", lua_erode);
	lua_register(state, "dilate",lua_dilate);
	lua_register(state, "morphologyEx", lua_morphologyEx);
	
	lua_register(state, "blur", lua_blur);
	lua_register(state, "cvtColor", lua_cvtColor);
	lua_register(state, "GaussianBlur", lua_GaussianBlur);
	lua_register(state, "medianBlur", lua_medianBlur);
	lua_register(state, "Canny", lua_Canny);
	lua_register(state, "Sobel", lua_Sobel);
	lua_register(state, "Scharr", lua_Scharr);
	lua_register(state, "filter2D", lua_filter2D);
	lua_register(state, "integral", lua_integral);
	lua_register(state, "Laplacian", lua_Laplacian);
	lua_register(state, "mulSpectrums", lua_mulSpectrums);
	lua_register(state, "sepFilter2D", lua_sepFilter2D);
	lua_register(state, "setTo", lua_setTo);
	lua_register(state, "Size", lua_Size);
	lua_register(state, "imread", lua_imread);
	lua_register(state, "imwrite", lua_imwrite);
	lua_register(state, "normalize", lua_normalize);
}

// ############################################################################ 

Mat get_struct_element(lua_State * state);
int Sobel_Scharr(lua_State * state, int type);

// Пороговое преобразование
int lua_threshold(lua_State * state)
{
	int ret = 1;
	matrix dst = NULL; //вход и выход

	try
	{
		unsigned thr, type; //порог и обозначение вида преобразования
		int real_type;
		matrix src = (matrix) lua_touserdata(state, 1);

		thr = lua_tointeger(state, 2);
		type = lua_tointeger(state, 3);
		
		throw_if(thr > 255, "TODO");
		throw_null(dst = matrix_copy(src, 1), "TODO");

		switch(type)
		{
			case 0:
			{
				real_type = THRESH_BINARY;

				break;
			}
			case 1:
			{
				real_type = THRESH_BINARY_INV;

				break;
			}
			case 2:
			{
				real_type = THRESH_TRUNC;

				break;
			}
			case 3:
			{
				real_type = THRESH_TOZERO;

				break;
			}
			case 4:
			{
				real_type = THRESH_TOZERO_INV;

				break;
			}
			default:
				throw_("TODO");
		}

		threshold(* src, * dst, thr, 255, real_type);
		lua_pushlightuserdata(state, dst);
	}
	catch(...)
	{
		matrix_delete(dst);

		ret = 0;
	}

	return ret;
}

// Разделение на каналы
int lua_split(lua_State * state)
{
	int ret;

	try
	{
		int v;
		vector<Mat> ch;
		matrix src = (matrix) lua_touserdata(state, 1);

		split(* src, ch);
		ret = ch.size();

		for(v = 0; v < ret; v++)
		{
			matrix mat;

			throw_null(mat = new CMatrix(ch[v]), "TODO");
			lua_pushlightuserdata(state, mat);
		}
	}
	catch(...)
	{
		ret = 0;
	}

	return ret;
}

// Объединение каналов
int lua_merge(lua_State * state)
{
	int ret = 1;
	matrix dst = NULL;

	try
	{
		vector<Mat> ch;
		matrix ch_1 = (matrix) lua_touserdata(state, 1);
		matrix ch_2 = (matrix) lua_touserdata(state, 2);
		matrix ch_3 = (matrix) lua_touserdata(state, 3);

		throw_null(dst = matrix_create(ch_1->height(), ch_1->width(), 3, UNSIGNED_INT_8_BIT_ELEMENT), "TODO");

		ch.push_back(* ch_1);
		ch.push_back(* ch_2);
		ch.push_back(* ch_3);

		merge(ch, * dst);

		lua_pushlightuserdata(state, dst);
	}
	catch(...)
	{
		matrix_delete(dst);

		ret = 0;
	}

    return ret;
}

// Поканальное побитовое И
int lua_bitwise_and(lua_State * state)
{
	int ret = 1;
	matrix dst = NULL;

	try
	{
		matrix op_1 = (matrix) lua_touserdata(state, 1);
		matrix op_2 = (matrix) lua_touserdata(state, 2);

		throw_null(dst = matrix_copy(op_1, 0), "TODO");
		bitwise_and(* op_1, * op_2, * dst);
		lua_pushlightuserdata(state, dst);
	}
	catch(...)
	{
		matrix_delete(dst);

		ret = 0;
	}

	return ret;
}

//Канальное побитовое НЕ
int lua_bitwise_not(lua_State * state)
{
	int ret = 1;
	matrix dst = NULL;

	try
	{
		matrix op_1 = (matrix) lua_touserdata(state, 1);

		throw_null(dst = matrix_copy(op_1, 0), "TODO");
		bitwise_not(* op_1, * dst);
		lua_pushlightuserdata(state, dst);
	}
	catch(...)
	{
		matrix_delete(dst);

		ret = 0;
	}

	return ret;
}

//Поканальное побитовое ИЛИ
int lua_bitwise_or(lua_State * state)
{
	int ret = 1;
	matrix dst = NULL;

	try
	{
		matrix op_1 = (matrix) lua_touserdata(state, 1);
		matrix op_2 = (matrix) lua_touserdata(state, 2);

		throw_null(dst = matrix_copy(op_1, 0), "TODO");
		bitwise_or(* op_1, * op_2, * dst);
		lua_pushlightuserdata(state, dst);
	}
	catch(...)
	{
		matrix_delete(dst);

		ret = 0;
	}

	return ret;
}

//Поканальное побитовое исключающее ИЛИ
int lua_bitwise_xor(lua_State * state)
{
	int ret = 1;
	matrix dst = NULL;

	try
	{
		matrix op_1 = (matrix) lua_touserdata(state, 1);
		matrix op_2 = (matrix) lua_touserdata(state, 2);

		throw_null(dst = matrix_copy(op_1, 0), "TODO");
		bitwise_xor(* op_1, * op_2, * dst);
		lua_pushlightuserdata(state, dst);
	}
	catch(...)
	{
		matrix_delete(dst);

		ret = 0;
	}

	return ret;
}

//Адаптивное пороговое преобразование
int lua_adaptiveThreshold(lua_State * state)
{
	int ret = 1;
	matrix dst = NULL; //вход и выход
    
	try 
	{
		const vector<int> al_lst = { CV_ADAPTIVE_THRESH_MEAN_C, CV_ADAPTIVE_THRESH_GAUSSIAN_C };
		const vector<int> type_lst = { CV_THRESH_TOZERO_INV, CV_THRESH_BINARY };
		matrix src = (matrix) lua_touserdata(state, 1);
		const unsigned thr = lua_tointeger(state, 2); // порог
		const unsigned al = lua_tointeger(state, 3); // обозначение вида преобразования
		const unsigned type = lua_tointeger(state, 4); // обозначение алгоритма преобразования
		const unsigned ok = lua_tointeger(state, 5); // размер окресности преобразования
		
		throw_if(thr > 255, "TODO");
		throw_if(al >= al_lst.size() || type >= type_lst.size(), "TODO");
		throw_null(dst = matrix_copy(src, 1), "TODO");

		adaptiveThreshold(* src, * dst, thr, al_lst[al], type_lst[type], ok, 1);

		lua_pushlightuserdata(state, dst);
	}
	catch(...)
	{
		matrix_delete(dst);

		ret = 0;
	}
    
	return ret;
}
    
//Вычисление абсолютного значения каждого элемента матрицы
int lua_abs(lua_State * state)
{
	int ret = 1;
	matrix dst = NULL;

	try
	{
		matrix op_1 = (matrix) lua_touserdata(state, 1);

		throw_null(dst = matrix_copy(op_1, 0), "TODO");
		* (Mat *) dst = abs(* op_1);
		lua_pushlightuserdata(state, dst);
	}
	catch(...)
	{
		matrix_delete(dst);

		ret = 0;
	}

	return ret;
}

//Дискретное косинусное преобразование
int lua_dct(lua_State * state)
{
	int ret = 1;
	matrix dst = NULL;

	try
	{
		Mat op_gray, op_float;
		matrix op_1 = (matrix) lua_touserdata(state, 1);

		throw_null(dst = matrix_create(op_1->rows, op_1->cols, op_1->channels(), DOUBLE_ELEMENT), "TODO");
		
		cvtColor(* op_1, op_gray, CV_RGB2GRAY);
		op_float = Mat_<float>(op_gray);
		dct(op_float, * dst, DCT_ROWS);

		lua_pushlightuserdata(state, dst);
	}
	catch(...)
	{
		matrix_delete(dst);

		ret = 0;
	}

	return ret;
}

//Обратное дискретное косинусное преобразование
int lua_idct(lua_State * state)
{
	int ret = 1;
	matrix dst = NULL;

	try
	{
		Mat res;
		matrix op_1 = (matrix) lua_touserdata(state, 1);

		throw_null(dst = matrix_copy(op_1, 0), "TODO");
		
		dct(* op_1, res, DCT_ROWS);
		* dst = Mat_<uint8_t>(res);
		
		lua_pushlightuserdata(state, dst);
	}
	catch(...)
	{
		matrix_delete(dst);

		ret = 0;
	}

	return ret;
}

//Дискретное преобразование Фурье
int lua_dft(lua_State * state)
{
	int ret = 1;
	matrix dst = NULL;

	try
	{
		Mat op_gray, I;
		matrix op_1 = (matrix) lua_touserdata(state, 1);

		throw_null(dst = matrix_create(op_1->rows, op_1->cols, 2, DOUBLE_ELEMENT), "TODO");
		
		cvtColor(* op_1, op_gray, CV_RGB2GRAY);
		vector<Mat> ch_I = { Mat_<float>(op_gray), Mat::zeros(op_gray.size(), CV_32F) };
		merge(ch_I, I);

		dft(I, * dst);

		lua_pushlightuserdata(state, dst);
	}
	catch(...)
	{
		matrix_delete(dst);

		ret = 0;
	}

	return ret;
}

//Обратное дискретное преобразование Фурье
int lua_idft(lua_State * state)
{
	int ret = 1;
	matrix dst = NULL;

	try
	{
		Mat __dst;
		vector<Mat> res;
		matrix op_1 = (matrix) lua_touserdata(state, 1);

		throw_null(dst = matrix_create(op_1->rows, op_1->cols, 1, UNSIGNED_INT_8_BIT_ELEMENT), "TODO");

		idft(* op_1, __dst, DFT_SCALE);
		split(__dst, res);
		* dst = Mat_<uint8_t>(res[0]);

		lua_pushlightuserdata(state, dst);
	}
	catch(...)
	{
		matrix_delete(dst);

		ret = 0;
	}

	return ret;
}

//Аппроксимация ломаной линией с заданной точностью
int lua_approxPolyDP(lua_State * state)
{
	int ret = 1;
	matrix dst = NULL;

	try
	{
		matrix op_1 = (matrix) lua_touserdata(state, 1);
		unsigned ep = lua_tointeger(state, 2); //точность аппраксимации
		
		throw_null(dst = matrix_copy(op_1, 0), "TODO");
		
		approxPolyDP(* op_1, * dst, ep, true);
		lua_pushlightuserdata(state, dst);
	}
	catch(...)
	{
		matrix_delete(dst);

		ret = 0;
	}

	return ret;
}

//Расчет периметра контура или длины кривой
int lua_arcLength (lua_State * state)
{
	int ret = 1;

	try
	{
		matrix op_1 = (matrix) lua_touserdata(state, 1);
		const double res = arcLength(* op_1, true);

		lua_pushnumber(state, res);
	}
	catch(...)
	{
		ret = 0;
	}

	return ret;
}

//Двусторонняя фильтрация изображения
int lua_bilateralFilter (lua_State * state)
{
	int ret = 1;
	matrix dst = NULL;

	try
	{
		matrix op_1 = (matrix) lua_touserdata(state, 1);
		const unsigned d = lua_tointeger(state, 2); // диаметр окресности каждого пикселя
		const unsigned sC = lua_tointeger(state, 3); // значение фильтра в цветовом пространстве
		const unsigned sS = lua_tointeger(state, 4); // значение фильтра в пространстве координат

		throw_null(dst = matrix_copy(op_1, 0), "TODO");
		bilateralFilter(* op_1, * dst, d, sC, sS, BORDER_DEFAULT);
		lua_pushlightuserdata(state, dst);
	}
	catch(...)
	{
		matrix_delete(dst);

		ret = 0;
	}

	return ret;
}

//Создание матрицы
int lua_matrix_create(lua_State * state)
{
	int ret = 1;
	matrix dst = NULL;
	int type[3] = { UNSIGNED_INT_8_BIT_ELEMENT, UNSIGNED_INT_16_BIT_ELEMENT, DOUBLE_ELEMENT };

	try
	{
		const unsigned h = lua_tointeger(state, 1);
		const unsigned w = lua_tointeger(state, 2);
		const unsigned ch = lua_tointeger(state, 3);
		const unsigned t = lua_tointeger(state, 4);
		
		throw_null(dst = matrix_create(h, w, ch, type[t]), "TODO");
		lua_pushlightuserdata(state, dst);
	}
	catch(...)
	{
		matrix_delete(dst);

		ret = 0;
	}

	return ret;
}

//Удаление матрицы
int lua_matrix_delete(lua_State * state)
{
	int ret = 0;
	
	try
	{
		matrix mtx = (matrix) lua_touserdata(state, 1);

		matrix_delete(mtx);
	}
	catch(...)
	{
		;
	}
	
	return ret;
}

//Получение значения элемента матрицы
int lua_matrix_get_value(lua_State * state)
{
	int ret = 1;

	try
	{
		int t;
		matrix mtx = (matrix) lua_touserdata(state, 1);
		const unsigned r = lua_tointeger(state, 2);
		const unsigned c = lua_tointeger(state, 3);
		const unsigned ch = lua_tointeger(state, 4);
		
		matrix_element_type(mtx, & t);
		
		switch(t)
		{
			#define GET_VALUE(tp_ind, tp, fn)\
			case tp_ind:\
			{\
				tp res;\
				\
				matrix_get_value(mtx, r, c, ch, (void *) & res);\
				lua_pushinteger(state, res);\
				\
				break;\
			}

			GET_VALUE(UNSIGNED_INT_8_BIT_ELEMENT, uint8_t, lua_pushinteger);
			GET_VALUE(UNSIGNED_INT_16_BIT_ELEMENT, uint16_t, lua_pushinteger);
			GET_VALUE(DOUBLE_ELEMENT, double, lua_pushnumber);
		}
		
	}
	catch(...)
	{
		ret = 0;
	}

    return ret;
  
}

//Установ значения элемента матрицы
int lua_matrix_set_value(lua_State * state)
{
	int ret = 0;

	try
	{
		int t;
		matrix mtx = (matrix) lua_touserdata(state, 1);
		const unsigned r = lua_tointeger(state, 2);
		const unsigned c = lua_tointeger(state, 3);
		const unsigned ch = lua_tointeger(state, 4);
		
		matrix_element_type(mtx, & t);
		
		switch(t)
		{
			#define SET_VALUE(tp_ind, tp, fn)\
			case tp_ind:\
			{\
				tp value;\
				\
				value = fn(state, 5);\
				matrix_set_value(mtx, r, c, ch, (void *) & value);\
				\
				break;\
			}

			SET_VALUE(UNSIGNED_INT_8_BIT_ELEMENT, uint8_t, lua_tointeger);
			SET_VALUE(UNSIGNED_INT_16_BIT_ELEMENT, uint16_t, lua_tointeger);
			SET_VALUE(DOUBLE_ELEMENT, double, lua_tonumber);
		}
	}
	catch(...)
	{
		ret = 0;
	}

    return ret;
  
}

//Размытие изображения
int lua_erode(lua_State * state)
{
	int ret = 1;
	matrix dst = NULL;

	try
	{
		matrix op_1 = (matrix) lua_touserdata(state, 1);
		
		throw_null(dst = matrix_copy(op_1, 0), "TODO");
		erode(* op_1, * dst, get_struct_element(state));
		lua_pushlightuserdata(state, dst);
	}
	catch(...)
	{
		matrix_delete(dst);

		ret = 0;
	}

	return ret;
}

//Расширение изображения
int lua_dilate(lua_State * state)
{
	int ret = 1;
	matrix dst = NULL;

	try
	{
		matrix op_1 = (matrix) lua_touserdata(state, 1);

		throw_null(dst = matrix_copy(op_1, 0), "TODO");

		dilate(* op_1, * dst, get_struct_element(state));
		lua_pushlightuserdata(state, dst);
	}
	catch(...)
	{
		matrix_delete(dst);

		ret = 0;
	}

	return ret;
}

//Расширенное морфологическое преобразование
int lua_morphologyEx(lua_State * state)
{
	int ret = 1;
	matrix dst = NULL;

	try
	{
		const vector<int> morph = { MORPH_OPEN, MORPH_CLOSE, MORPH_GRADIENT, MORPH_TOPHAT, MORPH_BLACKHAT };
		matrix op_1 = (matrix) lua_touserdata(state, 1);
		const unsigned type_morph = lua_tointeger(state, 2);

		throw_if(type_morph >= morph.size(), "TODO");

		throw_null(dst = matrix_copy(op_1, 0), "TODO");
		morphologyEx(* op_1, * dst, morph[type_morph], get_struct_element(state));
		lua_pushlightuserdata(state, dst);
	}
	catch(...)
	{
		matrix_delete(dst);

		ret = 0;
	}

	return ret;
}

Mat get_struct_element(lua_State * state)
{
	const vector<int> morph = { MORPH_RECT, MORPH_ELLIPSE, MORPH_CROSS };
	int height, width, type, number = lua_gettop(state);
	
	number = number - 2;
	type = lua_tointeger(state, number);
	number ++;
	height = lua_tointeger(state, number);
	number ++;
	width = lua_tointeger(state, number);
	
	return getStructuringElement(morph[type], Size(width, height));
}

//Размытие изображения
int lua_blur(lua_State * state)
{
	int ret = 1;
	matrix dst = NULL;

	try
	{
		matrix op_1 = (matrix) lua_touserdata(state, 1);
		const int width = lua_tointeger(state, 2);
		const int height = lua_tointeger(state, 3);

		throw_null(dst = matrix_copy(op_1, 0), "TODO");
		
		blur(* op_1, * dst, Size(width, height));
		lua_pushlightuserdata(state, dst);
	}
	catch(...)
	{
		matrix_delete(dst);

		ret = 0;
	}

	return ret;
  
}

//Преобразование изображения из одного цветового пространства в другое
int lua_cvtColor(lua_State * state)
{
	int ret = 1;
	matrix dst = NULL;

	try
	{
		const vector <int> type_color =
		{
			CV_RGB2GRAY, CV_GRAY2RGB,CV_BGR2XYZ, CV_RGB2XYZ, CV_XYZ2BGR, CV_XYZ2RGB, CV_BGR2YCrCb, CV_RGB2YCrCb, CV_YCrCb2BGR, CV_YCrCb2RGB,
			CV_BGR2HSV, CV_RGB2HSV, CV_HSV2BGR, CV_HSV2RGB, CV_BGR2HLS, CV_RGB2HLS, CV_HLS2BGR, CV_HLS2RGB, CV_BGR2Lab, CV_RGB2Lab, CV_Lab2BGR,
			CV_Lab2RGB, CV_BGR2Luv, CV_RGB2Luv, CV_Luv2BGR, CV_Luv2RGB
		};
		matrix op_1 = (matrix) lua_touserdata(state, 1);
		const unsigned type = lua_tointeger(state, 2);

		throw_if(type >= type_color.size(), "TODO");

		throw_null(dst = matrix_copy(op_1, 0), "TODO");
		cvtColor(* op_1, * dst, type_color[type]);
		lua_pushlightuserdata(state, dst);
	}
	catch(...)
	{
		matrix_delete(dst);

		ret = 0;
	}

	return ret;
}

//Размытие изображения с помощью фильтра Гаусса
int lua_GaussianBlur(lua_State * state)
{
	int ret = 1;
	matrix dst = NULL;

	try
	{
		matrix op_1 = (matrix) lua_touserdata(state, 1);
		const int width = lua_tointeger(state, 2);
		const int height = lua_tointeger(state, 3);

		throw_null(dst = matrix_copy(op_1, 0), "TODO");
		
		GaussianBlur(* op_1, * dst, Size(width, height), 0);
		lua_pushlightuserdata(state, dst);
	}
	catch(...)
	{
		matrix_delete(dst);

		ret = 0;
	}

	return ret;
  
}

//Размытие изображения при помощи медианного фильтра
int lua_medianBlur(lua_State * state)
{
	int ret = 1;
	matrix dst = NULL;

	try
	{
		matrix op_1 = (matrix) lua_touserdata(state, 1);
		const int size = lua_tointeger(state, 2);

		throw_null(dst = matrix_copy(op_1, 0), "TODO");
		medianBlur(* op_1, * dst, size);
		lua_pushlightuserdata(state, dst);
	}
	catch(...)
	{
		matrix_delete(dst);

		ret = 0;
	}

	return ret;
}

//Детектор границ
int lua_Canny(lua_State * state)
{
	int ret = 1;
	matrix dst = NULL;

	try
	{
		matrix op_1 = (matrix) lua_touserdata(state, 1);
		const double min = lua_tonumber(state, 2);
		const double max = lua_tonumber(state, 3);
		const int size = lua_tointeger(state, 4);

		throw_null(dst = matrix_copy(op_1, 0), "TODO");
		Canny(* op_1, * dst, min, max, size);
		lua_pushlightuserdata(state, dst);
	}
	catch(...)
	{
		matrix_delete(dst);

		ret = 0;
	}

	return ret;
}

// оператор Собеля
int lua_Sobel(lua_State * state)
{
	const int size = lua_tointeger(state, 4);

	return Sobel_Scharr(state, size);
}

// оператор Щарра
int lua_Scharr(lua_State * state)
{
	return Sobel_Scharr(state, CV_SCHARR);
}

int Sobel_Scharr(lua_State * state, int type)
{
	int ret = 1;
	matrix dst = NULL;
	
	try
	{
		matrix op_1 = (matrix) lua_touserdata(state, 1);
		const int xorder = lua_tointeger(state, 2);
		const int yorder = lua_tointeger(state, 3);

		throw_null(dst = matrix_copy(op_1, 0), "TODO");
		Sobel(* op_1, * dst, CV_8U, xorder, yorder, type);
		lua_pushlightuserdata(state, dst);
	}
	catch(...)
	{
		matrix_delete(dst);

		ret = 0;
	}

	return ret;
}

//Свертка изображения с ядром
int lua_filter2D(lua_State * state)
{
	int ret = 1;
	matrix dst = NULL;
	
	try
	{
		matrix op_1 = (matrix) lua_touserdata(state, 1);
		matrix op_2 = (matrix) lua_touserdata(state, 2);

		throw_null(dst = matrix_copy(op_1, 0), "TODO");
		filter2D(* op_1, * dst, CV_8U, * op_2);
		lua_pushlightuserdata(state, dst);
	}
	catch(...)
	{
		matrix_delete(dst);

		ret = 0;
	}

	return ret;
}

//Интегрирование изображения
int lua_integral(lua_State * state)
{
	int ret = 1;
	matrix dst = NULL;
	
	try
	{
		matrix op_1 = (matrix) lua_touserdata(state, 1);

		throw_null(dst = matrix_create(op_1->height() + 1, op_1->width() + 1, op_1->number_of_channels(), UNSIGNED_INT_8_BIT_ELEMENT), "TODO");
		integral(* op_1, * dst);
		lua_pushlightuserdata(state, dst);
	}
	catch(...)
	{
		matrix_delete(dst);

		ret = 0;
	}

	return ret;
}

//Лапласиан изображения
int lua_Laplacian(lua_State * state)
{
	int ret = 1;
	matrix dst = NULL;

	try
	{
		matrix op_1 = (matrix) lua_touserdata(state, 1);
		const int size = lua_tointeger(state, 2);

		throw_null(dst = matrix_copy(op_1, 0), "TODO");
		Laplacian(* op_1, * dst, size);
		lua_pushlightuserdata(state, dst);
	}
	catch(...)
	{
		matrix_delete(dst);

		ret = 0;
	}

	return ret;
}

//Поэлементное умножение двух спектров Фурье
int lua_mulSpectrums(lua_State * state)
{
	int ret = 1;
	matrix dst = NULL;

	try
	{
		matrix op_1 = (matrix) lua_touserdata(state, 1);
		matrix op_2 = (matrix) lua_touserdata(state, 2);

		throw_null(dst = matrix_copy(op_1, 0), "TODO");
		mulSpectrums(* op_1, * op_2, * dst, DFT_ROWS);
		lua_pushlightuserdata(state, dst);
	}
	catch(...)
	{
		matrix_delete(dst);

		ret = 0;
	}

	return ret;
}

//Линейная фильтрация
int lua_sepFilter2D(lua_State * state)
{
	int ret = 1;
	matrix dst = NULL;
	
	try
	{
		matrix op_1 = (matrix) lua_touserdata(state, 1);
		matrix kernelX = (matrix) lua_touserdata(state, 2);
		matrix kernelY = (matrix) lua_touserdata(state, 3);

		throw_null(dst = matrix_copy(op_1, 0), "TODO");
		sepFilter2D (* op_1, * dst, CV_8U, * kernelX, * kernelY);
		lua_pushlightuserdata(state, dst);
	}
	catch(...)
	{
		matrix_delete(dst);

		ret = 0;
	}

	return ret;
}

//Устанавливает все элементы матрицы до указанного значения
int lua_setTo(lua_State * state)
{
	int ret = 0;
	
	try
	{
		matrix op_1 = (matrix) lua_touserdata(state, 1);
		const int value = lua_tointeger(state, 2);

		op_1->setTo(value);
	}
	catch(...)
	{
		;
	}

	return ret;
}

//Получение ширины, высоты и количества каналов в матрице
int lua_Size(lua_State * state)
{
	int ret = 3;
	
	try
	{
		matrix op_1 = (matrix) lua_touserdata(state, 1);

		lua_pushinteger(state, op_1->height());
		lua_pushinteger(state, op_1->width());
		lua_pushinteger(state, op_1->number_of_channels());
	}
	catch(...)
	{
		ret = 0;
	}

	return ret;
}

//Чтение из памяти
int lua_imread(lua_State * state)
{
	int ret = 1;
	matrix dst = NULL;
	
	try
	{
		const char * fname = (const char *) lua_tostring(state, 1);

		throw_null(dst = matrix_load_image(fname), "TODO");
		lua_pushlightuserdata(state, dst);
	}
	catch(...)
	{
		matrix_delete(dst);

		ret = 0;
	}

	return ret;
	
}

//Запись по указанному адресу
int lua_imwrite(lua_State * state)
{
	int ret = 0;
	
	try
	{
		matrix op_1 = (matrix) lua_touserdata(state, 2);
		const char * fname = (const char *) lua_tostring(state, 1);
		
		throw_if(matrix_save_image(op_1, fname), "TODO");
	}
	catch(...)
	{
		;
	}

	return ret;
}

// TODO
int lua_normalize(lua_State * state)
{
	int ret = 1;
	matrix dst = NULL;
	
	try
	{
		matrix op_1 = (matrix) lua_touserdata(state, 1);
		const int ch = op_1->channels();

		throw_null(dst = matrix_create(op_1->height(), op_1->width(), 1, op_1->element_type()), "TODO");
		
		if(ch == 2)
		{
			vector <Mat> res;

			split(* op_1, res);
			* dst = Mat_<uint8_t>(res[0]);
		}
		else
			* dst = Mat_<uint8_t>(* op_1);
		
		lua_pushlightuserdata(state, dst);
	}
	catch(...)
	{
		matrix_delete(dst);

		ret = 0;
	}

	return ret;
}

