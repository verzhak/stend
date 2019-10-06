
#include "ui/gui/ui/main_window.hpp"

CMainWindow::CMainWindow() :
	QMainWindow(NULL), db(this), script_view(this), modules_list(this), src_video_view(this), src_metadata_view(this), dst_video_view(this), map_view(this), height_map_view(this), db_menu_video(this, & db, true), db_menu_map(this, & db, false)
{
	setupUi(this);

	// ############################################################################ 
	// Связь с главным циклом и драйвером дисплея

	p_main_window = this;
	connect(& main_loop, SIGNAL(__start()), this, SLOT(experiment_start()));
	connect(& main_loop, SIGNAL(__stop()), this, SLOT(experiment_stop()));

	// ############################################################################ 
	// Настройка виджетов

	modules_list.setEditTriggers(QAbstractItemView::NoEditTriggers);
	modules_list.setSelectionMode(QAbstractItemView::SingleSelection);
	modules_list.setContextMenuPolicy(Qt::CustomContextMenu);
	connect(& modules_list, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(modules_list_context_menu(const QPoint &)));

	const auto flags = Qt::SubWindow | Qt::WindowTitleHint | Qt::WindowMinMaxButtonsHint | Qt::WindowShadeButtonHint;

	setup_sub_window(script_window, & script_view, script_window_title, ":/icons/script", QSize(600, 600), flags, false);
	setup_sub_window(modules_list_window, & modules_list, modules_list_window_title, ":/icons/module", QSize(200, 400), flags, false);
	setup_sub_window(src_video_window, & src_video_view, src_video_window_title, ":/icons/src_video", QSize(400, 400), flags, false);
	setup_sub_window(src_metadata_window, & src_metadata_view, src_metadata_window_title, ":/icons/metadata", QSize(400, 400), flags, false);
	setup_sub_window(dst_video_window, & dst_video_view, dst_video_window_title, ":/icons/save", QSize(400, 400), flags, false);
	setup_sub_window(map_window, & map_view, map_window_title, ":/icons/map", QSize(400, 400), flags, false);
	setup_sub_window(height_map_window, & height_map_view, height_map_window_title, ":/icons/height_map", QSize(400, 400), flags, false);

	modules_list_window->setStatusTip(modules_list_window_title);

	auto init_menu = [ this ](QMenu & menu, const QString title, const QString icon_name)
	{
		menu.setTitle(title);
		menu.setIcon(QIcon(icon_name));
	};

	auto add_action = [ this ](const QString title, const QString icon_name, const char * fun_name)
	{
		QAction * action = tool_bar->addAction(QIcon(icon_name), title, this, fun_name);
		
		action->setToolTip(title);
		action->setStatusTip(title);

		return action;
	};

	auto add_button = [ this ](QToolButton & button, QMenu & menu, const QString title, const QString icon_name)
	{
		button.setToolTip(title);
		button.setStatusTip(title);
		button.setIcon(QIcon(icon_name));
		button.setMenu(& menu);
		button.setPopupMode(QToolButton::InstantPopup);

		tool_bar->addWidget(& button);
	};

	// ############################################################################ 
	// Загрузка и сохранение

	init_menu(video_menu, trUtf8("Видео"), ":/icons/video");
	src_video_action = video_menu.addAction(QIcon(":/icons/src_video"), trUtf8("Загрузить исходное видео"), this, SLOT(process_src_video()));
	src_metadata_action = video_menu.addAction(QIcon(":/icons/metadata"), trUtf8("Загрузить метаданные исходного видео"), this, SLOT(process_src_metadata()));
	dst_video_action = video_menu.addAction(QIcon(":/icons/save"), trUtf8("Сохранить результирующее видео"), this, SLOT(process_dst_video()));
	
	init_menu(map_menu, trUtf8("Карта"), ":/icons/maps");
	map_action = map_menu.addAction(QIcon(":/icons/map"), trUtf8("Загрузить исходную карту"), this, SLOT(process_map()));
	height_map_action = map_menu.addAction(QIcon(":/icons/height_map"), trUtf8("Загрузить матрицу высот"), this, SLOT(process_height_map()));

	script_action = process_menu.addAction(QIcon(":/icons/script"), trUtf8("Загрузить скрипт"), this, SLOT(process_script()));
	process_menu.addAction(QIcon(":/icons/module"), trUtf8("Загрузить модуль"), this, SLOT(process_module()));
	process_menu.addMenu(& video_menu);
	process_menu.addMenu(& map_menu);

	// ############################################################################ 
	// Организация окон

	init_menu(foreground_menu, trUtf8("На передний план"), ":/icons/foreground");
	foreground_menu.addAction(QIcon(":/icons/script"), script_window_title, this, SLOT(foreground_script_window()));
	foreground_menu.addAction(QIcon(":/icons/module"), modules_list_window_title, this, SLOT(foreground_modules_list_window()));
	foreground_menu.addAction(QIcon(":/icons/src_video"), src_video_window_title, this, SLOT(foreground_src_video_window()));
	foreground_menu.addAction(QIcon(":/icons/metadata"), src_metadata_window_title, this, SLOT(foreground_src_metadata_window()));
	foreground_menu.addAction(QIcon(":/icons/dst_video"), dst_video_window_title, this, SLOT(foreground_dst_video_window()));
	foreground_menu.addAction(QIcon(":/icons/map"), map_window_title, this, SLOT(foreground_map_window()));
	foreground_menu.addAction(QIcon(":/icons/height_map"), height_map_window_title, this, SLOT(foreground_height_map_window()));

	window_menu.addMenu(& foreground_menu);
	window_menu.addAction(QIcon(":/icons/cascade"), trUtf8("Каскадом"), mdi_area, SLOT(cascadeSubWindows()));
	window_menu.addAction(QIcon(":/icons/tile"), trUtf8("Равномерно распределить"), mdi_area, SLOT(tileSubWindows()));

	// ############################################################################ 
	// Эксперимент

	experiment_menu.addAction(QIcon(":/icons/experiment_load"), trUtf8("Загрузить описание эксперимента"), this, SLOT(process_experiment_load()));
	experiment_menu.addAction(QIcon(":/icons/experiment_save"), trUtf8("Сохранить описание эксперимента"), this, SLOT(process_experiment_save()));

	// ############################################################################ 
	// Статистика

	init_menu(stat_menu_display, trUtf8("Вывести на экран"), ":/stat/display");
	init_menu(stat_menu_save, trUtf8("Сохранить"), ":/stat/save");

	for(auto & it : main_loop.stats)
	{
		QString icon_name = ":/stat/";
		CStat * obj = it.data();
		icon_name += obj->name_en();

		stat_menu_display.addAction(QIcon(icon_name), obj->name_ru(), obj, SLOT(display()));
		stat_menu_save.addAction(QIcon(icon_name), obj->name_ru(), obj, SLOT(save()));
	}

	stat_menu.addMenu(& stat_menu_display);
	stat_menu.addMenu(& stat_menu_save);

	// ############################################################################ 
	// База данных
	
	init_menu(db_menu, trUtf8("База данных"), ":/db/db");
	
	db_menu.addAction(QIcon(":/db/connect"), trUtf8("Подключиться к базе данных"), this, SLOT(db_connect()));
	db_menu.addSeparator();

	auto add_db_submenu = [ & ](CDbMenu & menu, const QString & menu_name, const QString & icon_name, const char * set_first_fname_fun_name, const char * set_second_fname_fun_name)
	{
		init_menu(menu, menu_name, icon_name);
		menu.setEnabled(false);
		connect(& menu, SIGNAL(set_first_fname(const QString)), this, set_first_fname_fun_name);
		connect(& menu, SIGNAL(set_second_fname(const QString)), this, set_second_fname_fun_name);
		db_menu.addMenu(& menu);
	};

	add_db_submenu(db_menu_video, trUtf8("Видеопоследовательности"), ":/db/video", SLOT(load_src_video(const QString)), SLOT(load_src_metadata(const QString)));
	add_db_submenu(db_menu_map, trUtf8("Карты"), ":/db/map", SLOT(load_map(const QString)), SLOT(load_height_map(const QString)));

	// ############################################################################ 

	add_button(process_button, process_menu, trUtf8("Загрузка и сохранение"), ":/icons/process");
	add_button(db_button, db_menu, trUtf8("База данных"), ":/db/db");
	add_button(window_button, window_menu, trUtf8("Управление окнами"), ":/icons/window");
	experiment_toggle_action = add_action("", "", SLOT(experiment_toggle()));
	add_button(experiment_button, experiment_menu, trUtf8("Эксперимент"), ":/icons/experiment");
	add_button(stat_button, stat_menu, trUtf8("Статистика"), ":/stat/stat");
	add_action(trUtf8("Настройки"), ":/icons/setup", SLOT(setup()));
	add_action(trUtf8("О программе"), ":/icons/about", SLOT(about()));

	stat_button.setEnabled(false);

	// ############################################################################ 

	unload_script();
	modules_list.clear();
	unload_src_video();
	unload_src_metadata();
	unset_dst_video();
	unload_map();
	unload_height_map();
}

