
#ifndef VIDEO_HPP
#define VIDEO_HPP

#include "all.hpp"
#include "matrix/matrix.hpp"
#include "module/interface.h"

class CVideoRead
{
	VideoCapture video;

	public:

		CVideoRead(const QString fname);

		matrix read();
		inline double fps() { return video.get(CV_CAP_PROP_FPS); };
		inline Size size() { return Size(video.get(CV_CAP_PROP_FRAME_WIDTH), video.get(CV_CAP_PROP_FRAME_HEIGHT)); };
		inline unsigned frame_num() { return video.get(CV_CAP_PROP_FRAME_COUNT); };
};

class CVideoWrite
{
	const QString fname;
	const double fps;
	bool is_first;
	unsigned height, width;
	VideoWriter video;

	public:

		CVideoWrite(const QString fname, const double fps);

		void write(matrix frame);
};

#endif

