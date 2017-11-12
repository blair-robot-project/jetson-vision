#include <numeric>
#include "opencv2/opencv.hpp"

using namespace std;
using namespace cv;

void on_low_h_thresh_trackbar(int, void *);
void on_high_h_thresh_trackbar(int, void *);
void on_low_s_thresh_trackbar(int, void *);
void on_high_s_thresh_trackbar(int, void *);
void on_low_v_thresh_trackbar(int, void *);
void on_high_v_thresh_trackbar(int, void *);

Mat1b threshholdWithCUDA(const Mat &in, const Ptr<cuda::Filter> &gBlur, const Vec3b &lowerHSV, const Vec3b &upperHSV);
vector<Point2i> getLargestContour(const Mat1b &in, const int &mode);

/**
 *
 * @param contour
 * @param width
 * @param height
 * @return A vector of 4 corners, in order top left, top right, bottom left, bottom right
 */
vector<Point2f> getCorners(const vector<Point2i> &contour, const int &width, const int &height);

bool checkCorners(const vector<Point2f> &corners, const int &width, const int &height, const int &minDiag=30, const int &border=0);

Mat1f getCameraPose(const vector<Point2f> &pictureCorners, const vector<Point3f> &objectCorners,
                    const Mat1f &cameraMatrix,
                    const Mat1f &distortionCoefficients);

int low_h=40, low_s=135, low_v=55;
int high_h=60, high_s=255, high_v=160;

vector<vector<Point2i>> contours;

int main(int argc, char* argv[]) {

	vector<Point3f> objPoints(4);
	objPoints[0] = Point3f(0,0,0);
	objPoints[1] = Point3f(2,0,0);
	objPoints[2] = Point3f(0,5.75,0);
	objPoints[3] = Point3f(2,5.75,0);

	Mat frame, goodFrame, cameraMatrix, distCoeff, rvec, tvec, zyx, final;
	Mat1b threshed;
	Mat bottom = Mat(Vec4f(0,0,0,1));
	bottom.convertTo(bottom, 6);
	Ptr<cuda::Filter> gBlur;
	vector<Point> cont;
	vector<Point2f> points(4);
	int width, height;

	VideoCapture cap("/media/LinHDD/Videos/my_video-1.mkv");

//	VideoCapture cap(0);

//	VideoCapture cap("/media/LinHDD/Images/vision/Screenshot from my_video-1.mkv.png");

	width = static_cast<int>(cap.get(CAP_PROP_FRAME_WIDTH));
	height = static_cast<int>(cap.get(CAP_PROP_FRAME_HEIGHT));

	gBlur = cuda::createGaussianFilter(CV_8UC3,CV_8UC3, Size(5,5), 0, 0);

	const string inputSettingsFile = argc > 1 ? argv[1] : "default.yml";

	FileStorage fs(inputSettingsFile, FileStorage::READ); // Read the settings
	if (!fs.isOpened())
	{
		cout << "Could not open the configuration file: \"" << inputSettingsFile << "\"" << endl;
		return -1;
	}

	fs["camera_matrix"] >> cameraMatrix;
	fs["distortion_coefficients"] >> distCoeff;

	namedWindow("Video Capture", WINDOW_NORMAL);
	namedWindow("Object Detection", WINDOW_FULLSCREEN);
	//-- Trackbars to set thresholds for RGB values
	createTrackbar("Low H","Object Detection", &low_h, 180, on_low_h_thresh_trackbar);
	createTrackbar("High H","Object Detection", &high_h, 180, on_high_h_thresh_trackbar);
	createTrackbar("Low S", "Object Detection", &low_s, 255, on_low_s_thresh_trackbar);
	createTrackbar("High S", "Object Detection", &high_s, 255, on_high_s_thresh_trackbar);
	createTrackbar("Low V", "Object Detection", &low_v, 255, on_low_v_thresh_trackbar);
	createTrackbar("High V", "Object Detection", &high_v, 255, on_high_v_thresh_trackbar);

	while((char)waitKey(17)!='q'){
		cap>>frame;
		if(!frame.empty()) {
			goodFrame = frame.clone();
		}
		threshed = threshholdWithCUDA(goodFrame, gBlur, Vec3b(low_h, low_s, low_v), Vec3b(high_h, high_s, high_v));
		cont = getLargestContour(threshed, CHAIN_APPROX_SIMPLE);

		if (!cont.empty()) {
			points = getCorners(cont, width, height);
			if(checkCorners(points,width,height,30,0)){
				cout << "M = "<< endl << " "  << getCameraPose(points, objPoints, cameraMatrix, distCoeff) << endl << endl;
			}
		}

		imshow("Video Capture", goodFrame);
		imshow("Object Detection", threshed);
	}
	return 0;
}

