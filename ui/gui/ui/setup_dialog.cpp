
#include "ui/gui/ui/setup_dialog.hpp"

CSetupDialog::CSetupDialog(CMainWindow * __parent) :
	QDialog(__parent), parent(__parent)
{
	setupUi(this);

	unsigned_validator.setRange(2, 4096);
	
#define CREATE_UNSIGNED_EDIT(widget, var_old, def)\
	widget->setValidator(& unsigned_validator);\
	var_old = def;\
	widget->setText(QString::number(def));\
	connect(widget, SIGNAL(textChanged(const QString &)), this, SLOT(editing_finished()));

#define CREATE_UNSIGNED_LIST(widget, var_old, def)\
	var_old = def;\
	widget->setCurrentIndex(def);\
	connect(widget, SIGNAL(currentIndexChanged(int)), this, SLOT(editing_finished()));

#define CREATE_BOOLEAN_CHECK(widget, var_old, def)\
	var_old = def;\
	widget->setChecked(def);\
	connect(widget, SIGNAL(stateChanged(int)), this, SLOT(editing_finished()));

	CREATE_UNSIGNED_EDIT(height_edit, height_old, parent->main_loop.vector_map.height);
	CREATE_UNSIGNED_EDIT(width_edit, width_old, parent->main_loop.vector_map.width);
	CREATE_UNSIGNED_EDIT(texture_size_edit, texture_size_old, parent->main_loop.vector_map.texture_size);
	CREATE_UNSIGNED_EDIT(texture_per_row_edit, texture_per_row_old, parent->main_loop.vector_map.texture_per_row);
	CREATE_UNSIGNED_LIST(accuracy_box, accuracy_old, parent->main_loop.vector_map.accuracy);
	CREATE_UNSIGNED_LIST(palette_box, palette_old, parent->main_loop.vector_map.palette);
	CREATE_BOOLEAN_CHECK(is_polygon_as_a_line_box, is_polygon_as_a_line_old, parent->main_loop.vector_map.is_polygon_as_a_line);
}

void CSetupDialog::editing_finished()
{
#define VALUE_COND_EDIT_RESET(widget, fun, var_old)\
	widget->text().fun() != var_old

#define VALUE_COND_LIST_RESET(widget, var_old)\
	widget->currentIndex() != var_old

#define VALUE_COND_CHECK_RESET(widget, var_old)\
	widget->isChecked() != var_old

#define VALUE_COND_EDIT_APPLY(widget)\
	! widget->text().isEmpty()

	const bool is_reset_enabled =
	(
		VALUE_COND_EDIT_RESET(height_edit, toUInt, height_old)
		||
		VALUE_COND_EDIT_RESET(width_edit, toUInt, width_old)
		||
		VALUE_COND_EDIT_RESET(texture_size_edit, toUInt, texture_size_old)
		||
		VALUE_COND_EDIT_RESET(texture_per_row_edit, toUInt, texture_per_row_old)
		||
		VALUE_COND_LIST_RESET(accuracy_box, accuracy_old)
		||
		VALUE_COND_LIST_RESET(palette_box, palette_old)
		||
		VALUE_COND_CHECK_RESET(is_polygon_as_a_line_box, is_polygon_as_a_line_old)
	);
	const bool is_apply_enabled =
	is_reset_enabled
	&&
	(
		VALUE_COND_EDIT_APPLY(height_edit)
		&&
		VALUE_COND_EDIT_APPLY(width_edit)
		&&
		VALUE_COND_EDIT_APPLY(texture_size_edit)
		&&
		VALUE_COND_EDIT_APPLY(texture_per_row_edit)
		// accuracy - любое значение может быть применимо
		// palette - любое значение может быть применимо
		// is_polygon_as_a_line - любое значение может быть применимо
	);
	
	apply_button->setEnabled(is_apply_enabled);
	reset_button->setEnabled(is_reset_enabled);
}

void CSetupDialog::on_apply_button_clicked()
{
	try
	{
		throw_if(texture_size_edit->text().toUInt() % 4, "Размер текстуры должен быть кратен четырем");

		// ############################################################################ 
		// Применение изменений

#define APPLY_UNSIGNED_EDIT(widget, var, var_old)\
		var = var_old = widget->text().toUInt();

#define APPLY_UNSIGNED_LIST(widget, var, var_old, type)\
		var = var_old = (type) widget->currentIndex();

#define APPLY_BOOLEAN_CHECK(widget, var, var_old)\
		var = var_old = widget->isChecked();

		APPLY_UNSIGNED_EDIT(height_edit, parent->main_loop.vector_map.height, height_old);
		APPLY_UNSIGNED_EDIT(width_edit, parent->main_loop.vector_map.width, width_old);
		APPLY_UNSIGNED_EDIT(texture_size_edit, parent->main_loop.vector_map.texture_size, texture_size_old);
		APPLY_UNSIGNED_EDIT(texture_per_row_edit, parent->main_loop.vector_map.texture_per_row, texture_per_row_old);
		APPLY_UNSIGNED_LIST(accuracy_box, parent->main_loop.vector_map.accuracy, accuracy_old, EMapAccuracy);
		APPLY_UNSIGNED_LIST(palette_box, parent->main_loop.vector_map.palette, palette_old, EPalette);
		APPLY_BOOLEAN_CHECK(is_polygon_as_a_line_box, parent->main_loop.vector_map.is_polygon_as_a_line, is_polygon_as_a_line_old);

		// ############################################################################ 

		editing_finished();
	}
	catch(...)
	{
		;
	}
}

void CSetupDialog::on_reset_button_clicked()
{
#define RESET_UNSIGNED_EDIT(widget, var_old) \
	widget->setText(QString::number(var_old))

#define RESET_UNSIGNED_LIST(widget, var_old) \
	widget->setCurrentIndex(var_old)

#define RESET_BOOLEAN_CHECK(widget, var_old) \
	widget->setChecked(var_old)

	RESET_UNSIGNED_EDIT(height_edit, height_old);
	RESET_UNSIGNED_EDIT(width_edit, width_old);
	RESET_UNSIGNED_EDIT(texture_size_edit, texture_size_old);
	RESET_UNSIGNED_EDIT(texture_per_row_edit, texture_per_row_old);
	RESET_UNSIGNED_LIST(accuracy_box, accuracy_old);
	RESET_UNSIGNED_LIST(palette_box, palette_old);
	RESET_BOOLEAN_CHECK(is_polygon_as_a_line_box, is_polygon_as_a_line_old);
}

