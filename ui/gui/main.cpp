
#include "all.hpp"
#include "script_engine/script_engine.hpp"
#include "module/module.hpp"
#include "matrix/matrix.hpp"
#include "stat/stat.hpp"
#include "ui/main_window.hpp"

int main(int argc, char * argv[])
{
	int ret;

	try
	{
		QApplication app(argc, argv);
		qInstallMessageHandler(message_handler);
		Q_INIT_RESOURCE(base_resources);
		Q_INIT_RESOURCE(gui_resources);

		// ############################################################################ 
		// Заставка

		QPixmap img(":/splash"); // TODO Другой splash
		QSplashScreen splash(img);
		splash.show();

		app.processEvents();

		// ############################################################################ 

		CMainWindow main_window;

		main_window.show();
		splash.finish(& main_window);

		ret = app.exec();
	}
	catch(...)
	{
		ret = -1;
	}

	return ret;
}

void message(const QString msg)
{
	if(msg != NULL)
		QMessageBox::critical(NULL, QObject::trUtf8("Ошибка"), msg);
}

