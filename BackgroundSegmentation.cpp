
#include "BackgroundSegmentation.h"

BackgroundSegmentation :: BackgroundSegmentation(): Runnable()
{
	mog_.set("detectShadows", 0); 
	foreground_ = Mat();
	frame_ = Mat();
}

BackgroundSegmentation :: ~BackgroundSegmentation()
{

}

void BackgroundSegmentation :: setFrame(Mat frame, Mat mask)
{
	frame_ = frame;
	mask_ = mask;
}

Mat BackgroundSegmentation :: getForeground()
{
	return foreground_;
}

void BackgroundSegmentation :: do_work()
{
	mog_.operator ()(frame_, foreground_, 0.3, mask_  );
}
