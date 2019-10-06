
#include "context/context.hpp"

const context context_get()
{
	CContext * p_ctx = CScriptEngine::get_context();

	return p_ctx->get();
}

int context_create_variable(const char * name, const int type)
{
	int ret = 0;
	CContext * p_ctx = CScriptEngine::get_context();

	try
	{
		// Если именнованная константа существует, то ошибки нет - просто возвращаем -1
		throw_if_wm(p_ctx->is_exists(name))

		switch(type)
		{
			#define CREATE_VARIABLE(ind, tp)\
			case ind:\
			{\
				p_ctx->create(name, tp);\
				\
				break;\
			}

			#define CREATE_VARIABLE_WITH_BAD_TYPE(ind)\
			case ind:\
			{\
				throw_("Тип не поддерживается");\
				\
				break;\
			}

			CREATE_VARIABLE(INT_TYPE, QVariant::Int)
			CREATE_VARIABLE(DOUBLE_TYPE, QVariant::Double)
			CREATE_VARIABLE(STRING_TYPE, QVariant::String)
			CREATE_VARIABLE(BOOLEAN_TYPE, QVariant::Bool)

			CREATE_VARIABLE_WITH_BAD_TYPE(MATRIX_TYPE)
			CREATE_VARIABLE_WITH_BAD_TYPE(VOID_POINTER_TYPE)
		}
	}
	catch(...)
	{
		ret = -1;
	}

	return ret;
}

int context_set_value(const char * name, const void * value, const unsigned value_size)
{
	int ret = 0;
	CContext * p_ctx = CScriptEngine::get_context();

	try
	{
		throw_if_wm(! p_ctx->is_exists(name));

		QVariant & __value = p_ctx->get(name);

		switch(__value.type())
		{
			#define SET_SIMPLE_TYPE(c_type, qt_type)\
			case QVariant::qt_type:\
			{\
				__value = QVariant(* (c_type *) value);\
			\
				break;\
			}

			SET_SIMPLE_TYPE(bool, Bool)
			SET_SIMPLE_TYPE(double, Double)
			SET_SIMPLE_TYPE(int, Int)

			case QVariant::String:
			{
				unsigned v;
				char * ptr = (char *) value;
				QString str;

				throw_if(! value_size, "В функцию передана строка размером 0 байт");

				for(v = 0; v < value_size; v++)
					str += ptr[v];

				__value = str;

				break;
			}

			default:
				throw_("Некорректный тип данных в файле метаданных");
		}
	}
	catch(...)
	{
		ret = -1;
	}

	return ret;
}

int context_get_value(const char * name, void * value, const unsigned value_size)
{
	int ret = 0;
	CContext * p_ctx = CScriptEngine::get_context();

	try
	{
		// Если именнованная константа не существует, то ошибки нет - просто возвращаем -1
		throw_if_wm(! p_ctx->is_exists(name));

		QVariant __value = p_ctx->get(name);

		switch(__value.type())
		{
			#define READ_SIMPLE_TYPE(c_type, qt_type, fun)\
			case QVariant::qt_type:\
			{\
				* (c_type *) value = __value.fun();\
			\
				break;\
			}

			READ_SIMPLE_TYPE(bool, Bool, toBool)
			READ_SIMPLE_TYPE(double, Double, toDouble)
			READ_SIMPLE_TYPE(int, Int, toInt)

			case QVariant::String:
			{
				throw_if(! value_size, "В функцию передана строка размером 0 байт");

				strncpy((char *) value, __value.toString().toLocal8Bit().constData(), value_size - 1);
				((char *) value)[value_size - 1] = '\0';

				break;
			}

			default:
				throw_("Некорректный тип данных в файле метаданных");
		}
	}
	catch(...)
	{
		ret = -1;
	}

	return ret;
}

// ############################################################################ 

CContext::CContext()
{
	CScriptEngine::p_ctx = this; // TODO Костыль
}

