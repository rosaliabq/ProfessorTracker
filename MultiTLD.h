
#include <opencv2\core\core.hpp>
#include <opencv2\highgui\highgui.hpp>

#include "motld\MultiObjectTLD.h"

#include "Runnable.h"

using namespace cv;

class MultiTLD : public Runnable 
{
public:
	MultiTLD() : Runnable()
	{
		MOTLDSettings settings(COLOR_MODE_RGB);
		settings.useColor = true;
		int capWidth = 	640;
		int capHeight = 480;	
		this->motld_ = new MultiObjectTLD(640, 480, settings);
		unsigned char img[(640*480)*3];
	}

	~MultiTLD()
	{
	}

	void init(Rect box)
	{
		ObjectBox objBox = {box.x, box.y, box.width, box.height, 0};
		motld_->addObject(objBox);
	}

	void setFrame(Mat frame)
	{
		img_ = frame.data;
	}

	Rect getResult()
	{
		Rect rBox;
		cout << motld_->getValid() <<endl;
		if (motld_->getValid() )
		{
			ObjectBox objBox = motld_->getObjectBox();
			rBox = Rect(objBox.x, objBox.y, objBox.width, objBox.height);
		}
		else
		{
			rBox = Rect(0, 0, 0, 0);
		}

		return rBox;
	}

private:
	unsigned char * img_;
	MultiObjectTLD *motld_;

	virtual void do_work()
	{
		motld_->processFrame(img_);
	}
};