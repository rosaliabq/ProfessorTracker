#include <opencv2\core\core.hpp>

#include <tld\TLD.h>

#include "Runnable.h"

using namespace tld;
using namespace cv;

class openTLD : public Runnable
{
public:
	openTLD();
	~openTLD();
	void setFrame(Mat frame);
	void init(Rect box);
	Rect* getResult();

private:
	TLD *tld_;
	Rect *result_;
	Mat frame_;
	virtual void do_work();
};