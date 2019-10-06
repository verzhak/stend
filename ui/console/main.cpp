
#include "all.hpp"
#include "main_loop/display.hpp"
#include "ui/console/core.hpp"

int main(int argc, char * argv[])
{
	int ret = 0;

	try
	{
		qInstallMessageHandler(message_handler);
		Q_INIT_RESOURCE(base_resources);

		CConsole console(argc, argv);
		QCoreApplication app(argc, argv);

		QObject::connect(& console, SIGNAL(finished()), & app, SLOT(quit()));
		QTimer::singleShot(0, & console, SLOT(run()));

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
	fprintf(stderr, "%s\n", msg.toLocal8Bit().constData());
}

