//
// Created by noah on 11/12/17.
//

#ifndef JETSON_VISION_CORNER_FINDER_H
#define JETSON_VISION_CORNER_FINDER_H

#include "opencv2/opencv.hpp"

using namespace cv;
using namespace std;

class corner_finder {
public:
	/**
	 * Get the points in a list of points closest to the edge of the screen.
	 *
	 * @param contour The list of points
	 * @return A vector of 4 corners, in order top left, top right, bottom left, bottom right
	 */
	vector<Point2f> get_corners(const vector<Point2i> &contour);

	/**
	 * Default constructor.
	 *
	 * @param width The screen width, in pixels
     * @param height The screen height, in pixels.
	 */
	corner_finder(const int &width, const int &height);

private:
	vector<Point2i> screenCorners;
	vector<Point2f> toRet;
	double minBL, minBR, minTL, minTR;

};


#endif //JETSON_VISION_CORNER_FINDER_H
