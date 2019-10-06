
#include "all.hpp"

void message_handler(QtMsgType type, const QMessageLogContext & context, const QString & msg)
{
	bool is_abort = false;
	QString __msg;
	QTextStream stream(& __msg);

	if(! msg.isEmpty())
		stream << "\n" << msg;

	stream << "\nFile " << context.file << "\nLine " << context.line << "\nFunction: " << context.function;

    switch(type)
	{
		case QtDebugMsg:
		{
			printf("[TODO] %s\n", msg.toLocal8Bit().constData());

			break;
		}
	    case QtWarningMsg:
		{
			// TODO Подавление Qt'шных сообщений
			return;
//			__msg.prepend("[ Warning ]");

//			break;
		}
		case QtCriticalMsg:
		{
			__msg.prepend("[ Critical ]");
			message(__msg);

			break;
		}
		case QtFatalMsg:
		{
			__msg.prepend("[ Fatal ]");
			message(__msg);

			is_abort = true;

			break;
		}
    }

	if(is_abort)
		abort();
}

void * alloc(const unsigned type_size, const unsigned dim, ...)
{
	void * mem = NULL;
	va_list val;
	const unsigned dim_1 = dim - 1, dim_2 = dim - 2;
	unsigned u, v, num, size, step, * dim_size;
	char * pof, * dof, * t_dof; /* Стандартом гарантируется, что sizeof(char) == 1 */

	try
	{
		throw_null(dim_size = (unsigned *) alloca(dim * sizeof(unsigned)), "Ошибка при выделении памяти");

		va_start(val, dim);

		for(u = 0, size = 0, num = 1; u < dim; u++)
		{
			num *= (dim_size[u] = va_arg(val, unsigned));
			size += (u == dim_1 ? type_size : sizeof(void *)) * num;
		}

		va_end(val);

		throw_null(mem = malloc(size), "Ошибка при выделении памяти");

		for(u = 0, num = 1, dof = (char *) mem; u < dim_1; u++)
		{
			num *= dim_size[u];
			pof = dof;
			t_dof = (dof += num * sizeof(void *));
			step = (u == dim_2 ? type_size : sizeof(void *)) * dim_size[u + 1];

			for(v = 0; v < num; v++, pof += sizeof(void *), t_dof += step)
				memcpy(pof, & t_dof, sizeof(void *));
		}
	}
	catch(...)
	{
		free(mem);
		mem = NULL;
	}

	return mem;
}

