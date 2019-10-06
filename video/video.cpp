
#include "video/video.hpp"

CVideoRead::CVideoRead(const QString fname) :
	video(fname.toLocal8Bit().constData())
{
	throw_if(! video.isOpened(), "Не удалось открыть файл с видеопоследовательностью на чтение");
}

matrix CVideoRead::read()
{
	Mat frame;
	matrix ret = NULL;

	if(video.read(frame))
		throw_null(ret = new CMatrix(frame), "Не удалось скопировать кадр");

	return ret;
}

// ############################################################################ 

CVideoWrite::CVideoWrite(const QString __fname, const double __fps) :
	fname(__fname), fps(__fps), is_first(true)
{
	;
}

void CVideoWrite::write(matrix frame)
{
	Mat frame_rgb;

	if(is_first)
	{
		is_first = false;
		height = frame->height();
		width = frame->width();

		auto fourcc =
			// CV_FOURCC('X', '2', '6', '4')
			// CV_FOURCC('A', 'V', 'C', '1')
			// CV_FOURCC('F', 'M', 'P', '4')
			CV_FOURCC('X', 'V', 'I', 'D')
			// CV_FOURCC_DEFAULT
			;

		// toLocal8Bit() почему-то выбрасывает segfault в Windows
		throw_if(! video.open(fname.toStdString(), fourcc, fps, Size(width, height)), "Не удалось открыть результирующий файл для сохранения видеопоследовательности");
	}

	throw_if(! (frame->rows == height && frame->cols == width), "Размер очередного кадра не совпадает с размерами предыдущих кадров сохраняемой видеопоследовательности");

	switch(frame->number_of_channels())
	{
		case 1:
		{
			cvtColor(* frame, frame_rgb, CV_GRAY2RGB);

			break;
		}
		case 3:
		{
			frame_rgb = * frame;

			break;
		}
		default:
		{
			throw_("Неподдерживаемое количество каналов для сохраняемого изображения");

			break;
		}
	}
	
	video.write(frame_rgb);
}

