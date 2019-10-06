
#include "ui/gui/ui/menu.hpp"

CDbMenu::CDbMenu(QWidget * parent, CDB * db, bool is_video) :
	QMenu(parent)
{
	throw_null(db, "Передан нулевой указатель");
	__db = db;
	__is_video = is_video;

	connect(this, SIGNAL(triggered(QAction *)), this, SLOT(trigger(QAction *)));
}

void CDbMenu::trigger(QAction * action)
{
	const QString hash = action->data().toString();
	QString fname_1, fname_2;

	if(__is_video)
		__db->load_video(hash, fname_1, fname_2);
	else
		__db->load_map(hash, fname_1, fname_2);

	emit set_first_fname(fname_1);
	emit set_second_fname(fname_2);
}

