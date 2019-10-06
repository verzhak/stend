
#ifndef DB_HPP
#define DB_HPP

#include "all.hpp"
#include "third_party/qtftp/qftp.h"

enum EEntryType
{
	EET_DB,
	EET_CACHE_DB,
	EET_CACHE
};

struct SVideoEntry
{
	EEntryType type;
	QString hash, name, video_link, metadata_link;
};

struct SMapEntry
{
	EEntryType type;
	QString hash, name, map_link, height_map_link;
};

class CDownloadFile : public QObject
{
	Q_OBJECT

	bool is_download_finished;
	int download_id;
	QFile fl;
	QFtp ftp;

	public:

		CDownloadFile(const QString fname, const QString link);

		void wait_for_download();

	public slots:

		void start(int);
		void finish(int, bool);
};

class CDB : public QObject
{
	Q_OBJECT

	QString __host;
	QSqlDatabase cache, db;
	QMap<QString, SVideoEntry> video;
	QMap<QString, SMapEntry> map;

	QString load_file(const QString hash, const QString link, const QString dname);

	public:

		CDB(QObject * parent = NULL);
		~CDB();

		void connect(const QString host);
		void disconnect();

		inline bool is_connect() { return ! __host.isEmpty(); };
		inline const QMap<QString, SVideoEntry> video_list() { return video; };
		inline const QMap<QString, SMapEntry> map_list() { return map; };

		void load_video(const QString hash, QString & video_fname, QString & metadata_fname);
		void load_map(const QString hash, QString & map_fname, QString & height_map_fname);
};

#endif

