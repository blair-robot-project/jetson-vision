//
// Created by noah on 11/12/17.
//

#ifndef JETSON_VISION_CONTOURFINDER_H
#define JETSON_VISION_CONTOURFINDER_H

#include "opencv2/opencv.hpp"

using namespace cv;
using namespace std;

class contour_finder {
public:
	/**
	 * Get the largest contour in an image.
	 *
	 * @param in The matrix to find contours in.
	 * @return a vector of the points in the contour.
	 */
	vector<Point2i> get_largest_contour(const Mat1b &in);

	/**
	 * Default constructor.
	 *
	 * @param mode The mode of contour approximation to use, either CHAIN_APPROX_SIMPLE or CHAIN_APPROX_NONE.
	 */
	explicit contour_finder(const int &mode);

private:
	vector<vector<Point2i>> contours;
	int mode;

	/**
	 * Comparator for sorting contours.
	 *
	 * @param a A contour.
	 * @param b Another contour.
	 * @return True if a should go before b in the ordering, false otherwise.
	 */
	static bool contour_comparator(const vector<Point> &a, const vector<Point> &b);
};

#endif //JETSON_VISION_CONTOURFINDER_H
