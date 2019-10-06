
#ifndef ALL_HPP
#define ALL_HPP

#include <cstdio>
#include <cstring>
#include <cstdarg>
#include <unistd.h>
#include <time.h>
#include <omp.h>
#include <QtCore>
#include <QApplication>
#include <QDialog>
#include <QMessageBox>
#include <QXmlStreamReader>
#include <QMenu>
#include <QToolButton>
#include <QFont>
#include <QSplashScreen>
#include <QTimer>
#include <QtTest>
#include <QFileDialog>
#include <QSyntaxHighlighter>
#include <QListWidget>
#include <QTextBrowser>
#include <QMdiSubWindow>
#include <QResource>
#include <QLabel>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QInputDialog>
#include <lua.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/ocl/ocl.hpp>
#include <combine.h>

using namespace std;
using namespace cv;

// ############################################################################ 
// Вывод сообщений

void message(const QString msg);
void message_handler(QtMsgType type, const QMessageLogContext & context, const QString & msg);

// ############################################################################ 
// TODO

#define qTODO qDebug

// ############################################################################
// Исключения

#define throw_(msg) \
{\
	qCritical() << msg;\
	throw 1;\
};

#define throw_if(condition, msg) \
{\
	if((condition))\
		throw_(QObject::trUtf8(msg));\
}

#define throw_if_db(condition, db, msg) \
{\
	if((condition))\
		throw_(QString("%1 [%2]").arg(QObject::trUtf8(msg)).arg(db.lastError().text()));\
}

#define throw_null(pointer, msg) \
	throw_if((pointer) == NULL, msg)

#define throw_wm \
{\
	throw 1;\
};

#define throw_if_wm(condition) \
{\
	if((condition))\
		throw_wm;\
}

#define throw_null_wm(pointer) \
	throw_if_wm((pointer) == NULL)

// ############################################################################ 
// Выделение памяти

void * alloc(const unsigned type_size, const unsigned dim, ...);

#endif

