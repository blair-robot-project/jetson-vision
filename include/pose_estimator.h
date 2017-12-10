//
// Created by noah on 11/12/17.
//

#ifndef JETSON_VISION_POSE_ESTIMATOR_H
#define JETSON_VISION_POSE_ESTIMATOR_H

#include "opencv2/opencv.hpp"

using namespace cv;
using namespace std;

class pose_estimator {
public:

	/**
	 * Default constructor.
	 *
	 * @param objectCorners The absolute, 3D position of the corners of the actual object.
	 * @param cameraMatrix The matrix describing how the camera distorts images.
	 * @param distortionCoefficients The matrix of coefficients for how the camera is distorted.
	 */
	pose_estimator(const vector<Point3f> &objectCorners,
	               const Mat1f &cameraMatrix,
	               const Mat1f &distortionCoefficients);

	/**
	 * Get the position and rotation of the camera.
     * @param corners The 4 corners of the object as detected by the camera.
	 * @return A 4x4 matrix with top-left 3x3 describing the rotation of the camera and the right column giving x,y,z position.
	 */
	Mat get_pose(const vector<Point2f> &corners);

private:
	vector<Point3f> objectCorners;
	Mat1f cameraMatrix, distortionCoefficients;
	Mat rvec, tvec, zyx, final;
	const Mat bottom = Mat(Vec4d(0,0,0,1)).t();

};


#endif //JETSON_VISION_POSE_ESTIMATOR_H
