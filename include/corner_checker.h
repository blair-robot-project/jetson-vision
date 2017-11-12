//
// Created by noah on 11/12/17.
//

#ifndef JETSON_VISION_CORNER_CHECKER_H
#define JETSON_VISION_CORNER_CHECKER_H

#include "opencv2/opencv.hpp"

using namespace cv;
using namespace std;

class corner_checker {
public:
	/**
	 * Default constructor.
	 *
	 * @param width The width of the screen, in pixels.
	 * @param height The height of the screen, in pixels.
	 * @param minDiag The minimum diagonal distance of the rectangle for it to be considered large enough
	 * @param border The number of pixels around the edge of the screen where, if the corners are within those pixels, the
     *  object is on the edge of the screen and doesn't count.
	 */
	corner_checker(const int &width, const int &height, const int &minDiag = 30,
	               const int &border = 0);

	/**
	 * Check the given corners of a rectangle and check that the object is large enough and not on the edge of the screen.
	 * @param corners The corners to check, in order top left, top right, bottom left, bottom right.
	 * @return true if the object is a non-edge rectangle, false otherwise.
	 */
	bool check_corners(const vector<Point2f> &corners);

private:
	int minDiag;
	int maxX, minX, maxY, minY;
};


#endif //JETSON_VISION_CORNER_CHECKER_H
