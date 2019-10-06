
#ifndef MENU_HPP
#define MENU_HPP

#include "all.hpp"
#include "db/db.hpp"

class CDbMenu : public QMenu
{
	Q_OBJECT

	CDB * __db;
	bool __is_video;

	public:

		CDbMenu(QWidget * parent, CDB * db, bool is_video);

	public slots:

		void trigger(QAction * action);

	signals:

		void set_first_fname(const QString fname);
		void set_second_fname(const QString fname);
};

#endif

