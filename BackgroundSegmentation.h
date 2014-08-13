
#include <stdio.h>

#include <opencv2\opencv.hpp>
#include <opencv2\imgproc\imgproc.hpp>
#include <opencv2\video\background_segm.hpp>

#include "Runnable.h"

using namespace cv;

class BackgroundSegmentation : public Runnable
{

public:
	void setFrame(Mat frame, Mat mask);
	Mat getForeground(void);

	BackgroundSegmentation();
	~BackgroundSegmentation();

private:
	virtual void do_work();
	BackgroundSubtractorMOG2 mog_;
	Mat foreground_;
	Mat frame_;
	Mat mask_;

};