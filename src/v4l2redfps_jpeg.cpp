/* ---------------------------------------------------------------------------
** This software is in the public domain, furnished "as is", without technical
** support, and with no warranty, express or implied, as to its usefulness for
** any purpose.
**
** v4l2redfps_jpeg.cpp
**
** Copy from a MJpeg V4L2 capture device to an other V4L2 output device with
** reduced framerate
**
** -------------------------------------------------------------------------*/

#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <linux/videodev2.h>
#include <sys/ioctl.h>
#include <signal.h>

#include <chrono>
#include <fstream>

#include "logger.h"

#include "V4l2Device.h"
#include "V4l2Capture.h"
#include "V4l2Output.h"

using namespace std::chrono;

int stop = 0;

/* ---------------------------------------------------------------------------
**  SIGINT handler
** -------------------------------------------------------------------------*/
void sighandler(int)
{
	printf("SIGINT\n");
	stop = 1;
}

/* ---------------------------------------------------------------------------
**  main
** -------------------------------------------------------------------------*/
int main(int argc, char* argv[])
{
	int verbose = 0;
	const char* in_devname = "/dev/video0";
	const char* out_devname = "/dev/video1";
	int c = 0;

	int frameCapturesPerSecond;
	V4l2Access::IoType ioTypeIn = V4l2Access::IOTYPE_MMAP;
	V4l2Access::IoType ioTypeOut = V4l2Access::IOTYPE_MMAP;

	while ((c = getopt(argc, argv, "hP:F:v::rw")) != -1)
	{
		switch (c)
		{
		case 'v':	verbose = 1; if (optarg && *optarg == 'v') verbose++;  break;
		case 'r':	ioTypeIn = V4l2Access::IOTYPE_READWRITE; break;
		case 'w':	ioTypeOut = V4l2Access::IOTYPE_READWRITE; break;
		case 'F':
			frameCapturesPerSecond = atoi(optarg);
			break;
		case 'h':
		{
			std::cout << argv[0] << " [-v[v]] [-W width] [-H height] [-F outputReducedFPS] source_device dest_device" << std::endl;
			std::cout << "\t -v            : verbose " << std::endl;
			std::cout << "\t -vv           : very verbose " << std::endl;
			std::cout << "\t -r            : V4L2 capture using read interface (default use memory mapped buffers)" << std::endl;
			std::cout << "\t -w            : V4L2 capture using write interface (default use memory mapped buffers)" << std::endl;
			std::cout << "\t source_device : V4L2 capture device (default " << in_devname << ")" << std::endl;
			std::cout << "\t dest_device   : V4L2 capture device (default " << out_devname << ")" << std::endl;
			exit(0);
		}
		}
	}
	if (optind < argc)
	{
		in_devname = argv[optind];
		optind++;
	}
	if (optind < argc)
	{
		out_devname = argv[optind];
		optind++;
	}

	// initialize log4cpp
	initLogger(verbose);

	// init V4L2 capture interface
	V4L2DeviceParameters param(in_devname, V4L2_PIX_FMT_MJPEG, 1920, 1080, 0, verbose);
	V4l2Capture* videoCapture = V4l2Capture::create(param, ioTypeIn);

	if (videoCapture == NULL)
	{
		LOG(WARN) << "Cannot create V4L2 capture interface for device:" << in_devname;
	}
	else
	{
		// init V4L2 output interface
		V4L2DeviceParameters outparam(out_devname, videoCapture->getFormat(), videoCapture->getWidth(), videoCapture->getHeight(), 0, verbose);
		V4l2Output* videoOutput = V4l2Output::create(outparam, ioTypeOut);
		if (videoOutput == NULL)
		{
			LOG(WARN) << "Cannot create V4L2 output interface for device:" << out_devname;
		}
		else
		{
			timeval tv;

			LOG(NOTICE) << "Start Copying from " << in_devname << " to " << out_devname;
			signal(SIGINT, sighandler);

			int copiedFrame = 0;
			int droppedFrame = 0;
			int droppedFramePerCopy = 0;
			while (!stop)
			{
				usleep( (1.0f/frameCapturesPerSecond) * 1000000);

				tv.tv_sec = 1;
				tv.tv_usec = 0;
				int ret = videoCapture->isReadable(&tv);
				if (ret == 1)
				{
					char buffer[videoCapture->getBufferSize()];
					int rsize = videoCapture->read(buffer, sizeof(buffer));
					if (rsize == -1)
					{
						LOG(NOTICE) << "stop " << strerror(errno);
						stop = 1;
					}
					else
					{
						int wsize = videoOutput->write(buffer, rsize);
						LOG(DEBUG) << "CopiedFrames/S:1," << ", CopiedSize:" << wsize << ", TotalCopiedFrames:" << copiedFrame++;
					}
				}
				else if (ret == -1)
				{
					LOG(NOTICE) << "stop " << strerror(errno);
					stop = 1;
				}
			}
			delete videoOutput;
		}
		delete videoCapture;
	}

	return 0;
}
