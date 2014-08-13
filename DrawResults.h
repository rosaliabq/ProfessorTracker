
#include "opencv2\core\core.hpp"
#include "opencv2\highgui\highgui.hpp"

using namespace cv;

class DrawResults
{
public:

	void draw(Mat frame, vector< Rect_<int> > results);
	Mat getdetectedMask();

	static DrawResults* getInstance();

private:
	DrawResults();
	Rect scale_rect(const Rect& r, float scale);

	static DrawResults* pDrawResults;
	Mat frame_;
	Mat detectedMask_;
	bool bodyMask;
};