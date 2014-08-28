
#include <stdio.h>
#include <iostream>

#include <opencv2\highgui\highgui.hpp>
#include <opencv2\imgproc\imgproc.hpp>

#include "Runnable.h"
#include "detection_based_tracker.hpp"

using namespace cv;

#define PROFILE_DETECTION 1

class DetectionBased : public Runnable 
{
public:
	void setFrameMask(Mat frame, Mat mask);
	vector< Rect_<int> > getFaces();

	DetectionBased();
	~DetectionBased();

	void stop();

private:

	string pathXml_;
	DetectionBasedTracker *dbtObject_;
	DetectionBasedTracker::Parameters params_;
	Mat frame_, mask_;
	vector< Rect_<int> > faces_;

	void process();
	virtual void do_work();
};