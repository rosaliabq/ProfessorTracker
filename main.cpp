
#include <stdio.h>
#include <iostream>

#include <opencv2\core\core.hpp>
#include <opencv2\imgproc\imgproc.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\video\video.hpp>

#include "BackgroundSegmentation.h"
#include "DetectionBased.h"
#include "DrawResults.h"

#define SHOW_FOREGROUND 0

using namespace cv;
using namespace std;

int main(){
	string path="C:/Users/Rosalía/Desktop/Beca/combustibles.mp4";
	VideoCapture capture(path);
	if ( !capture.isOpened() )
	{
		cout << "Failed to open!\n" << endl;
		return -1;
	}

	Mat frame, foreground, detectionMask;
	vector<Rect> facesResult;

	BackgroundSegmentation *bgSegmentation = new BackgroundSegmentation();
	DetectionBased *dbTracker = new DetectionBased();

	for (;;)
	{
		capture >> frame;
		
		if ( !frame.empty() )
		{
			if ( !bgSegmentation->isRunning() )
			{
				foreground = bgSegmentation->getForeground();
				bgSegmentation->setFrame(frame,  DrawResults::getInstance()->getdetectedMask() );
				bgSegmentation->go();
			}

			if ( !dbTracker->isRunning() )
			{
				facesResult.clear();
				facesResult = dbTracker->getFaces();
				Mat prueba = Mat::ones(frame.rows,frame.cols,CV_8UC1);
				dbTracker->setFrameMask(frame, prueba);
				dbTracker->go();
			}

			DrawResults::getInstance()->draw(frame,facesResult);

#if SHOW_FOREGROUND
			if ( !foreground.empty() )
			{
				imshow("Foreground", foreground);
			}  
#endif // SHOW_FOREGROUND

		}else
		{
			break;
		}
		if( cv::waitKey(1) == 27 )
		{
			break;
		}

	}


	return 0;

}
