
#include <stdio.h>

#include <opencv2\objdetect\objdetect.hpp>

#include "Runnable.h"

using namespace cv;

#define DEFAULT_PATH "C:/Users/Rosalía/Desktop/Beca/haar/haarcascade_frontalface_alt.xml"

class ViolaJones : public Runnable {

public:
	ViolaJones();
	ViolaJones(string path, Size minSize, bool region, bool flip );
	~ViolaJones();

	void setMats(Mat frame, Mat mask);

	vector<Rect> getResult();
	
	

private:
	string cascadePath_;
	Size minSize_;
	Mat frame_, mask_;
	vector<Rect> results_;
	CascadeClassifier cascade_;
	bool detectInRegion_,flipFrame_;

	virtual void do_work();

};

