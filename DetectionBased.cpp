
#include "DetectionBased.h"

DetectionBased :: DetectionBased() : Runnable()
{
	params_.maxObjectSize = 400;
	params_.maxTrackLifetime = 20;
	params_.minDetectionPeriod = 7;
	params_.minNeighbors = 3;
	params_.minObjectSize = 20;
	params_.scaleFactor = 1.05;

	pathXml_="C:/Users/Rosalía/Desktop/Beca/haar/haarcascade_frontalface_alt.xml";
	// haarcascade_frontalface_alt  lbpcascade_profileface
	dbtObject_=new DetectionBasedTracker(pathXml_,params_);
	dbtObject_->run();
}

DetectionBased :: ~DetectionBased()
{

}

void DetectionBased :: setFrameMask(Mat frame, Mat mask)
{
	cvtColor(frame,frame_,CV_BGR2GRAY);
	mask_ = mask;
}

vector< Rect_<int> > DetectionBased :: getFaces()
{
	return faces_;
}

void DetectionBased :: do_work()
{
	dbtObject_->process(frame_,mask_);
	dbtObject_->getObjects(faces_);
}

void DetectionBased :: stop()
{
	dbtObject_->stop();
}