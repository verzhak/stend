
#ifndef SMART_LIST_HPP
#define SMART_LIST_HPP

#include "all.hpp"

template<typename tp>
class CSmartList : public QList<QSharedPointer<tp> >
{
	public:

		typedef QList<QSharedPointer<tp> > super;

		CSmartList() :
			super()
		{
			;
		}

		void append(tp * ptr)
		{
			throw_null(ptr, "Получен нулевой указатель");

			super::append(QSharedPointer<tp>(ptr));
		};

		void removeOne(tp * ptr)
		{
			const unsigned size = this->size();
			unsigned v;

			throw_null(ptr, "Невозможно удалить пустой указатель");

			for(v = 0; v < size; v++)
				if((* this)[v] == ptr)
				{
					this->removeAt(v);

					break;
				}
		};
};

#endif

