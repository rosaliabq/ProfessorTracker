#ifndef CMT_H
#define CMT_H

#define _USE_MATH_DEFINES

#include <stdio.h>
#include <cmath>
#include <limits>
#include <iostream>

#include <opencv2/opencv.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/nonfree/features2d.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/nonfree/nonfree.hpp>
#include <opencv2/nonfree/features2d.hpp>

#include "Runnable.h"

#define NAN numeric_limits<float>::quiet_NaN()

using namespace cv;
using namespace std;

class CMT : public Runnable
{
public:

	CMT();
	//bool isPoints(cv::Mat im_gray0, cv::Point2f topleft, cv::Point2f bottomright);
	void initialise(Mat frame0, Point2f topleft, Point2f bottomright);

	inline void setFrame(cv::Mat input_frame)
	{
		cv::Mat im_gray;
		cvtColor(input_frame, im_gray, CV_BGR2GRAY);
		frame_gray = im_gray;
	}

	inline bool getInit()
	{
		return isInit;
	}

	inline bool getHasResult()
	{
		return hasResult;
	}

	inline Rect getBox()
	{
		Rect box = Rect(topLeft,bottomRight);
		return box;
	}

private:

	// ----------PARAMETERS

	string detectorType;
	string descriptorType;
	string matcherType;
	int descriptorLength;
	int thrOutlier;
	float thrConf;
	float thrRatio;
	Mat frame_gray;

	bool estimateScale;
	bool estimateRotation;

	bool isInit;

	Ptr<FeatureDetector> detector;
	Ptr<DescriptorExtractor> descriptorExtractor;
	Ptr<DescriptorMatcher> descriptorMatcher;

	Mat selectedFeatures;
	vector<int> selectedClasses;
	Mat featuresDatabase;
	vector<int> classesDatabase;

	vector<vector<float> > squareForm;
	vector<vector<float> > angles;

	Point2f topLeft;
	Point2f topRight;
	Point2f bottomRight;
	Point2f bottomLeft;
	Rect lastWindow;
	Rect_<float> boundingbox;

	bool hasResult;

	Point2f centerToTopLeft;
	Point2f centerToTopRight;
	Point2f centerToBottomRight;
	Point2f centerToBottomLeft;

	vector<Point2f> springs;

	Mat im_prev;
	vector<pair<KeyPoint,int> > activeKeypoints;
	vector<pair<KeyPoint,int> > trackedKeypoints;

	int nbInitialKeypoints;

	vector<Point2f> votes;

	vector<pair<KeyPoint, int> > outliers;

	//-----------FUNCTIONS

	virtual void do_work();
	void estimate(const vector<pair<KeyPoint, int> >& keypointsIN, Point2f& center, float& scaleEstimate, float& medRot, vector<pair<KeyPoint, int> >& keypoints);
	void processFrame(Mat im_gray);

};

class Cluster
{
public:
	int first, second;//cluster id
	float dist;
	int num;
};

void inout_rect(const vector<KeyPoint>& keypoints, Point2f topleft, Point2f bottomright, vector<KeyPoint>& in, std::vector<KeyPoint>& out);
void track(Mat im_prev, Mat im_gray, const vector<pair<KeyPoint, int> >& keypointsIN, vector<pair<KeyPoint, int> >& keypointsTracked, vector<unsigned char>& status, int THR_FB = 20);
Point2f rotate(Point2f p, float rad);

#endif // CMT_H
