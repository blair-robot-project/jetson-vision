#include <numeric>
#include "../include/main.h"

using namespace std;
using namespace cv;

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
	threshholder threshholder(cuda::createGaussianFilter(CV_8UC3,CV_8UC3, Size(5,5), 0, 0), Vec3b(40, 135, 55),
	                          Vec3b(60, 255, 160));
	contour_finder contourFinder(CHAIN_APPROX_SIMPLE);
	vector<Point> cont;
	vector<Point2f> points(4);
	int width, height;

	VideoCapture cap("/media/LinHDD/Videos/my_video-1.mkv");

//	VideoCapture cap(0);

//	VideoCapture cap("/media/LinHDD/Images/vision/Screenshot from my_video-1.mkv.png");

	width = static_cast<int>(cap.get(CAP_PROP_FRAME_WIDTH));
	height = static_cast<int>(cap.get(CAP_PROP_FRAME_HEIGHT));

	corner_finder cornerFinder(width, height);
	corner_checker cornerChecker(width, height, 30, 20);

	const string inputSettingsFile = argc > 1 ? argv[1] : "default.yml";

	FileStorage fs(inputSettingsFile, FileStorage::READ); // Read the settings
	if (!fs.isOpened())
	{
		cout << "Could not open the configuration file: \"" << inputSettingsFile << "\"" << endl;
		return -1;
	}

	fs["camera_matrix"] >> cameraMatrix;
	fs["distortion_coefficients"] >> distCoeff;

	pose_estimator poseEstimator = pose_estimator(objPoints, cameraMatrix, distCoeff);

	namedWindow("Video Capture", WINDOW_NORMAL);

	while((char)waitKey(17)!='q'){
		cap>>frame;

		if(!frame.empty()) {
			goodFrame = frame.clone();
		}

		threshed = threshholder.threshhold(goodFrame);
		cont = contourFinder.get_largest_contour(threshed);

		if (!cont.empty()) {
			points = cornerFinder.get_corners(cont);

			if(cornerChecker.check_corners(points)){
				cout << "M = "<< endl << " "  << poseEstimator.get_pose(points) << endl << endl;
			}
		}

		imshow("Video Capture", goodFrame);
	}
	return 0;
}