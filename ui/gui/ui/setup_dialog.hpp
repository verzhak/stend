
#ifndef SETUP_DIALOG_HPP
#define SETUP_DIALOG_HPP

#include "all.hpp"
#include "ui_setup_dialog.h"
#include "map/map.hpp"
#include "ui/gui/ui/main_window.hpp"

class CMainWindow;

class CSetupDialog : public QDialog, private Ui::setup_dialog
{
	Q_OBJECT

	bool is_polygon_as_a_line_old;
	unsigned height_old, width_old, texture_size_old, texture_per_row_old;
	EMapAccuracy accuracy_old;
	EPalette palette_old;
	QIntValidator unsigned_validator;
	CMainWindow * parent;

	public:

		CSetupDialog(CMainWindow * __parent);

	public slots:

		void editing_finished();

		void on_apply_button_clicked();
		void on_reset_button_clicked();
};

#endif

