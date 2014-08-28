
#include "DrawResults.h"
#include <opencv2\imgproc\imgproc.hpp>

DrawResults* DrawResults :: pDrawResults= 0;

DrawResults* DrawResults :: getInstance()
{

	if (!pDrawResults) 
	{
		pDrawResults = new DrawResults();
	}
	return pDrawResults;

}

DrawResults :: DrawResults()
{
	detectedMask_ = Mat();
	bodyMask = ConfigFile::GetInstance()->getbodyMask(); //ConfigFile::GetInstance()->getbodyMask();
}

Mat DrawResults :: getdetectedMask()
{
	return detectedMask_;
}

void DrawResults :: draw(Mat frame, vector< Rect_<int> > results)
{

	frame.copyTo(frame_);
	for ( int i = 0; i < results.size() ; i++ )
	{
		rectangle(frame_, results[i], Scalar(255, 255, 255), 2);
	}
	Mat prueba;
	
	imshow("Video", frame_);

	if ( results.size() > 0 )
	{
		Rect r0(Point(), frame.size());
		detectedMask_ = Mat::zeros(frame.rows, frame.cols, CV_8UC1);
		for ( int i = 0; i < results.size() ; i++ )
		{
			Rect r1=scale_rect(results[i], 2.0);
			r1 = r1 & r0;
			detectedMask_(r1) = 255;
		}
	}
}

Rect DrawResults :: scale_rect(const Rect& r, float scale)
{
	float width,height;
	int x,y;

	Point2f m = Point2f(r.x+((float)r.width)/2, r.y+((float)r.height)/2);

	if (bodyMask)
	{
		width  = r.width  * 3.0;
		height = r.height * 10.0;
		x = cvRound(m.x - width/2);
		y = cvRound(m.y - height/10);
		
	}else
	{
		width  = r.width  * scale;
		height = r.height * scale;
		x = cvRound(m.x - width/2);
		y = cvRound(m.y - height/2);
		
	}
		
	return Rect(x, y, cvRound(width), cvRound(height));

}