CMainWindow::~CMainWindow()
{
	;
}

void CMainWindow::setup_sub_window(QMdiSubWindow *& window, QWidget * widget, const QString window_title, const QString icon_name, const QSize size, const QFlags<Qt::WindowType> flags, bool delete_on_close)
{
	throw_null(window = mdi_area->addSubWindow(widget, flags), "Не удалось создать дочернее окно");
	window->setWindowTitle(window_title);
	window->setWindowIcon(QIcon(icon_name)); // TODO Из-за флагов иконка не отображается
	window->resize(size);
	window->setAttribute(Qt::WA_DeleteOnClose, delete_on_close);
	window->setSystemMenu(NULL);
	mdi_area->setActiveSubWindow(window);
	window->show();
}

QString CMainWindow::get_fname_to_save_stat()
{
	return QFileDialog::getSaveFileName(this, trUtf8("Сохранить статистику"), "", trUtf8("Текстовый файл (*.txt)"));
}

void CMainWindow::display_Mat(QMdiSubWindow * window, const Mat & img)
{
	Mat rgb;
	const QSize size(img.cols, img.rows);

	switch(img.channels())
	{
		case 1:
		{
			rgb = img.clone();
			cvtColor(img, rgb, CV_GRAY2RGB);

			break;
		}
		case 3:
		{
			cvtColor(img, rgb, CV_BGR2RGB);

			break;
		}
		default:
		{
			vector<Mat> ch;

			split(img, ch);
			rgb = ch[0].clone();
			cvtColor(img, rgb, CV_GRAY2RGB);

			break;
		}
	};

	((QLabel *) window->widget())->setPixmap
	(
		QPixmap::fromImage
		(
			QImage((const unsigned char *) rgb.data, rgb.cols, rgb.rows, QImage::Format_RGB888)
		)
	);

	window->adjustSize();
}

