
#ifndef MODULE_HPP
#define MODULE_HPP

#include "all.hpp"
#include "matrix/matrix.hpp"
#include "module/interface.h"

class CModule
{
	const unsigned buf_size = 4096;
	QList<QPair<QString, int> > _params, _return_values;
	QString _name;
	QLibrary library;

	int (* __init)(char *, const unsigned, unsigned *, unsigned *);
	int (* __destroy)();
	int (* __reset)();
	int (* __get_name)(const bool, const unsigned, char *, const unsigned);
	int (* __get_type)(const bool, const unsigned, int *);
	int (* __get_value)(const bool, const unsigned, void *);
	int (* __set_value)(const bool, const unsigned, const void *);
	int (* __run)();

	QFunctionPointer resolve(const QString name);
	void load_elem(const bool is_param, const unsigned num);

	public:

		CModule(const QString fname);
		~CModule();

		inline const QString fname() { return library.fileName(); };
		inline const QString name() { return _name; };
		void reset();
		int run(lua_State * state);
};

#endif

