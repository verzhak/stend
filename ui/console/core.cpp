
#include "ui/console/core.hpp"

CConsole::CConsole(int argc, char * argv[]) :
	QObject(NULL)
{
	throw_if(argc != 8, "Недостаточное количество аргументов");

	script_fname = argv[1];
	src_video_fname = argv[2];
	src_metadata_fname = argv[3];
	dst_video_fname = argv[4];
	map_fname = argv[5];
	height_map_fname = argv[6];
	classifier_fname = argv[7];
}

void CConsole::run()
{
	CMainLoop main_loop;

	main_loop.vector_map.height = 576;
	main_loop.vector_map.width = 704;
	main_loop.vector_map.texture_size = 1024;
	main_loop.vector_map.texture_per_row = 4;
	main_loop.vector_map.accuracy = EMA_HIGH;
	main_loop.vector_map.palette = EP_THIN_LINE;
	main_loop.vector_map.is_polygon_as_a_line = true;
	main_loop.vector_map.map_fname = map_fname;
	main_loop.vector_map.height_map_fname = height_map_fname;
	main_loop.vector_map.classifier_fname = classifier_fname;

	// ############################################################################ 

	/*
	CDB db;
	QString db_src_video_fname, db_src_metadata_fname, db_map_fname, db_height_map_fname;

	db.connect("127.0.0.1");
	QString hash_video = db.video_list().keys()[0];
	QString hash_map = db.map_list().keys()[0];

	db.load_video(hash_video, db_src_video_fname, db_src_metadata_fname);
	db.load_map(hash_map, db_map_fname, db_height_map_fname);
	*/

	// ############################################################################ 

	main_loop.script.load_module("/home/amv/disser/project/super_stend/src/modules/build/cpp/libsimple.so");
	main_loop.script.load_module("/home/amv/disser/project/super_stend/src/modules/build/cpp/libopencv.so");
	main_loop.script.load_module("/home/amv/disser/project/super_stend/src/modules/build/pascal/libsimple.so");
	// main_loop.script.load_module("/home/amv/disser/project/super_stend/src/disser/build/libdeblurring.so");
	// main_loop.script.load_module("/home/amv/disser/project/super_stend/src/disser/build/lible_cun.so");

	main_loop.script.load(script_fname);
	main_loop.start(src_video_fname, src_metadata_fname, dst_video_fname);
	// main_loop.start(db_src_video_fname, db_src_metadata_fname, dst_video_fname);

	// const QString stat = "call";
	const QString stat = "combine_quality";

	main_loop.stats[stat]->display();
	main_loop.stats[stat]->save(QString("/home/amv/trash/") + stat);

	emit finished();
}

