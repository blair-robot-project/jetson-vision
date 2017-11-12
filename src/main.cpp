#include <numeric>
#include "../include/main.h"
#include "../include/threshholder.h"

using namespace std;
using namespace cv;

/**
 * The upper and lower threshhold bounds.
 */
uchar low_h=40, low_s=135, low_v=55;
uchar high_h=60, high_s=255, high_v=160;

/**
 * Field to avoid garbage collection.
 */
vector<vector<Point2i>> contours;

/**
 * Constantly find a print position.
 * @param argc Number of arguments
 * @param argv Path to camera data file.
 * @return 0 if run without error, error code otherwise.
 */
int main(int argc, char* argv[]) {

	vector<Point3f> objPoints(4);
	objPoints[0] = Point3f(0,0,0);
	objPoints[1] = Point3f(2,0,0);
	objPoints[2] = Point3f(0,5.75,0);
	objPoints[3] = Point3f(2,5.75,0);

	Mat3b frame, goodFrame;
	Mat cameraMatrix, distCoeff;
	Mat1b threshed;
	Threshholder threshholder(cuda::createGaussianFilter(CV_8UC3,CV_8UC3, Size(5,5), 0, 0), Vec3b(low_h, low_s, low_v), Vec3b(high_h, high_s, high_v));
	vector<Point> cont;
	vector<Point2f> points(4);
	int width, height;

	VideoCapture cap("/media/LinHDD/Videos/my_video-1.mkv");

//	VideoCapture cap(0);

//	VideoCapture cap("/media/LinHDD/Images/vision/Screenshot from my_video-1.mkv.png");

	width = static_cast<int>(cap.get(CAP_PROP_FRAME_WIDTH));
	height = static_cast<int>(cap.get(CAP_PROP_FRAME_HEIGHT));

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

	while((char)waitKey(17)!='q'){
		cap>>frame;

		if(!frame.empty()) {
			goodFrame = frame.clone();
		}

		threshed = threshholder.threshhold(goodFrame);
		cont = getLargestContour(threshed, CHAIN_APPROX_SIMPLE);

		if (!cont.empty()) {
			points = getCorners(cont, width, height);

			if(checkCorners(points,width,height,30,0)){
				cout << "M = "<< endl << " "  << getCameraPose(points, objPoints, cameraMatrix, distCoeff) << endl << endl;
			}
		}

		imshow("Video Capture", goodFrame);
	}
	return 0;
}

/**
 * Get the largest contour in an image.
 *
 * @param in The matrix to find contours in.
 * @param mode The mode of contour approximation to use, either CHAIN_APPROX_SIMPLE or CHAIN_APPROX_NONE.
 * @return a vector of the points in the contour.
 */
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
 * Get the points in a list of points closest to the edge of the screen.
 * @param contour The list of points
 * @param width The screen width, in pixels
 * @param height The screen height, in pixels.
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

/**
 * Check the given corners of a rectangle and check that the object is large enough and not on the edge of the screen.
 * @param corners The corners to check, in order top left, top right, bottom left, bottom right.
 * @param width The width of the screen, in pixels.
 * @param height The height of the screen, in pixels.
 * @param minDiag The minimum diagonal distance of the rectangle for it to be considered large enough
 * @param border The number of pixels around the edge of the screen where, if the corners are within those pixels, the
 *  object is on the edge of the screen and doesn't count.
 * @return true if the object is a non-edge rectangle, false otherwise.
 */
bool checkCorners(const vector<Point2f> &corners, const int &width, const int &height, const int &minDiag, const int &border){
	//Make sure they're unique
	if(corners[0] == corners[1] || corners[0] == corners[2] || corners[0] == corners[3] || corners[1] == corners[2] ||
			corners[1] == corners[3] || corners[2] == corners[3]){
		return false;
	}

	//Check that the diagonals are at least a certain length and the points are a distance away from the edges.
	return !(norm(corners[0] - corners[3]) < minDiag || norm(corners[1] - corners[2]) < minDiag) &&
	       corners[0].x > border && corners[2].x > border && corners[0].y > border && corners[1].y > border &&
corners[1].x < width-border && corners[3].x < width-border && corners[2].y < height - border && corners[2].y < height - border;

}

/**
 * Get the position and rotation of the camera.
 * @param pictureCorners The 4 corners of the object as detected by the camera.
 * @param objectCorners The absolute, 3D position of the corners of the actual object.
 * @param cameraMatrix The matrix describing how the camera distorts images.
 * @param distortionCoefficients The matrix of coefficients for how the camera is distorted.
 * @return A 4x4 matrix with top-left 3x3 describing the rotation of the camera and the right column giving x,y,z position.
 */
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