
#include "main_loop/display.hpp"

unsigned delay;
const string src_wname = "Source", dst_wname = "Result";
Mat __src, __dst;

void display::init(const unsigned fps)
{
	delay = 1000 / fps;
}

void display::destroy()
{
	waitKey(-1);
	destroyAllWindows();
}

QString display::get_fname_to_save_stat()
{
	char buf[4096];

	printf("Enter fname >> ");
	scanf("%s", buf);
	
	return buf;
}

void display::show_Mat(const Mat & src, const Mat & dst)
{
	__src = src.clone();
	__dst = dst.clone();

	imshow(src_wname, __src);
	imshow(dst_wname, __dst);

	waitKey(delay);
}

void display::show_png(const QString & name_ru, const QString & fname)
{
	Mat img = imread(fname.toLocal8Bit().constData());

	throw_null(img.data, "Не удалось открыть png-файл");
	imshow("Results", img);
	waitKey(-1);
}

