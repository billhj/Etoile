#pragma once

#ifndef MOTIONDETECTOR_H
#define MOTIONDETECTOR_H

#include <iostream>
#include <fstream>
#include <stdlib.h>

#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <time.h>
#include "util/Dirent.h"
#include <sstream>
#include <sys/types.h>
#include <sys/stat.h>

namespace Etoile
{
	using namespace std;
	using namespace cv;

	class MotionDetector
	{
	public:
		MotionDetector(void);
		virtual ~MotionDetector(void);
		void apply();
		void setDirectory(const std::string& path);
		void cleanDirectory();
		void setEnable(bool b){ _enable = b;}
		void isShow(bool b){_show = b;}
	private:
		string _dir;
		string _ext;
		string _logfile;
		int _delay;
		bool _enable;
		bool _show;
	};
}

#endif //MOTIONDETECTOR_H