void CMainWindow::display_Mat(const Mat & src, const Mat & dst)
{
	display_Mat(src_video_window, src);
	display_Mat(dst_video_window, dst);
}

void CMainWindow::display_png(const QString & name_ru, const QString & fname)
{
	QMdiSubWindow * window = NULL;
	QLabel * view = NULL;
	QPixmap pixmap(fname);
	QSize size;

	try
	{
		throw_if(pixmap.isNull(), "Не удалось загрузить график");
		throw_null(view = new QLabel(this), "Не удалось создать средство отображения PNG-файлов");

		view->setPixmap(pixmap);
		size = pixmap.size();

		setup_sub_window(window, view, name_ru, ":/icons/experiment_results", size, 0, true);
		window->adjustSize();
	}
	catch(...)
	{
		if(window != NULL)
			delete window;
		else if(view != NULL) // We bind view to window using DeleteOnClose
			delete view;
	}
}

// ############################################################################ 
// База данных

void CMainWindow::db_connect()
{
	try
	{
		const QString host = QInputDialog::getText(this, trUtf8("Адрес БД"), trUtf8("Введите адрес хоста, на котором запущена СУБД"), QLineEdit::Normal, trUtf8("127.0.0.1"));;

		db.connect(host);
		db_reload();
	}
	catch(...)
	{
		db_menu_video.clear();
		db_menu_map.clear();

		db_menu_video.setEnabled(false);
		db_menu_map.setEnabled(false);
	}
}

