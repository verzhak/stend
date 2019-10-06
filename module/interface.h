
/*!

\file

\brief Интерфейс сторонних модулей - реализаций алгоритмов

Заголовочный файл совместим со стандартами C90 и C++03.

В пользовательских модулях необходимо реализовать следующие функции:

- init();
- destroy();
- reset();
- run();
- get_name();
- get_type();
- get_value();
- set_value().

Следующие функции являются служебными и предоставляются стендом для пользовательских модулей:

- matrix_create();
- matrix_delete();
- matrix_copy();
- matrix_load_image();
- matrix_save_image();
- matrix_get_value();
- matrix_set_value();
- matrix_height();
- matrix_width();
- matrix_number_of_channels();
- matrix_element_type();
- matrix_pointer_to_data();
- matrix_pointer_to_row();
- map_set_state();
- map_get_state();
- map_get_frame();
- context_get();
- context_create_variable();
- context_set_value();
- context_get_value();
- combine_quality();
- skeleton().

Модуль <B>должен</B> выделять в функции init() и самостоятельно очищать в функции destroy() память под свои возвращаемые значения. Память под свои параметры модуль может самостоятельно выделять, а может и не выделять. В том случае, если модуль не выделяет память под свои параметры, он <B>ни коим образом не должен</B> управлять памятью, выделенной под его параметры.

*/

#ifndef INTERFACE_HPP
#define INTERFACE_HPP

#ifndef EXPORT_VARIABLE

	/*! \brief Константа компилятора, отмечающая переменные, экспортируемые из модуля
	 *
	 * Устанавливается до подключения файла в зависимости от используемого компилятора
	 */
	#define EXPORT_VARIABLE

#endif

#ifndef EXPORT_FUNCTION

	/*! \brief Константа компилятора, отмечающая функции, экспортируемые из модуля
	 *
	 * Устанавливается до подключения файла в зависимости от используемого компилятора
	 */
	#define EXPORT_FUNCTION

#endif

