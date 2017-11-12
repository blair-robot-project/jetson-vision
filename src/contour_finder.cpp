//
// Created by noah on 11/12/17.
//

#include <numeric>
#include "contour_finder.h"

/**
 * Default constructor.
 *
 * @param mode The mode of contour approximation to use, either CHAIN_APPROX_SIMPLE or CHAIN_APPROX_NONE.
 */
contour_finder::contour_finder(const int &mode) {
	this->mode = mode;
}

/**
 * Get the largest contour in an image.
 *
 * @param in The matrix to find contours in.
 * @return a vector of the points in the contour.
 */
vector<Point2i> contour_finder::get_largest_contour(const Mat1b &in){
	findContours(in, this->contours, RETR_EXTERNAL, mode, Point2f(0, 0));
	if (this->contours.empty()){
		return vector<Point2i>();
	}

	nth_element(contours.begin(), contours.begin(), contours.end(), contour_finder::contour_comparator);

	return this->contours[0];
}

/**
 * Comparator for sorting contours.
 *
 * @param a A contour.
 * @param b Another contour.
 * @return True if a should go before b in the ordering, false otherwise.
 */
bool contour_finder::contour_comparator(const vector<Point> &a, const vector<Point> &b){
	return a.size() > b.size();
}