void CMainWindow::db_reload()
{
	try
	{
		if(db.is_connect())
		{
			auto str_type = [](const EEntryType type)
			{
				switch(type)
				{
					case EET_DB:
						return QString("БД");
					case EET_CACHE_DB:
						return QString("БД, кэш");
					case EET_CACHE:
						return QString("Кэш !!!");
				};

				throw_("Некорректный тип элемента списка");
			};

			db_menu_video.clear();

			for(auto & entry : db.video_list())
			{
				QAction * action = db_menu_video.addAction(QString("%1 [%2]").arg(entry.name).arg(str_type(entry.type)));
				action->setData(QVariant(entry.hash));
			}

			db_menu_map.clear();

			for(auto & entry : db.map_list())
			{
				QAction * action = db_menu_map.addAction(QString("%1 [%2]").arg(entry.name).arg(str_type(entry.type)));
				action->setData(QVariant(entry.hash));
			}

			db_menu_video.setEnabled(true);
			db_menu_map.setEnabled(true);
		}
	}
	catch(...)
	{
		;
	}
}

// ############################################################################ 
// Контроль эксперимента

void CMainWindow::experiment_start()
{
	try
	{
		const QString tip = trUtf8("Останов эксперимента");

		experiment_toggle_action->setIcon(QIcon(":/icons/stop"));
		experiment_toggle_action->setText(tip);
		experiment_toggle_action->setToolTip(tip);
		experiment_toggle_action->setStatusTip(tip);

		stat_button.setEnabled(true);

	}
	catch(...)
	{
		;
	}
}

void CMainWindow::experiment_stop()
{
	const QString tip = trUtf8("Запуск эксперимента");
	const bool is_experiment_may_be_run = ! (script_fname.isEmpty() || src_video_fname.isEmpty() || src_metadata_fname.isEmpty() || dst_video_fname.isEmpty());

	experiment_toggle_action->setIcon(QIcon(":/icons/start"));
	experiment_toggle_action->setText(tip);
	experiment_toggle_action->setToolTip(tip);
	experiment_toggle_action->setStatusTip(tip);
	experiment_toggle_action->setEnabled(is_experiment_may_be_run);
}

void CMainWindow::experiment_toggle()
{
	try
	{
		if(main_loop.is_run())
			main_loop.stop();
		else
		{
			main_loop.script.load(script_fname);
			main_loop.vector_map.map_fname = map_fname;
			main_loop.vector_map.height_map_fname = height_map_fname;
			main_loop.start(src_video_fname, src_metadata_fname, dst_video_fname);
		}
	}
	catch(...)
	{
		;
	}
}

