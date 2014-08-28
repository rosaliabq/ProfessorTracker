#ifdef _WIN32 
#define _CRT_SECURE_NO_DEPRECATE
#endif 

#include <stdio.h>
#include <iostream>

#include <opencv2\core\core.hpp>
#include <opencv2\imgproc\imgproc.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\video\video.hpp>

#include "BackgroundSegmentation.h"
#include "DetectionBased.h"
#include "DrawResults.h"
#include "AdaptiveBackgroundLearning.h"
#include "openTLD.h"
#include "opticalFlow.h"

using namespace cv;
using namespace std;

int main(){
	string path="C:/Users/Rosalía/Desktop/Beca/combustibles.mp4";
	VideoCapture capture(0);
	if ( !capture.isOpened() )
	{
		cout << "Failed to open!\n" << endl;
		return -1;
	}

	Mat frame, foreground, detectionMask;
	vector<Rect> facesResult;

	BackgroundSegmentation *bgSegmentation2 = new BackgroundSegmentation();
	AdaptiveBackgroundLearning *bgSegmentation = new AdaptiveBackgroundLearning();

	DetectionBased *dbTracker = new DetectionBased();

	openTLD *tld = new openTLD();
	opticalFlow *of = new opticalFlow();
	bool init = false;

	for (;;)
	{
		capture >> frame;

		if ( !frame.empty() )
		{
			if (!ConfigFile::GetInstance()->getAdaptativeBg() )
			{
				if ( !bgSegmentation2->isRunning() )
				{
					foreground = bgSegmentation2->getForeground();
					bgSegmentation2->setFrame(frame,  DrawResults::getInstance()->getdetectedMask() );
					bgSegmentation2->go();
				}
			}
			else
			{
				if (!bgSegmentation->isRunning() )
				{
					foreground = bgSegmentation->getResult();
					if (!DrawResults::getInstance()->getdetectedMask().empty() )
					{
						foreground = foreground | DrawResults::getInstance()->getdetectedMask();
					}
					bgSegmentation->setFrame(frame);
					bgSegmentation->go();
				}
			}

			if ( !dbTracker->isRunning() )
			{
				facesResult.clear();
				facesResult = dbTracker->getFaces();
				dbTracker->setFrameMask(frame, foreground);
				dbTracker->go();
			}

			if (!init && facesResult.size()==1 )
			{
				tld->setFrame(frame);
				tld->init(facesResult[0]);
				init = true;

				of->setFrame(frame);
				of->init(facesResult[0]);

			}

			if (init && !tld->isRunning() )
			{
				if (tld->getResult() != NULL )
				{
					rectangle(frame, *tld->getResult() , Scalar(255, 0, 255) , 2);
				}
				tld->setFrame(frame);
				tld->go();

			}

			if (init && !of->isRunning() )
			{
				vector<Point2f> resultOf=of->getResult();
				for (int i=0; i<resultOf.size(); i++)
				{
					circle(frame, resultOf[i], 2, Scalar(0, 255, 0));
				}
				of->setFrame(frame);
				of->go();
			}


			DrawResults::getInstance()->draw(frame, facesResult);

			if (ConfigFile::GetInstance()->getshowBackground() )
			{
				if ( !foreground.empty() )
				{
					imshow("Foreground", foreground);
				}  
			}

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
