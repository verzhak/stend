
#include "matrix/matrix.hpp"
#include "module/interface.h"

QList<CMatrix *> * p_matrices; // TODO Убрать костыль

// ############################################################################

matrix matrix_create(const unsigned height, const unsigned width, const unsigned ch_num, const int element_type)
{
	CMatrix * mtx = NULL;

	try
	{
		throw_null(mtx = new CMatrix(height, width, ch_num, element_type), "Не удалось создать матрицу");
	}
	catch(...)
	{
		matrix_delete(mtx);
		mtx = NULL;
	}

	return mtx;
}

matrix matrix_copy(matrix mtx, const int is_copy_data)
{
	CMatrix * __mtx = NULL;
	
	try
	{
		throw_null(mtx, "Исходная матрица отсутствует")
		throw_null(__mtx = new CMatrix(* mtx, is_copy_data), "Не удалось создать матрицу");
	}
	catch(...)
	{
		matrix_delete(__mtx);
		__mtx = NULL;
	}

	return __mtx;
}

matrix matrix_load_image(const char * fname)
{
	CMatrix * mtx = NULL;
	
	try
	{
		throw_null(mtx = new CMatrix(fname), "Не удалось загрузить матрицу");
	}
	catch(...)
	{
		matrix_delete(mtx);
		mtx = NULL;
	}

	return mtx;
}

int matrix_save_image(matrix mtx, const char * fname)
{
	int ret = 0;

	try
	{
		((CMatrix *) mtx)->save(fname);
	}
	catch(...)
	{
		ret = -1;
	}

	return ret;
}

int matrix_get_value(matrix mtx, const unsigned row, const unsigned column, const unsigned channel, void * value)
{
	int ret = 0;

	try
	{
		((CMatrix *) mtx)->get_value(row, column, channel, value);
	}
	catch(...)
	{
		ret = -1;
	}

	return ret;
}

int matrix_set_value(matrix mtx, const unsigned row, const unsigned column, const unsigned channel, const void * value)
{
	int ret = 0;

	try
	{
		((CMatrix *) mtx)->set_value(row, column, channel, value);
	}
	catch(...)
	{
		ret = -1;
	}

	return ret;
}

#define GET_INFO(fun, mfun, type)\
int fun(matrix mtx, type * value)\
{\
	int ret = 0;\
\
	try\
	{\
		throw_null(mtx, "Матрица отсутствует");\
		throw_null(value, "Значение отсутствует");\
		\
		* value = ((CMatrix *) mtx)->mfun();\
	}\
	catch(...)\
	{\
		ret = -1;\
	}\
\
	return ret;\
}

GET_INFO(matrix_height, height, unsigned)
GET_INFO(matrix_width, width, unsigned)
GET_INFO(matrix_number_of_channels, number_of_channels, unsigned)
GET_INFO(matrix_element_type, element_type, int)
GET_INFO(matrix_pointer_to_data, pointer_to_data, void *)

int matrix_pointer_to_row(matrix mtx, const unsigned row, void ** ptr)
{
	int ret = 0;

	try
	{
		throw_null(mtx, "Матрица отсутствует");
		throw_null(ptr, "Значение отсутствует");\

		* ptr = ((CMatrix *) mtx)->pointer_to_row(row);

	}
	catch(...)
	{
		ret = -1;
	}

	return ret;
}

int matrix_delete(matrix mtx)
{
	int ret = 0;

	try
	{
		throw_null(mtx, "Матрица отсутствует");

		delete mtx;
	}
	catch(...)
	{
		ret = -1;
	}

	return ret;
}

// ############################################################################

CMatrices::CMatrices()
{
	p_matrices = & matrices; // TODO Убрать костыль
}

CMatrices::~CMatrices()
{
	clear();
}

void CMatrices::init()
{
	clear();
}

void CMatrices::clear()
{
	QList<CMatrix *> lst = matrices;

	for(auto & mtx : lst)
		delete mtx;
}

uint64_t CMatrices::memory()
{
	uint64_t mem = 0;

	for(auto & __mtx : matrices)
		mem += __mtx->size();

	return mem;
}

// ############################################################################ 

CMatrix::CMatrix()
{
	p_matrices->append(this);
}