void CMainWindow::experiment_load(const QString fname)
{
	try
	{
		QXmlStreamReader stream;
		QFile fl(fname);
		QString name;

		throw_if(! fl.open(QIODevice::ReadOnly | QIODevice::Text), "Не удалось открыть файл на чтение");
		stream.setDevice(& fl);

		throw_if(! stream.readNextStartElement(), "Не удалось прочитать открывающий тег");
		throw_if(stream.name() != "verzhak", "Открывающий тег не соответствует формату");

		unload_script();
		unload_all_modules();
		unload_src_video();
		unload_src_metadata();
		unset_dst_video();
		unload_map();
		unload_height_map();
		//	unload_classifier(); TODO Раскомментировать после реализации LOAD_SET для classifier

		while(! stream.atEnd())
		{
			const QXmlStreamReader::TokenType type = stream.readNext();

			switch(type)
			{
				case QXmlStreamReader::StartElement:
				{
					if(stream.isWhitespace())
						stream.skipCurrentElement();

					name = stream.name().toString();

					break;
				}
				case QXmlStreamReader::EndElement:
				{
					name = "";

					break;
				}
				case QXmlStreamReader::Characters:
				{
					if(! stream.isWhitespace())
					{
						if(name == "script")
							load_script(stream.text().toString());
						else if(name == "module")
							load_module(stream.text().toString());
						else if(name == "src_video")
							load_src_video(stream.text().toString());
						else if(name == "src_metadata")
							load_src_metadata(stream.text().toString());
						else if(name == "dst_video")
							set_dst_video(stream.text().toString());
						else if(name == "map")
						{
							load_map(stream.text().toString());
							main_loop.vector_map.map_fname = map_fname;
						}
						else if(name == "height_map")
						{
							load_height_map(stream.text().toString());
							main_loop.vector_map.height_map_fname = height_map_fname;
						}
						else if(name == "classifier")
						{
						//	load_classifier(stream.text().toString()); TODO Раскомментировать после реализации LOAD_SET для classifier
							main_loop.vector_map.classifier_fname = classifier_fname;
						}
						else if(name == "foreshortening_height")
							main_loop.vector_map.height = stream.text().toUInt();
						else if(name == "foreshortening_width")
							main_loop.vector_map.width = stream.text().toUInt();
						else if(name == "texture_size")
							main_loop.vector_map.texture_size = stream.text().toUInt();
						else if(name == "texture_per_row")
							main_loop.vector_map.texture_per_row = stream.text().toUInt();
						else if(name == "map_accuracy")
							main_loop.vector_map.accuracy = (EMapAccuracy) stream.text().toUInt();
						else if(name == "palette")
							main_loop.vector_map.palette = (EPalette) stream.text().toUInt();
						else if(name == "is_polygon_as_a_line")
							main_loop.vector_map.is_polygon_as_a_line = (stream.text().toUInt() == 1);
					}

					break;
				}
			}
		}
	}
	catch(...)
	{
		;
	}
}

void CMainWindow::experiment_save(const QString fname)
{
	try
	{
		QXmlStreamWriter stream;
		QFile fl(fname);

		throw_if(! fl.open(QIODevice::WriteOnly | QIODevice::Text), "Не удалось открыть файл на запись");
		stream.setDevice(& fl);

		stream.setAutoFormatting(true);
		stream.writeStartDocument();
		stream.writeStartElement("verzhak");

#define WRITE_UINT(name, value)\
	stream.writeTextElement(name, QString::number(value));

#define WRITE_FNAME(name, fname)\
	if(! fname.isEmpty())\
		stream.writeTextElement(name, fname);

			WRITE_FNAME("script", script_fname);
			WRITE_FNAME("src_video", src_video_fname);
			WRITE_FNAME("src_metadata", src_metadata_fname);
			WRITE_FNAME("dst_video", dst_video_fname);
			WRITE_FNAME("map", map_fname);
			WRITE_FNAME("height_map", height_map_fname);
			WRITE_FNAME("classifier", classifier_fname);
			WRITE_UINT("foreshortening_height", main_loop.vector_map.height);
			WRITE_UINT("foreshortening_width", main_loop.vector_map.width);
			WRITE_UINT("texture_size", main_loop.vector_map.texture_size);
			WRITE_UINT("texture_per_row", main_loop.vector_map.texture_per_row);
			WRITE_UINT("map_accuracy", main_loop.vector_map.accuracy);
			WRITE_UINT("palette", main_loop.vector_map.palette);
			WRITE_UINT("is_polygon_as_a_line", main_loop.vector_map.is_polygon_as_a_line);

			for(auto & md : modules_list.findItems("*", Qt::MatchWildcard))
				stream.writeTextElement("module", main_loop.script.get_module_fname(md->text()));

		stream.writeEndElement();
		stream.writeEndDocument();
	}
	catch(...)
	{
		;
	}
}