Mat1b threshholdWithCUDA(const Mat &in, const Ptr<cuda::Filter> &gBlur, const Vec3b &lowerHSV, const Vec3b &upperHSV){
	cuda::GpuMat mat;
	Mat out;
	mat.upload(in);
	cuda::cvtColor(mat,mat,COLOR_BGR2HSV);
	gBlur->apply(mat,mat);
	mat.download(out);
	inRange(out, lowerHSV, upperHSV, out);
	return out;
}

vector<Point2i> getLargestContour(const Mat1b &in, const int &mode){
	findContours(in, contours, RETR_EXTERNAL, mode, Point2f(0, 0));
	if (contours.empty()){
		return vector<Point2i>();
	}
	vector<int> indices(contours.size());
	iota(indices.begin(), indices.end(), 0);

	sort(indices.begin(), indices.end(), [&contours](int lhs, int rhs) {
		return contours[lhs].size() > contours[rhs].size();
	});

	return contours[indices[0]];
}

/**
 *
 * @param contour
 * @param width
 * @param height
 * @return A vector of 4 corners, in order top left, top right, bottom left, bottom right
 */
vector<Point2f> getCorners(const vector<Point2i> &contour, const int &width, const int &height){
	vector<Point2i> screenCorners = {Point2i(0,0), Point2i(width,0), Point2i(0,height), Point2i(width,height)};
	vector<Point2f> toRet(4);
	double minBL = INFINITY, minBR = INFINITY, minTL = INFINITY, minTR = INFINITY;
	for (const Point2i &i : contour) {
		if(norm(i - screenCorners[0]) < minBL){
			minBL = norm(i - screenCorners[0]);
			toRet[0] = i;
		}
		if(norm(i - screenCorners[1]) < minBR){
			minBR = norm(i - screenCorners[1]);
			toRet[1] = i;
		}
		if(norm(i - screenCorners[2]) < minTL){
			minTL = norm(i - screenCorners[2]);
			toRet[2] = i;
		}
		if(norm(i - screenCorners[3]) < minTR){
			minTR = norm(i - screenCorners[3]);
			toRet[3] = i;
		}
	}
	return toRet;
}

bool checkCorners(const vector<Point2f> &corners, const int &width, const int &height, const int &minDiag, const int &border){
	//Make sure they're unique
	if(corners[0] == corners[1] || corners[0] == corners[2] || corners[0] == corners[3] || corners[1] == corners[2] ||
			corners[1] == corners[3] || corners[2] == corners[3]){
		return false;
	}

	//Check that the diagonals are at least a certain length
	if(norm(corners[0]-corners[3]) < minDiag || norm(corners[1]-corners[2]) < minDiag){
		return false;
	}

	return (corners[0].x > border && corners[2].x > border && corners[0].y > border && corners[1].y > border &&
	corners[1].x < width-border && corners[3].x < width-border && corners[2].y < height - border && corners[2].y < height - border);
}

Mat1f getCameraPose(const vector<Point2f> &pictureCorners, const vector<Point3f> &objectCorners,
                    const Mat1f &cameraMatrix,
                    const Mat1f &distortionCoefficients){
	Mat1f rvec, tvec, zyx, final;
	solvePnP(objectCorners,pictureCorners, cameraMatrix, distortionCoefficients, rvec, tvec);
	Rodrigues(rvec, zyx);
	hconcat(zyx, tvec, final);
	vconcat(final,Mat(Vec4f(0,0,0,1)).t(), final);
	final = final.inv();
	return final;
}

void on_low_h_thresh_trackbar(int, void *)
{
	low_h = min(high_h-1, low_h);
	setTrackbarPos("Low H","Object Detection", low_h);
}
void on_high_h_thresh_trackbar(int, void *)
{
	high_h = max(high_h, low_h+1);
	setTrackbarPos("High H", "Object Detection", high_h);
}
void on_low_s_thresh_trackbar(int, void *)
{
	low_s = min(high_s-1, low_s);
	setTrackbarPos("Low S","Object Detection", low_s);
}
void on_high_s_thresh_trackbar(int, void *)
{
	high_s = max(high_s, low_s+1);
	setTrackbarPos("High S", "Object Detection", high_s);
}
void on_low_v_thresh_trackbar(int, void *)
{
	low_v= min(high_v-1, low_v);
	setTrackbarPos("Low V","Object Detection", low_v);
}
void on_high_v_thresh_trackbar(int, void *)
{
	high_v = max(high_v, low_v+1);
	setTrackbarPos("High V", "Object Detection", high_v);
}