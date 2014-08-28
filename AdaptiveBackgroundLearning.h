/*
This file is part of BGSLibrary.

BGSLibrary is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

BGSLibrary is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with BGSLibrary.  If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

#include <iostream>

#include <opencv2/opencv.hpp>

#include "Runnable.h"

class AdaptiveBackgroundLearning : public Runnable
{
private:
	bool firstTime;
	cv::Mat img_background;
	double alpha;
	long limit;
	long counter;
	double minVal;
	double maxVal;
	bool enableThreshold;
	int threshold;
	bool showForeground;
	bool showBackground;
	cv::Mat img_input, img_output;

	virtual void do_work();

public:
	AdaptiveBackgroundLearning();
	~AdaptiveBackgroundLearning();

	void setFrame(cv::Mat frame);
	cv::Mat getResult();

};