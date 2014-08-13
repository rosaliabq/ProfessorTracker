
#include "ViolaJones.h"

ViolaJones :: ViolaJones() : Runnable()
{
	minSize_ = Size(96,96);
	cascadePath_ = DEFAULT_PATH;
	frame_ = Mat();
	mask_ = Mat();
	results_.clear();
	detectInRegion_=false;
	flipFrame_=false;

	cascade_.load( cascadePath_);
}

ViolaJones :: ViolaJones(string path, Size minSize, bool region, bool flip ) : Runnable()
{
	minSize_ = minSize;
	cascadePath_ = path;
	frame_ = Mat();
	mask_ = Mat();
	results_.clear();
	detectInRegion_ = region;
	flipFrame_ = flip;

	cascade_.load( cascadePath_);
}

ViolaJones :: ~ViolaJones()
{

}

void ViolaJones :: setMats(Mat frame, Mat mask)
{
	if (!flipFrame_)
	{
		frame_ = frame;
		mask_ = mask;
	}
	else
	{
		flip(frame, frame_, 1);
		flip(mask, mask_, 1);
	}
	
}

vector<Rect> ViolaJones :: getResult()
{
	if (flipFrame_)
	{
		vector<Rect> results2_;
		for (int i = 0 ; i < results_.size() ; i++ ) 
		{
			int x1 = frame_.cols - results_.at(i).x  - results_.at(i).width;
			int y1 = results_.at(i).y;
			results2_.push_back( Rect(x1, y1, results_.at(i).width, results_.at(i).height ) );
		}
		return results2_;
	}
	return results_;
}

void ViolaJones :: do_work()
{
	results_.clear();
	if (detectInRegion_)
	{
		cascade_.detectMultiScale( frame_, results_, 1.1, 2, 0|CV_HAAR_FIND_BIGGEST_OBJECT|CV_HAAR_SCALE_IMAGE, minSize_, Size(), mask_);
	}
	else
	{
		cascade_.detectMultiScale( frame_, results_, 1.1, 2, 0|CV_HAAR_SCALE_IMAGE, minSize_, Size(), mask_);
	}
}


