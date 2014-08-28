
#include "opticalFlow.h"

opticalFlow::opticalFlow() : Runnable()
{
	cornersPrev_.clear();
	corners_.clear();
}

opticalFlow::~opticalFlow()
{

}

vector<Point2f> opticalFlow::init(Rect box)
{
	Mat mask = Mat::zeros(frameGrey_.rows, frameGrey_.cols, CV_8U);
	mask(box) = 1;
	goodFeaturesToTrack(frameGrey_, corners_, 100, 0.05, 1.0, mask);
	update();
	return cornersPrev_;
}

void opticalFlow::setFrame(Mat frame)
{
	Mat frameG;
	cvtColor(frame, frameG, CV_BGR2GRAY);
	frameG.copyTo(frameGrey_);
}

void opticalFlow::do_work()
{
	calcOpticalFlowPyrLK(framePrev_, frameGrey_, 
		cornersPrev_, corners_, 
		features_found_, feature_errors_,
		Size(15, 15), 5,
		cvTermCriteria(CV_TERMCRIT_ITER | CV_TERMCRIT_EPS, 20, 0.3), 0);

	update();
}

void opticalFlow::update()
{
	frameGrey_.copyTo(framePrev_);
	cornersPrev_ = move(corners_);
	corners_.clear();
	frameGrey_ = Mat();

}

vector<Point2f> opticalFlow::getResult()
{
	return cornersPrev_;
}