#ifdef __cplusplus
extern "C"
{
#endif

	// ############################################################################ 
	//@{

	//! \name Типы элементов матриц
	
	//! \brief Целое, 8-ми битное, беззнаковое (uint8_t)
	#define UNSIGNED_INT_8_BIT_ELEMENT 0

	//! \brief Целое, 16-ти битное, беззнаковое (uint16_t)
	#define UNSIGNED_INT_16_BIT_ELEMENT 2

	//! \brief Вещественное двойной точности (double)
	#define DOUBLE_ELEMENT 1

	//@}
	// ############################################################################ 

	#ifdef SHARED_OBJECT

		//! \cond HIDDEN_SYMBOLS

		typedef void * matrix;

		//! \endcond

	#else

		//! \brief Описатель матрицы
		typedef CMatrix * matrix;

	#endif

	//! \brief Максимальный размер в байтах строк, хранящихся в контексте
	#define CONTEXT_STRING_MAX_SIZE 4096

	//! \brief Структура данных, описывающая состояние виртуальной модели местности
	struct s_map_state
	{
		//! Координата по оси OX ракурса виртуальной модели местности, соответствующего обрабатываемому кадру видеопоследовательности
		double x;

		//! Координата по оси OY ракурса виртуальной модели местности, соответствующего обрабатываемому кадру видеопоследовательности
		double y;

		//! Координата по оси OZ (высота) ракурса виртуальной модели местности, соответствующего обрабатываемому кадру видеопоследовательности
		double h;

		//! Курс (градусы) ракурса виртуальной модели местности, соответствующего обрабатываемому кадру видеопоследовательности
		double course;

		//! Крен (градусы) ракурса виртуальной модели местности, соответствующего обрабатываемому кадру видеопоследовательности
		double roll;

		//! Тангаж (градусы) ракурса виртуальной модели местности, соответствующего обрабатываемому кадру видеопоследовательности
		double pitch;

		//! Угол обзора по оси OX (градусы) ракурса виртуальной модели местности, соответствующего обрабатываемому кадру видеопоследовательности
		double aspect_x;

		//! Угол обзора по оси OY (градусы) ракурса виртуальной модели местности, соответствующего обрабатываемому кадру видеопоследовательности
		double aspect_y;

		//! Система координат
		unsigned coord_system;
	};

	//! \brief Состояние виртуальной модели местности
	typedef s_map_state * map_state;

	//! \brief Структура данных, описывающая контекст эксперимента
	struct s_context
	{
		//! Номер текущего обрабатываемого кадра в видеопоследовательности
		unsigned frame_ind;

		//! Количество кадров в видеопоследовательности
		unsigned frame_num;
		
		//! Полный путь и имя файла с исходной видеопоследовательностью
		char src_video_fname[CONTEXT_STRING_MAX_SIZE];

		//! Полный путь и имя файла, в который будет сохранена результирующая видеопоследовательность
		char dst_video_fname[CONTEXT_STRING_MAX_SIZE];

		//! Полный путь и имя файла, хранящего текущую карту местности
		char map_fname[CONTEXT_STRING_MAX_SIZE];

		//! Полный путь и имя файла, хранящего текущую матрицу высот
		char height_map_fname[CONTEXT_STRING_MAX_SIZE];

		//! Состояние виртуальной модели местности, соответствующего обрабатываемому кадру видеопоследовательности
		s_map_state map_state;
	};

	//! \brief Контекст эксперимента
	typedef s_context * context;

	// ############################################################################ 
	//@{

	//! \name Типы входных и выходных параметров алгоритма
	
	//! \brief Целое со знаком (int)
	#define INT_TYPE 0
	
	//! \brief Вещественное двойной точности (double)
	#define DOUBLE_TYPE 1

	//! \brief Строка (char *)
	#define STRING_TYPE 2

	//! \brief Булев тип (int)
	#define BOOLEAN_TYPE 3

	//! \brief Матрица (matrix)
	#define MATRIX_TYPE 4

	//! \brief Указатель на произвольную область памяти (void *)
	#define VOID_POINTER_TYPE 5

	//@}
	// ############################################################################ 
	//@{

	//! \name Системы координат
	
	//! \brief Система координат сцены
	#define CT_GL 0

	//! \brief Географическая система координат
	#define CT_GEO 2

	//! \brief Система координат Гаусса - Крюгера
	#define CT_GAUSS_KRUEGER 3

	//@}
	// ############################################################################ 

#ifdef SHARED_OBJECT

	// ############################################################################
	// Служебные функции - матрицы
	
	EXPORT_VARIABLE matrix (* matrix_create)(const unsigned height, const unsigned width, const unsigned ch_num, const int element_type);
	EXPORT_VARIABLE int (* matrix_delete)(matrix mtx);
	EXPORT_VARIABLE matrix (* matrix_copy)(matrix mtx, const int is_copy_data);
	EXPORT_VARIABLE matrix (* matrix_load_image)(const char * fname);
	EXPORT_VARIABLE int (* matrix_save_image)(matrix mtx, const char * fname);
	EXPORT_VARIABLE int (* matrix_get_value)(matrix mtx, const unsigned row, const unsigned column, const unsigned channel, void * value);
	EXPORT_VARIABLE int (* matrix_set_value)(matrix mtx, const unsigned row, const unsigned column, const unsigned channel, const void * value);
	EXPORT_VARIABLE int (* matrix_height)(matrix mtx, unsigned * value);
	EXPORT_VARIABLE int (* matrix_width)(matrix mtx, unsigned * value);
	EXPORT_VARIABLE int (* matrix_number_of_channels)(matrix mtx, unsigned * value);
	EXPORT_VARIABLE int (* matrix_element_type)(matrix mtx, int * value);
	EXPORT_VARIABLE int (* matrix_pointer_to_data)(matrix mtx, void ** ptr);
	EXPORT_VARIABLE int (* matrix_pointer_to_row)(matrix mtx, const unsigned row, void ** ptr);
	
	// ############################################################################ 
	// Служебные функции - векторные карты
	
	EXPORT_VARIABLE int (* map_set_state)(const map_state state);
	EXPORT_VARIABLE int (* map_get_state)(map_state state);
	EXPORT_VARIABLE matrix (* map_get_frame)();

	// ############################################################################ 
	// Служебные функции - контекст
	
	EXPORT_VARIABLE const context (* context_get)();
	EXPORT_VARIABLE int (* context_create_variable)(const char * name, const int type);
	EXPORT_VARIABLE int (* context_set_value)(const char * name, const void * value, const unsigned value_size);
	EXPORT_VARIABLE int (* context_get_value)(const char * name, void * value, const unsigned value_size);

	// ############################################################################ 
	// Алгоритмы

	EXPORT_VARIABLE double (* combine_quality)(matrix contour);
	EXPORT_VARIABLE matrix (* skeleton)(matrix mtx);

#else

	//! \cond HIDDEN_SYMBOLS

	#define HIDDEN __attribute__ ((visibility ("hidden")))

	//! \endcond

	// ############################################################################
	// Служебные функции - матрицы

	/*!
	
	\brief Создание матрицы

	\param height - количество строк в матрице;
	\param width - количество столбцов в матрице;
	\param ch_num - количество каналов в матрице;
	\param element_type - тип элемента матрицы.

	\return описатель созданной матрицы - в случае ее успешного создания;
	\return NULL - в случае, если матрицу создать не удалось.

	\sa UNSIGNED_INT_8_BIT_ELEMENT, UNSIGNED_INT_16_BIT_ELEMENT, DOUBLE_ELEMENT

	*/
	/*! \cond HIDDEN_SYMBOLS */ HIDDEN /*! \endcond */
	matrix matrix_create(const unsigned height, const unsigned width, const unsigned ch_num, const int element_type);

	/*!
	
	\brief Копирование матрицы

	\param mtx - копируемая матрица;
	\param is_copy_data - флаг копирования (<> 0) содержимого матрицы.

	\return описатель скопированной матрицы - в случае ее успешного копирования;
	\return NULL - в случае, если матрицу скопировать не удалось.

	*/
	/*! \cond HIDDEN_SYMBOLS */ HIDDEN /*! \endcond */
	matrix matrix_copy(matrix mtx, const int is_copy_data);

	/*!
	
	\brief Загрузка изображения

	\param fname - полный путь и имя файла с изображением.

	\return описатель созданной матрицы - в случае, если загрузка изображения прошла успешно;
	\return NULL - в случае, если загрузить изображение не удалось.

	*/
	/*! \cond HIDDEN_SYMBOLS */ HIDDEN /*! \endcond */
	matrix matrix_load_image(const char * fname);

	/*!
	
	\brief Сохранение изображения

	\param mtx - описатель матрицы;
	\param fname - полный путь и имя файла, в который будет сохранено изображение.

	\return 0 - в случае успешного завершения операции;
	\return <> 0 - в случае неудачного завершения операции.

	*/
	/*! \cond HIDDEN_SYMBOLS */ HIDDEN /*! \endcond */
	int matrix_save_image(matrix mtx, const char * fname);

	/*!
	
	\brief Получение значения элемента матрицы

	\param mtx - описатель матрицы;
	\param row - номер строки;
	\param column - номер столбца;
	\param channel - номер канала (нумерация от 0);
	\param value - указатель на переменную, в которой будет сохранено получаемое значение.

	В случае, если матрица не является изображением, то в качестве channel можно указывать 0 (индекс единственного канала матрицы).

	Тип value должен совпадать с типом элемента матрицы.

	\return 0 - в случае успешного завершения операции;
	\return <> 0 - в случае неудачного завершения операции.

	*/
	/*! \cond HIDDEN_SYMBOLS */ HIDDEN /*! \endcond */
	int matrix_get_value(matrix mtx, const unsigned row, const unsigned column, const unsigned channel, void * value);

	/*!
	
	\brief Установ значения элемента матрицы

	\param mtx - описатель матрицы;
	\param row - номер строки;
	\param column - номер столбца;
	\param channel - номер канала (нумерация от 0);
	\param value - указатель на переменную, хранящую устанавливаемое значение.

	В случае, если матрица не является изображением, то в качестве channel можно указывать 0 (индекс единственного канала матрицы).

	Тип value должен совпадать с типом элемента матрицы.

	\return 0 - в случае успешного завершения операции;
	\return <> 0 - в случае неудачного завершения операции.

	*/
	/*! \cond HIDDEN_SYMBOLS */ HIDDEN /*! \endcond */
	int matrix_set_value(matrix mtx, const unsigned row, const unsigned column, const unsigned channel, const void * value);

	/*!
	
	\brief Получение количества строк в матрице

	\param mtx - описатель матрицы;
	\param value - указатель на переменную, в которой будет возвращено количество строк в матрице.

	\return 0 - в случае успешного завершения операции;
	\return <> 0 - в случае неудачного завершения операции.

	*/
	/*! \cond HIDDEN_SYMBOLS */ HIDDEN /*! \endcond */
	int matrix_height(matrix mtx, unsigned * value);

	/*!
	
	\brief Получение количества столбцов в матрице

	\param mtx - описатель матрицы;
	\param value - указатель на переменную, в которой будет возвращено количество столбцов в матрице.

	\return 0 - в случае успешного завершения операции;
	\return <> 0 - в случае неудачного завершения операции.

	*/
	/*! \cond HIDDEN_SYMBOLS */ HIDDEN /*! \endcond */
	int matrix_width(matrix mtx, unsigned * value);

	/*!
	
	\brief Получение количества спектральных каналов в матрице

	\param mtx - описатель матрицы;
	\param value - указатель на переменную, в которой будет возвращено количество каналов в матрице.

	Данная функция используется в случае, если матрица хранит изображение.

	\return 0 - в случае успешного завершения операции;
	\return <> 0 - в случае неудачного завершения операции.

	*/
	/*! \cond HIDDEN_SYMBOLS */ HIDDEN /*! \endcond */
	int matrix_number_of_channels(matrix mtx, unsigned * value);

	/*!
	
	\brief Определение типа элементов матрицы

	\param mtx - описатель матрицы;
	\param value - указатель на переменную, в которой будет возвращен код типа элемента матрицы.

	\return 0 - в случае успешного завершения операции;
	\return <> 0 - в случае неудачного завершения операции.

	\sa UNSIGNED_INT_8_BIT_ELEMENT, UNSIGNED_INT_16_BIT_ELEMENT, DOUBLE_ELEMENT

	*/
	/*! \cond HIDDEN_SYMBOLS */ HIDDEN /*! \endcond */
	int matrix_element_type(matrix mtx, int * value);

	/*!
	
	\brief Удаление матрицы

	\param mtx - описатель матрицы.

	\return 0 - в случае успешного завершения операции;
	\return <> 0 - в случае неудачного завершения операции.

	*/
	/*! \cond HIDDEN_SYMBOLS */ HIDDEN /*! \endcond */
	int matrix_delete(matrix mtx);

	/*!
	
	\brief Получение указателя на область памяти, в которой хранится содержимое матрицы

	\param mtx - описатель матрицы;
	\param ptr - указатель на указатель, в котором будет возвращен адрес области памяти, содержащей содержимое матрицы.

	\return 0 - в случае успешного завершения операции;
	\return <> 0 - в случае неудачного завершения операции.

	*/
	/*! \cond HIDDEN_SYMBOLS */ HIDDEN /*! \endcond */
	int matrix_pointer_to_data(matrix mtx, void ** ptr);

	/*!
	
	\brief Получение указателя на область памяти, в которой хранится содержимое строки матрицы

	\param mtx - описатель матрицы;
	\param row - индекс строки матрицы;
	\param ptr - указатель на указатель, в котором будет возвращен адрес области памяти, хранящей содержимое строки матрицы.

	\return 0 - в случае успешного завершения операции;
	\return <> 0 - в случае неудачного завершения операции.

	*/
	/*! \cond HIDDEN_SYMBOLS */ HIDDEN /*! \endcond */
	int matrix_pointer_to_row(matrix mtx, const unsigned row, void ** ptr);

	// ############################################################################ 
	// Служебные функции - векторные карты

	/*!
	
	\brief Установ состояния виртуальной модели местности (координат положения камеры, углов Эйлера ее ориентации, углы обзора камеры)

	\param state - состояние виртуальной модели местности.

	\return 0 - в случае успешного завершения операции;
	\return <> 0 - в случае неудачного завершения операции.

	\sa CT_GL, CT_GEO, CT_GAUSS_KRUEGER

	*/
	/*! \cond HIDDEN_SYMBOLS */ HIDDEN /*! \endcond */
	int map_set_state(const map_state state);

	/*!
	
	\brief Получение состояния виртуальной модели местности (координат положения камеры, углов Эйлера ее ориентации, углы обзора камеры)

	\param state - указатель на переменную, в которой будет возвращено состояние виртуальной модели местности.

	\return 0 - в случае успешного завершения операции;
	\return <> 0 - в случае неудачного завершения операции.

	\sa CT_GL, CT_GEO, CT_GAUSS_KRUEGER

	*/
	/*! \cond HIDDEN_SYMBOLS */ HIDDEN /*! \endcond */
	int map_get_state(map_state state);

	/*!
	
	\brief Чтение растрового изображения текущего ракурса виртуальной модели местности

	\return описатель изображения текущего ракурса виртуальной модели местности - в случае успешного завершения операции;
	\return NULL - в случае неудачного завершения операции.

	*/
	/*! \cond HIDDEN_SYMBOLS */ HIDDEN /*! \endcond */
	matrix map_get_frame();

	// ############################################################################ 
	// Служебные функции - контекст

	/*!
	
	\brief Получение контекста эксперимента

	\return описатель контекста эксперимента - в случае успешного завершения операции;
	\return NULL - в случае неудачного завершения операции.

	*/
	/*! \cond HIDDEN_SYMBOLS */ HIDDEN /*! \endcond */
	const context context_get();

	/*!
	
	\brief Создание глобальной (разделяемой между модулями) переменной в контексте выполнения

	\param name - имя переменной;
	\param type - тип переменной (поддерживаются INT_TYPE, DOUBLE_TYPE, STRING_TYPE, BOOLEAN_TYPE).

	\return 0 - в случае успешного завершения операции;
	\return <> 0 - в случае неудачного завершения операции (например, переменная существует).

	\sa INT_TYPE, DOUBLE_TYPE, STRING_TYPE, BOOLEAN_TYPE

	*/
	/*! \cond HIDDEN_SYMBOLS */ HIDDEN /*! \endcond */
	int context_create_variable(const char * name, const int type);

	/*!
	
	\brief Установ значения переменной из контекста выполнения

	\param name - имя переменной;
	\param value - указатель на область памяти, в которой хранится значение переменной;
	\param value_size - размер в байтах области памяти, в которой хранится значение переменной (используется для строковых переменных, для прочих игнорируется).

	\return 0 - в случае успешного завершения операции;
	\return <> 0 - в случае неудачного завершения операции (например, переменная не существует).

	*/
	/*! \cond HIDDEN_SYMBOLS */ HIDDEN /*! \endcond */
	int context_set_value(const char * name, const void * value, const unsigned value_size);

	/*!
	
	\brief Получение значения переменной из контекста выполнения

	\param name - имя переменной;
	\param value - указатель на область памяти, в которой будет возвращено значение переменной;
	\param value_size - размер в байтах области памяти, в которой должно быть возвращено значение переменной (используется для строковых переменных, для прочих игнорируется).

	\return 0 - в случае успешного завершения операции;
	\return <> 0 - в случае неудачного завершения операции (например, переменная не существует).

	*/
	/*! \cond HIDDEN_SYMBOLS */ HIDDEN /*! \endcond */
	int context_get_value(const char * name, void * value, const unsigned value_size);

	// ############################################################################ 
	// Алгоритмы

	/*!
	
	\brief Оценка качества совмещения

	\param contour - контурное изображение.

	\return [0; 1] - оценка качества совмещения;
	\return < 0 - качество совмещения оценить не удалось.

	*/
	/*! \cond HIDDEN_SYMBOLS */ HIDDEN /*! \endcond */
	double combine_quality(matrix contour);

	/*!
	
	\brief Построение скелетов областей на изображении

	\param mtx - исходное изображение.

	Исходное изображение преобразуется к одноканальному бинарному изображению, в котором пиксель принимает значение 0, если на исходном изображении пиксель был заполнен черным цветом, и значение 1 во всех прочих случаях.

	\return результирующее одноканальное бинарное изображение - в случае успешного построения скелетов областей на исходном изображении;
	\return NULL - в случае неудачи.

	*/
	/*! \cond HIDDEN_SYMBOLS */ HIDDEN /*! \endcond */
	matrix skeleton(matrix mtx);

#endif

	/* ############################################################################  */
	/* Интерфейс модуля */

	/*!

	\brief Инициализация модуля

	Вызывается при загрузке модуля.

	\param module_name - строка, в которой будет возвращено название модуля;
	\param module_name_len - максимальное количество байт в названии модуля (считая конечный нулевой байт).
	\param param_num - указатель на целочисленную переменную, в которой должно быть возвращено количество параметров алгоритма, реализуемого модулем;
	\param return_value_num - указатель на целочисленную переменную, в которой должно быть возвращено количество значений, возвращаемых алгоритмом в вызывающую процедуру.

	\return 0 - в случае успешного завершения операции;
	\return <> 0 - в случае неудачного завершения операции.

	*/
	int EXPORT_FUNCTION init(char * module_name, const unsigned module_name_len, unsigned * param_num, unsigned * return_value_num);

	/*!

	\brief Деструктор модуля

	\return 0 - в случае успешного завершения операции;
	\return <> 0 - в случае неудачного завершения операции.

	*/
	int EXPORT_FUNCTION destroy();

	/*!

	\brief Инициализация внутренних элементов модуля

	Вызывается перед каждым запуском эксперимента.

	\return 0 - в случае успешного завершения операции;
	\return <> 0 - в случае неудачного завершения операции.

	*/
	int EXPORT_FUNCTION reset();

	/*!

	\brief Запуск алгоритма на выполнение

	\return 0 - в случае успешного завершения операции;
	\return <> 0 - в случае неудачного завершения операции.

	*/
	int EXPORT_FUNCTION run();

	/* ############################################################################  */
	/* Параметры и возвращаемые значения алгоритма */

	/*!

	\brief Получение имени внешней переменной алгоритма (параметра или возвращаемого значения)

	\param is_param - флаг обращения к внешней переменной алгоритма (true, если обращение производится к параметру алгоритма, false - если обращение производится к возвращаемому значению алгоритма);
	\param ind - индекс внешней переменной, считая от нуля;
	\param name - строка, в которой будет возвращено имя внешней переменной;
	\param name_len - максимальное количество байт (!) в имени внешней переменной (считая конечный нулевой байт).

	\return 0 - в случае успешного завершения операции;
	\return <> 0 - в случае неудачного завершения операции.

	*/
	int EXPORT_FUNCTION get_name(const bool is_param, const unsigned ind, char * name, const unsigned name_len);

	/*!

	\brief Получение типа внешней переменной алгоритма (параметра или возвращаемого значения)

	\param is_param - флаг обращения к внешней переменной алгоритма (true, если обращение производится к параметру алгоритма, false - если обращение производится к возвращаемому значению алгоритма);
	\param ind - индекс внешней переменной, считая от нуля;
	\param type - указатель на переменную, в которой будет возвращен индекс типа внешней переменной.

	\return 0 - в случае успешного завершения операции;
	\return <> 0 - в случае неудачного завершения операции.

	*/
	int EXPORT_FUNCTION get_type(const bool is_param, const unsigned ind, int * type);

	/*!

	\brief Получение фактического значения внешней переменной алгоритма (параметра или возвращаемого значения)

	\param is_param - флаг обращения к внешней переменной алгоритма (true, если обращение производится к параметру алгоритма, false - если обращение производится к возвращаемому значению алгоритма);
	\param ind - индекс внешней переменной алгоритма, считая от нуля;
	\param value - указатель на переменную, в которую будет сохранено фактическое значение внешней переменной алгоритма.

	\return 0 - в случае успешного завершения операции;
	\return <> 0 - в случае неудачного завершения операции.

	*/
	int EXPORT_FUNCTION get_value(const bool is_param, const unsigned ind, void * value);

	/*!

	\brief Установ фактического значения внешней переменной алгоритма (параметра или возвращаемого значения)

	\param is_param - флаг обращения к внешней переменной алгоритма (true, если обращение производится к параметру алгоритма, false - если обращение производится к возвращаемому значению алгоритма);
	\param ind - индекс внешней переменной алгоритма, считая от нуля;
	\param value - указатель на переменную, хранящую фактическое значение внешней переменной алгоритма.

	\return 0 - в случае успешного завершения операции;
	\return <> 0 - в случае неудачного завершения операции.

	*/
	int EXPORT_FUNCTION set_value(const bool is_param, const unsigned ind, const void * value);

#ifdef __cplusplus
}
#endif

#endif

