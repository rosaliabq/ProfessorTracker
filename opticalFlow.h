#include <opencv2/core/core.hpp>
#include <opencv2/video/tracking.hpp>

#include "Runnable.h"

using namespace cv;
using namespace std;

class opticalFlow : public Runnable
{
public:
	opticalFlow();
	~opticalFlow();

	vector<Point2f> init(Rect box);
	void setFrame(Mat frame);
	vector<Point2f> getResult();


	Mat frameGrey_, framePrev_;
	vector<Point2f> corners_, cornersPrev_;
	vector<uchar> features_found_;
	Mat feature_errors_;
private:
	virtual void do_work();
	void update();
};