// ############################################################################ 
// Окна на передний план

#define FOREGROUND(fun, window)\
void CMainWindow::fun()\
{\
	mdi_area->setActiveSubWindow(window);\
}

FOREGROUND(foreground_script_window, script_window)
FOREGROUND(foreground_modules_list_window, modules_list_window)
FOREGROUND(foreground_src_video_window, src_video_window)
FOREGROUND(foreground_src_metadata_window, src_metadata_window)
FOREGROUND(foreground_dst_video_window, dst_video_window)
FOREGROUND(foreground_map_window, map_window)
FOREGROUND(foreground_height_map_window, height_map_window)

// ############################################################################ 
// Загрузка, установ, выгрузка и сброс

#define LOAD_SET(fun, unload_fun, fname, window, window_title, code)\
void CMainWindow::fun(const QString __fname)\
{\
	try\
	{\
		code;\
\
		window->setStatusTip(__fname);\
		fname = __fname;\
\
		main_loop.stop();\
		db_reload();\
	}\
	catch(...)\
	{\
		unload_fun();\
	}\
}

#define UNLOAD_UNSET(fun, fname, window, content, window_title)\
void CMainWindow::fun()\
{\
	window->setStatusTip(window_title);\
	fname.clear();\
	content.clear();\
\
	main_loop.stop();\
}

LOAD_SET(load_script, unload_script, script_fname, script_window, script_window_title,
	QFile fl(__fname);
	throw_if(! fl.open(QIODevice::ReadOnly | QIODevice::Text), "Невозможно открыть файл");

	script_document.setPlainText(fl.readAll());
	script_highlighter.setDocument(& script_document);
	script_view.setDocument(& script_document);
	script_view.setTabStopWidth(10);
);

LOAD_SET(load_src_video, unload_src_video, src_video_fname, src_video_window, src_video_window_title,
	Mat frame;
	VideoCapture video(__fname.toLocal8Bit().constData());

	throw_if(! video.isOpened(), "Не удалось открыть файл с исходной видеопоследовательностью");
	video.set(CV_CAP_PROP_POS_FRAMES, video.get(CV_CAP_PROP_FRAME_COUNT) / 2);
	throw_if(! video.read(frame), "Не удалось прочитать файл с исходной видеопоследовательностью");
	display_Mat(src_video_window, frame);
);

#define YES_WINDOW(view, window)\
	const QPixmap img = QPixmap(":/icons/yes");\
\
	view.setPixmap(img);\
	window->adjustSize();

LOAD_SET(load_src_metadata, unload_src_metadata, src_metadata_fname, src_metadata_window, src_metadata_window_title,
		YES_WINDOW(src_metadata_view, src_metadata_window));

LOAD_SET(set_dst_video, unset_dst_video, dst_video_fname, dst_video_window, dst_video_window_title,
		YES_WINDOW(dst_video_view, dst_video_window));

LOAD_SET(load_map, unload_map, map_fname, map_window, map_window_title,
		YES_WINDOW(map_view, map_window));

LOAD_SET(load_height_map, unload_height_map, height_map_fname, height_map_window, height_map_window_title,
		YES_WINDOW(height_map_view, height_map_window));

// TODO LOAD_SET для classifier_fname

