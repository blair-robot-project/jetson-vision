#include "opencv2/opencv.hpp"
#include <iostream>
#include <chrono>

using namespace std;
using namespace cv;

void on_low_h_thresh_trackbar(int, void *);
void on_high_h_thresh_trackbar(int, void *);
void on_low_s_thresh_trackbar(int, void *);
void on_high_s_thresh_trackbar(int, void *);
void on_low_v_thresh_trackbar(int, void *);
void on_high_v_thresh_trackbar(int, void *);
int low_h=100, low_s=60, low_v=40;
int high_h=255, high_s=135, high_v=70;
//GPU: 4577282520, 5513312761
//CPU: 7645543143, 8696878819

int main(int argc, char* argv[]) {

	cout << "CUDA devices: " << cuda::getCudaEnabledDeviceCount() << endl;

	Mat frame, frame_threshold, cameraMatrix, distCoeff, map1, map2;
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;
	Ptr<cuda::Filter> gBlur;
	auto startTime = std::chrono::high_resolution_clock::now();
	auto totalTime = std::chrono::high_resolution_clock::now()-std::chrono::high_resolution_clock::now();

	VideoCapture cap("/media/LinHDD/Videos/test.mkv");

//	cap >> frame;
//	Size imageSize = frame.size();
//
//	const string inputSettingsFile = argc > 1 ? argv[1] : "default.yml";
//
//	FileStorage fs(inputSettingsFile, FileStorage::READ); // Read the settings
//	if (!fs.isOpened())
//	{
//		cout << "Could not open the configuration file: \"" << inputSettingsFile << "\"" << endl;
//		return -1;
//	}
//
//	fs["camera_matrix"] >> cameraMatrix;
//	fs["distortion_coefficients"] >> distCoeff;
//
//	initUndistortRectifyMap(cameraMatrix, distCoeff, Mat(),
//	                        getOptimalNewCameraMatrix(cameraMatrix, distCoeff, imageSize,
//	                                                  1, imageSize, 0), imageSize, CV_16SC2, map1, map2);

	namedWindow("Video Capture", WINDOW_NORMAL);
	namedWindow("Object Detection", WINDOW_FULLSCREEN);
	//-- Trackbars to set thresholds for RGB values
	createTrackbar("Low H","Object Detection", &low_h, 255, on_low_h_thresh_trackbar);
	createTrackbar("High H","Object Detection", &high_h, 255, on_high_h_thresh_trackbar);
	createTrackbar("Low S", "Object Detection", &low_s, 255, on_low_s_thresh_trackbar);
	createTrackbar("High S", "Object Detection", &high_s, 255, on_high_s_thresh_trackbar);
	createTrackbar("Low V", "Object Detection", &low_v, 255, on_low_v_thresh_trackbar);
	createTrackbar("High V", "Object Detection", &high_v, 255, on_high_v_thresh_trackbar);
	while((char)waitKey(17)!='q'){
		startTime = std::chrono::high_resolution_clock::now();
		cap>>frame;
		if(!frame.empty()) {
//		remap(frame, frame, map1, map2, INTER_LINEAR);
			cvtColor(frame, frame_threshold, COLOR_BGR2HSV);

			GaussianBlur(frame_threshold, frame_threshold,Size(3,3), 0, 0);
			//-- Detect the object based on RGB Range Values
			inRange(frame_threshold, Scalar(low_v, low_s, low_h), Scalar(high_v, high_s, high_h), frame_threshold);
			findContours(frame_threshold, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0));
			drawContours(frame, contours, -1, Scalar(0, 255, 0));
			//-- Show the frames
			imshow("Video Capture", frame);
			imshow("Object Detection", frame_threshold);
		} else {
			break;
		}
		totalTime += std::chrono::high_resolution_clock::now()-startTime;
	}
	cout << "Took "<< std::chrono::duration_cast<std::chrono::nanoseconds>(totalTime).count() << " nanoseconds.";
	return 0;
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