void CContext::init(const QString fname, const unsigned frame_num, const QString src_video_fname, const QString dst_video_fname, const QString map_fname, const QString height_map_fname)
{
	vars["x"] = QVariant((double) 0.);
	vars["y"] = QVariant((double) 0.);
	vars["h"] = QVariant((double) 0.);
	vars["course"] = QVariant((double) 0.);
	vars["roll"] = QVariant((double) 0.);
	vars["pitch"] = QVariant((double) 0.);
	vars["aspect_x"] = QVariant((double) 0.);
	vars["aspect_y"] = QVariant((double) 0.);
	vars["coord_system"] = QVariant((uint) CT_GL);

	vars["frame_ind"] = QVariant((uint) 0);
	vars["frame_num"] = QVariant((uint) frame_num);

	vars["src_video_fname"] = QVariant(src_video_fname);
	vars["dst_video_fname"] = QVariant(dst_video_fname);
	vars["map_fname"] = QVariant(map_fname);
	vars["height_map_fname"] = QVariant(height_map_fname);

	fl.reset(new QFile(fname));
	throw_if(! fl->open(QIODevice::ReadOnly | QIODevice::Text), "Не удалось открыть файл с метаданными");
	stream.setDevice(fl.data());

	throw_if(! stream.readNextStartElement(), "Не удалось прочитать открывающий тег (скорее всего, файл не содержит метаданные)");
	throw_if(stream.name() != "amv", "Открывающий тег не соответствует формату");
}

void CContext::next_frame()
{
	vars["frame_ind"] = vars["frame_ind"].toUInt() + 1;

	auto value_to_QVariant = [](QStringRef & type, QStringRef & value)
	{
		QVariant ret;

		if(type == "bool")
			ret = QVariant(value == "true");
		else if(type == "double")
			ret = QVariant(value.toDouble());
		else if(type == "int")
			ret = QVariant(value.toInt());
		else if(type == "unsigned")
			ret = QVariant(value.toUInt());
		else
			ret = QVariant(value.toString());

		return ret;
	};

	if(stream.readNextStartElement() && stream.name() == "frame")
	{
		bool is_frame = true;
		QStringRef value_name, value_type;

		while(is_frame)
		{
			const QXmlStreamReader::TokenType type = stream.readNext();
			QStringRef name = stream.name();
			QStringRef value = stream.text();
			
			switch(type)
			{
				case QXmlStreamReader::StartElement:
				{
					if(stream.isWhitespace())
						stream.skipCurrentElement();

					value_name = name;
					value_type = stream.attributes().value("type"); // If type attribute not exists, return empty string

					break;
				}
				case QXmlStreamReader::EndElement:
				{
					if(name == "frame")
						is_frame = false;

					break;
				}
				case QXmlStreamReader::Characters:
				{
					if(! stream.isWhitespace())
						vars[value_name.toString()] = value_to_QVariant(value_type, value);

					break;
				}
			}
		}
	}
}

void CContext::create(const QString vname, const QVariant::Type tp)
{
	throw_if(is_exists(vname), "Переменная в контексте уже существует");
	vars[vname] = QVariant(tp);
}

context CContext::get()
{
	#define SET_CONTEXT(var, name, fun)\
		ctx.var = get(name).fun();

	#define SET_CONTEXT_STRING(dst, src)\
		strncpy(ctx.dst, get(src).toString().toLocal8Bit().constData(), CONTEXT_STRING_MAX_SIZE);\
		ctx.dst[CONTEXT_STRING_MAX_SIZE - 1] = '\0';

	SET_CONTEXT(map_state.x, "x", toDouble)
	SET_CONTEXT(map_state.y, "y", toDouble)
	SET_CONTEXT(map_state.h, "h", toDouble)
	SET_CONTEXT(map_state.course, "course", toDouble)
	SET_CONTEXT(map_state.roll, "roll", toDouble)
	SET_CONTEXT(map_state.pitch, "pitch", toDouble)
	SET_CONTEXT(map_state.aspect_x, "aspect_x", toDouble)
	SET_CONTEXT(map_state.aspect_y, "aspect_y", toDouble)
	SET_CONTEXT(map_state.coord_system, "coord_system", toUInt)

	SET_CONTEXT(frame_ind, "frame_ind", toUInt)
	SET_CONTEXT(frame_num, "frame_num", toUInt)

	SET_CONTEXT_STRING(src_video_fname, "src_video_fname")
	SET_CONTEXT_STRING(dst_video_fname, "dst_video_fname")
	SET_CONTEXT_STRING(map_fname, "map_fname")
	SET_CONTEXT_STRING(height_map_fname, "height_map_fname")

	return & ctx;
}

QVariant & CContext::get(const QString vname)
{
	throw_if(! is_exists(vname), "Переменная в контексте не существует");

	return vars[vname];
}

QMap<QString, QVariant> CContext::get_all()
{
	return vars;
}

bool CContext::is_exists(const QString vname)
{
	return vars.count(vname);
}