UNLOAD_UNSET(unload_script, script_fname, script_window, script_view, script_window_title)
UNLOAD_UNSET(unload_src_video, src_video_fname, src_video_window, src_video_view, src_video_window_title)
UNLOAD_UNSET(unload_src_metadata, src_metadata_fname, src_metadata_window, src_metadata_view, src_metadata_window_title)
UNLOAD_UNSET(unset_dst_video, dst_video_fname, dst_video_window, dst_video_view, dst_video_window_title)
UNLOAD_UNSET(unload_map, map_fname, map_window, map_view, map_window_title)
UNLOAD_UNSET(unload_height_map, height_map_fname, height_map_window, height_map_view, height_map_window_title)
// TODO UNLOAD_UNSET для classifier_fname

void CMainWindow::load_module(const QString module_fname)
{
	const QString module_name = main_loop.script.load_module(module_fname);

	if(! modules_list.findItems(module_name, Qt::MatchExactly).size())
		modules_list.addItem(module_name);
}

void CMainWindow::unload_all_modules()
{
	for(auto it : modules_list.findItems("*", Qt::MatchWildcard))
		unload_module(it);
}

void CMainWindow::unload_module(QListWidgetItem * current_item)
{
	main_loop.script.unload_module(current_item->text());
	delete current_item;
}

// ############################################################################ 
// Слоты - загрузка и выгрузка исходных данных

#define PROCESS(msg, suffix, fun, load_fun, open_save_fun)\
void CMainWindow::fun()\
{\
	try\
	{\
		const QString fname = QFileDialog::open_save_fun(this, trUtf8(msg), "", trUtf8(suffix));\
\
		if(! fname.isEmpty())\
			load_fun(fname);\
	}\
	catch(...)\
	{\
		;\
	}\
}

PROCESS("Загрузить скрипт", "Скрипт (*.lua)", process_script, load_script, getOpenFileName)
PROCESS("Загрузить модуль", "Модуль (*.so *.dll)", process_module, load_module, getOpenFileName)
PROCESS("Загрузить исходную видеопоследовательность", "Видео (*.avi *.mpg *.mp4);;Кадр (*.bmp *.jpg *.png *.tiff)", process_src_video, load_src_video, getOpenFileName) // TODO Более полное перечисление расширений
PROCESS("Загрузить метаданные исходной видеопоследовательности", "Метаданные (*.xml)", process_src_metadata, load_src_metadata, getOpenFileName)
PROCESS("Сохранить результирующую видеопоследовательность", "Видео (*.avi *.mpg *.mp4);;Кадр (*.bmp *.jpg *.png *.tiff)", process_dst_video, set_dst_video, getSaveFileName) // TODO Более полное перечисление расширений + автодобавление расширения, если его нет 
PROCESS("Загрузить карту", "Карта (*.sxf)", process_map, load_map, getOpenFileName)
PROCESS("Загрузить матрицу высот", "Матрица высот (*.mtr)", process_height_map, load_height_map, getOpenFileName)
PROCESS("Загрузить описание эксперимента", "Описание эксперимента (*.xml)", process_experiment_load, experiment_load, getOpenFileName)
PROCESS("Сохранить описание эксперимента", "Описание эксперимента (*.xml)", process_experiment_save, experiment_save, getSaveFileName)

void CMainWindow::modules_list_context_menu(const QPoint & pos)
{
	try
	{
		QListWidgetItem * current_item = modules_list.currentItem();

		if(current_item != NULL && current_item->isSelected())
		{
			QMenu context_menu;
			QAction * delete_action = context_menu.addAction(QIcon(":/icons/delete"), trUtf8("Выгрузить модуль"));

			if(delete_action == context_menu.exec(modules_list.mapToGlobal(pos)))
				unload_module(current_item);
		}
	}
	catch(...)
	{
		;
	}

	main_loop.stop();
}

// ############################################################################ 
// Прочие слоты

void CMainWindow::setup()
{
	try
	{
		CSetupDialog(this).exec();
	}
	catch(...)
	{
		;
	}
}

void CMainWindow::about()
{
	try
	{
		CAboutDialog(this).exec();
	}
	catch(...)
	{
		;
	}
}

