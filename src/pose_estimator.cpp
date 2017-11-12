//
// Created by noah on 11/12/17.
//

#include "pose_estimator.h"

/**
 * Default constructor.
 *
 * @param objectCorners The absolute, 3D position of the corners of the actual object.
 * @param cameraMatrix The matrix describing how the camera distorts images.
 * @param distortionCoefficients The matrix of coefficients for how the camera is distorted.
 */
pose_estimator::pose_estimator(const vector<Point3f> &objectCorners, const Mat1f &cameraMatrix,
                               const Mat1f &distortionCoefficients) {
	this->objectCorners = objectCorners;
	this->cameraMatrix = cameraMatrix;
	this->distortionCoefficients = distortionCoefficients;
}

/**
 * Get the position and rotation of the camera.
 * @param corners The 4 corners of the object as detected by the camera.
 * @return A 4x4 matrix with top-left 3x3 describing the rotation of the camera and the right column giving x,y,z position.
 */
Mat1f pose_estimator::get_pose(const vector<Point2f> &corners) {
	rvec = Mat();
	tvec = Mat();
	solvePnP(objectCorners, corners, cameraMatrix, distortionCoefficients, rvec, tvec);
	Rodrigues(rvec, zyx);
	hconcat(zyx, tvec, final);
	vconcat(final,bottom, final);
	final = final.inv();
	return final;
}