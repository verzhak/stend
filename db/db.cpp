
#include "db/db.hpp"

CDB::CDB(QObject * parent) :
	QObject(parent)
{
	throw_if(! QSqlDatabase::isDriverAvailable("QPSQL"), "Драйвер для PostgreSQL не найден");
	throw_if(! QSqlDatabase::isDriverAvailable("QSQLITE"), "Драйвер для sqlite не найден");

	db = QSqlDatabase::addDatabase("QPSQL", "db");
	cache = QSqlDatabase::addDatabase("QSQLITE", "cache");

	__host = "";
}

CDB::~CDB()
{
	disconnect();
}

void CDB::connect(const QString host)
{
	disconnect();

	// ############################################################################ 
	// База данных

	db.setHostName(host);
	db.setDatabaseName("tapsi");
	db.setUserName("tapsi");
	db.setPassword("liverpool"); // TODO

	throw_if_db(! db.open(), db, "Не удалось подключиться к базе данных");

	QSqlQuery db_query(db);

	// ############################################################################ 
	// Кэш

	const QString cache_fname = host + ".db";
	const bool is_create_cache = ! QFile::exists(cache_fname);

	cache.setDatabaseName(cache_fname);
	throw_if_db(! cache.open(), cache, "Не удалось открыть кэширующую базу данных");

	QSqlQuery cache_query(cache);

	if(is_create_cache)
	{
		throw_if_db(! cache_query.exec("create table video (id int primary key, hash text, name text, video_fname text, metadata_fname text)"), cache, "Не удалось создать таблицу кэширования для видеоданных");
		throw_if_db(! cache_query.exec("create table map (id int primary key, hash text, name text, map_fname text, height_map_fname text)"), cache, "Не удалось создать таблицу кэширования для картографических материалов");
	}

	// ############################################################################ 
	// Создание каталогов под кэш
	
	QDir root = QDir::root();

	auto mkpath = [ & ](const QString dname)
	{
		throw_if(! root.mkpath(QString("%1/cache/%2/%3/").arg(QDir::currentPath()).arg(host).arg(dname)), "Не удалось создать каталог");
	};

	mkpath("video");
	mkpath("map");

	// ############################################################################ 
	// Инициализация списка видеоданных
	
	db_query.exec("select hash, name, video_link, metadata_link from video");

	while(db_query.next())
	{
		const QString hash = db_query.value(0).toString(), name = db_query.value(1).toString(), video_link = db_query.value(2).toString(), metadata_link = db_query.value(3).toString();
		const SVideoEntry entry = { .type = EET_DB, .hash = hash, .name = name, .video_link = video_link, .metadata_link = metadata_link };

		video[hash] = entry;
	}

	cache_query.exec("select hash, name, video_fname, metadata_fname from video");

	while(cache_query.next())
	{
		const QString hash = cache_query.value(0).toString(), name = cache_query.value(1).toString(), video_link = cache_query.value(2).toString(), metadata_link = cache_query.value(3).toString();
		const SVideoEntry entry = { .type = video.contains(hash) ? EET_CACHE_DB : EET_CACHE, .hash = hash, .name = name, .video_link = video_link, .metadata_link = metadata_link };

		video[hash] = entry;
	}

	// ############################################################################ 
	// Инициализация списка карт
	
	db_query.exec("select hash, name, map_link, height_map_link from map");

	while(db_query.next())
	{
		const QString hash = db_query.value(0).toString(), name = db_query.value(1).toString(), map_link = db_query.value(2).toString(), height_map_link = db_query.value(3).toString();
		const SMapEntry entry = { .type = EET_DB, .hash = hash, .name = name, .map_link = map_link, .height_map_link = height_map_link };

		map[hash] = entry;
	}

	cache_query.exec("select hash, name, map_fname, height_map_fname from map");

	while(cache_query.next())
	{
		const QString hash = cache_query.value(0).toString(), name = cache_query.value(1).toString(), map_link = cache_query.value(2).toString(), height_map_link = cache_query.value(3).toString();
		const SMapEntry entry = { .type = map.contains(hash) ? EET_CACHE_DB : EET_CACHE, .hash = hash, .name = name, .map_link = map_link, .height_map_link = height_map_link };

		map[hash] = entry;
	}

	// ############################################################################ 

	__host = host;
}

void CDB::disconnect()
{
	__host = "";

	db.close();
	cache.close();
}

QString CDB::load_file(const QString hash, const QString link, const QString dname)
{
	const QString fname = QString("%1/cache/%2/%3/%4.%5").arg(QDir::currentPath()).arg(__host).arg(dname).arg(hash).arg(link.section('.', -1));
	CDownloadFile fl(fname, link);

	fl.wait_for_download();

	return fname;
}

void CDB::load_video(const QString hash, QString & video_fname, QString & metadata_fname)
{
	throw_if(__host.isEmpty(), "База данных не подключена");
	throw_if(! video.contains(hash), "Видеопоследовательность с указанным хэшем не существует");

	SVideoEntry & entry = video[hash];

	if(entry.type == EET_DB)
	{
		QSqlQuery cache_query(cache);

		entry.video_link = load_file(hash, entry.video_link, "video");
		entry.metadata_link = load_file(hash, entry.metadata_link, "video");
		entry.type = EET_CACHE_DB;

		cache_query.exec(QString("insert into video (hash, name, video_fname, metadata_fname) values ('%1', '%2', '%3', '%4')").arg(hash, entry.name, entry.video_link, entry.metadata_link));
	}

	video_fname = entry.video_link;
	metadata_fname = entry.metadata_link;
}

void CDB::load_map(const QString hash, QString & map_fname, QString & height_map_fname)
{
	throw_if(__host.isEmpty(), "База данных не подключена");
	throw_if(! map.contains(hash), "Карта с указанным хэшем не существует");

	SMapEntry & entry = map[hash];

	if(entry.type == EET_DB)
	{
		QSqlQuery cache_query(cache);

		entry.map_link = load_file(hash, entry.map_link, "map");
		entry.height_map_link = load_file(hash, entry.height_map_link, "map");
		entry.type = EET_CACHE_DB;

		cache_query.exec(QString("insert into map (hash, name, map_fname, height_map_fname) values ('%1', '%2', '%3', '%4')").arg(hash, entry.name, entry.map_link, entry.height_map_link));
	}

	map_fname = entry.map_link;
	height_map_fname = entry.height_map_link;
}

// ############################################################################ 

CDownloadFile::CDownloadFile(const QString fname, const QString link) :
	QObject(NULL), fl(fname), ftp(this)
{
	QUrl url(link);

	is_download_finished = false;

	throw_if(! fl.open(QIODevice::WriteOnly), "Во время открытия файла на запись произошла ошибка");
	connect(& ftp, SIGNAL(commandStarted(int)), this, SLOT(start(int)));
	connect(& ftp, SIGNAL(commandFinished(int, bool)), this, SLOT(finish(int, bool)));

	ftp.setTransferMode(QFtp::Active);
	ftp.connectToHost(url.host(), url.port(21));
	ftp.login(url.userName(), url.password());
	download_id = ftp.get(url.path(), & fl);
	ftp.close();
}

void CDownloadFile::start(int id)
{
	;
}

void CDownloadFile::finish(int id, bool)
{
	if(id == download_id)
		is_download_finished = true;
}

void CDownloadFile::wait_for_download()
{
	while(! is_download_finished)
		qApp->processEvents(QEventLoop::AllEvents);
}

