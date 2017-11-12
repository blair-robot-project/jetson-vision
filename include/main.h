//
// Created by noah on 11/11/17.
//

#ifndef JETSON_VISION_MAIN_H
#define JETSON_VISION_MAIN_H

#include "opencv2/opencv.hpp"

using namespace std;
using namespace cv;

/**
 * Get the largest contour in an image.
 *
 * @param in The matrix to find contours in.
 * @param mode The mode of contour approximation to use, either CHAIN_APPROX_SIMPLE or CHAIN_APPROX_NONE.
 * @return a vector of the points in the contour.
 */
vector<Point2i> getLargestContour(const Mat1b &in, const int &mode);

/**
 *
 * @param contour
 * @param width
 * @param height
 * @return A vector of 4 corners, in order top left, top right, bottom left, bottom right
 */
vector<Point2f> getCorners(const vector<Point2i> &contour, const int &width, const int &height);

/**
 * Get the points in a list of points closest to the edge of the screen.
 * @param contour The list of points
 * @param width The screen width, in pixels
 * @param height The screen height, in pixels.
 * @return A vector of 4 corners, in order top left, top right, bottom left, bottom right
 */
bool checkCorners(const vector<Point2f> &corners, const int &width, const int &height, const int &minDiag=30, const int &border=0);

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
                    const Mat1f &distortionCoefficients);


#endif //JETSON_VISION_MAIN_H