CMatrix::CMatrix(const unsigned height, const unsigned width, const unsigned ch_num, const int element_type)
{
	init(height, width, ch_num, element_type);

	p_matrices->append(this);
}

CMatrix::CMatrix(const char * fname)
{
	(* (Mat *) this) = imread(fname, CV_LOAD_IMAGE_ANYDEPTH | CV_LOAD_IMAGE_COLOR);

	throw_if(empty(), "Не удалось загрузить изображение");

	p_matrices->append(this);
}

CMatrix::CMatrix(const Mat & __mtx)
{
	(* (Mat *) this) = __mtx.clone();

	p_matrices->append(this);
}

CMatrix::CMatrix(const CMatrix & __mtx, const bool is_copy_data)
{
	if(is_copy_data)
		(* (Mat *) this) = __mtx.clone();
	else
		init(__mtx.height(), __mtx.width(), __mtx.number_of_channels(), __mtx.element_type());

	p_matrices->append(this);
}

CMatrix::~CMatrix()
{
	while(p_matrices->removeOne(this))
		;
}

unsigned CMatrix::depth_to_unsigned(const int depth)
{
	switch(depth)
	{
		case CV_8U:
			return UNSIGNED_INT_8_BIT_ELEMENT;
		case CV_16U:
			return UNSIGNED_INT_16_BIT_ELEMENT;
		case CV_64F:
			return DOUBLE_ELEMENT;
	}

	throw_("Некорректный тип канала");
}

void CMatrix::init(const unsigned height, const unsigned width, const unsigned ch_num, const int element_type)
{
	switch(element_type)
	{
		case UNSIGNED_INT_8_BIT_ELEMENT:
		{
			create(height, width, CV_8UC(ch_num));

			break;
		};
		case UNSIGNED_INT_16_BIT_ELEMENT:
		{
			create(height, width, CV_16UC(ch_num));

			break;
		};
		case DOUBLE_ELEMENT:
		{
			create(height, width, CV_64FC(ch_num));

			break;
		};
		default:
		{
			throw_("Некорректный тип элемента");

			break;
		}
	}

	throw_if(empty(), "Не удалось создать матрицу");
}

void CMatrix::save(const char * fname)
{
	throw_if(empty(), "Матрица не содержит данных");
	throw_null(fname, "Путь и имя результирующего файла отсутствуют");
	throw_if(! imwrite(fname, * (Mat *) this), "Не удалось сохранить изображение");
}

void CMatrix::get_value(const unsigned row, const unsigned column, const unsigned channel, void * value)
{
	switch(depth())
	{
#define GET_VALUE(type_ind, type)\
case type_ind:\
{\
	* (type *) value = * ((type *) data + step[0] * row + step[1] * column + channel);\
\
	break;\
}

		GET_VALUE(CV_8U, uint8_t)
		GET_VALUE(CV_16U, uint16_t)
		GET_VALUE(CV_64F, double)

		default:
		{
			throw_("Некорректный тип канала");
		}
	}
}

void CMatrix::set_value(const unsigned row, const unsigned column, const unsigned channel, const void * value)
{
	switch(depth())
	{
#define SET_VALUE(type_ind, type)\
case type_ind:\
{\
	* ((type *) data + step[0] * row + step[1] * column + channel) = * (type *) value;\
\
	break;\
}

		SET_VALUE(CV_8U, uint8_t)
		SET_VALUE(CV_16U, uint16_t)
		SET_VALUE(CV_64F, double)

		default:
		{
			throw_("Некорректный тип канала");
		}
	}
}

void * CMatrix::pointer_to_row(const unsigned row)
{
	void * ptr;

	switch(depth())
	{
#define GET_ROW(type_ind, type)\
case type_ind:\
{\
	ptr = (void *) ((type *) data + step[0] * row); \
	\
	break;\
}

		GET_ROW(CV_8U, uint8_t)
		GET_ROW(CV_16U, uint16_t)
		GET_ROW(CV_64F, double)

		default:
		{
			throw_("Некорректный тип канала");
		}
	}

	return ptr;
}

unsigned CMatrix::element_type() const
{
	return CMatrix::depth_to_unsigned(depth());
}

