#include "openTLD.h"

openTLD::openTLD() : Runnable()
{
	tld_ = new TLD();
	frame_ = Mat();
}

openTLD::~openTLD()
{

}

void openTLD::setFrame(Mat frame)
{
	frame_ = frame;
}

void openTLD::init(Rect box)
{
	Mat frameGrey;
	cvtColor(frame_, frameGrey, CV_BGR2GRAY);
	tld_->detectorCascade->imgWidth = frameGrey.cols;
	tld_->detectorCascade->imgHeight = frameGrey.rows;
	tld_->detectorCascade->imgWidthStep = frameGrey.step;
	tld_->selectObject(frameGrey, &box);
}

void openTLD::do_work()
{
	if (!frame_.empty() )
	{
		tld_->processImage(frame_);
		result_ = tld_->currBB;
	}
}

Rect* openTLD::getResult()
{
	return result_;
}