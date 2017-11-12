//
// Created by noah on 11/12/17.
//

#include "corner_finder.h"

/**
 * Default constructor.
 *
 * @param width The screen width, in pixels
 * @param height The screen height, in pixels.
 */
corner_finder::corner_finder(const int &width, const int &height){
	this->screenCorners = {Point2i(0,0), Point2i(width,0), Point2i(0,height), Point2i(width,height)};
}

/**
 * Get the points in a list of points closest to the edge of the screen.
 *
 * @param contour The list of points
 * @return A vector of 4 corners, in order top left, top right, bottom left, bottom right
 */
vector<Point2f> corner_finder::get_corners(const vector<Point2i> &contour){
	this->toRet = vector<Point2f>(4);
	minBL = INFINITY;
	minBR = INFINITY;
	minTL = INFINITY;
	minTR = INFINITY;
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