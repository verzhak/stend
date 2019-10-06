
#ifndef MAIN_WINDOW_HPP
#define MAIN_WINDOW_HPP

#include "all.hpp"
#include "script_engine/script_engine.hpp"
#include "module/module.hpp"
#include "matrix/matrix.hpp"
#include "db/db.hpp"
#include "main_loop/main_loop.hpp"
#include "ui/gui/ui/setup_dialog.hpp"
#include "ui/gui/ui/about_dialog.hpp"
#include "ui_main_window.h"
#include "ui/gui/ui/script_highlighter.hpp"
#include "ui/gui/ui/menu.hpp"

class CMainWindow : public QMainWindow, private Ui::main_window
{
	Q_OBJECT

	const QString script_window_title = trUtf8("Скрипт");
	const QString modules_list_window_title = trUtf8("Модули");
	const QString src_video_window_title = trUtf8("Исходная видеопоследовательность");
	const QString src_metadata_window_title = trUtf8("Метаданные исходной видеопоследовательности");
	const QString dst_video_window_title = trUtf8("Результирующая видеопоследовательность");
	const QString map_window_title = trUtf8("Векторная карта");
	const QString height_map_window_title = trUtf8("Матрица высот");
	const QString db_connect_action_title = trUtf8("Открыть базу данных");
	QTextBrowser script_view;
	QListWidget modules_list;
	QLabel src_video_view, src_metadata_view, dst_video_view, map_view, height_map_view;
	QString script_fname, src_video_fname, src_metadata_fname, dst_video_fname, map_fname, height_map_fname, classifier_fname;
	QMenu process_menu, video_menu, map_menu, foreground_menu, window_menu, experiment_menu, stat_menu, stat_menu_display, stat_menu_save, db_menu;
	CDbMenu db_menu_video, db_menu_map;
	QToolButton process_button, window_button, experiment_button, stat_button, db_button;
	QTextDocument script_document;
	QScriptHighlighter script_highlighter;
	QMdiSubWindow * script_window, * modules_list_window, * src_video_window, * src_metadata_window, * dst_video_window, * map_window, * height_map_window;
	QAction * src_video_action, * src_metadata_action, * dst_video_action, * script_action, * experiment_toggle_action, * map_action, * height_map_action;
	CDB db;
	CMainLoop main_loop;
	
	void setup_sub_window(QMdiSubWindow *& window, QWidget * widget, const QString window_title, const QString icon_name, const QSize size, const QFlags<Qt::WindowType> flags, bool delete_on_close);
	void display_Mat(QMdiSubWindow * window, const Mat & img);

	friend class CSetupDialog;

	public:

		CMainWindow();
		~CMainWindow();

		QString get_fname_to_save_stat();
		void display_Mat(const Mat & src, const Mat & dst);
		void display_png(const QString & name_ru, const QString & fname);

	public slots:

		void load_module(const QString module_fname);
		void unload_all_modules();
		void unload_module(QListWidgetItem * current_item);

		void load_script(const QString script_fname);
		void unload_script();

		void load_src_video(const QString video_fname);
		void unload_src_video();

		void load_src_metadata(const QString metadata_fname);
		void unload_src_metadata();

		void set_dst_video(const QString video_fname);
		void unset_dst_video();

		void load_map(const QString map_fname);
		void unload_map();

		void load_height_map(const QString map_fname);
		void unload_height_map();

		void experiment_start();
		void experiment_load(const QString fname);
		void experiment_save(const QString fname);
		void experiment_toggle();
		void experiment_stop();
	
		void setup();
		void about();

		void db_connect();
		void db_reload();

		void foreground_script_window();
		void foreground_modules_list_window();
		void foreground_src_video_window();
		void foreground_src_metadata_window();
		void foreground_dst_video_window();
		void foreground_map_window();
		void foreground_height_map_window();

		void process_script();
		void process_module();
		void modules_list_context_menu(const QPoint & pos);
		void process_src_video();
		void process_src_metadata();
		void process_dst_video();
		void process_map();
		void process_height_map();
		void process_experiment_load();
		void process_experiment_save();
};

extern CMainWindow * p_main_window;

#endif

